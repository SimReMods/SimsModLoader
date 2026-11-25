// eyedropper.cpp - mods\eyedropper_steam.dll / _ea.dll / _cc.dll: the build / buy mode eyedropper (see README.md).
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <new>
#include "simsmod.h"
#include "game_api.h"
#if GAME_CC
#include "gen_sigs_cc.h"
#elif GAME_EA
#include "gen_sigs_ea.h"
#else
#include "gen_sigs.h"
#endif

extern "C" __declspec(dllexport) const unsigned SimsMod_Games =
    GAME_CC ? SIMSMOD_GAME_COMPLETE : GAME_EA ? SIMSMOD_GAME_LEGACY_EA : SIMSMOD_GAME_LEGACY;

unsigned ga::g_exeBase;
static const SimsLoaderApi *s_api;

template <class T> using Id = T;
#define ORIG(name, ...) static Id<__VA_ARGS__> o_##name

enum
{
    kEyedropperBtnImage = 9520,
    kEyedropperCursorId = 9520,
};

static void Eyedropper_Cancel(CPState *pState);
static bool Eyedropper_Toggle(CPState *pState);
static bool Eyedropper_IsActive();

namespace
{

enum PickKind
{
    kPickNone,
    kPickObject,
    kPickWall,
    kPickFloor,
};

struct Pick
{
    PickKind nKind;
    cXObject *pObject;
    CTilePt tile;
    int nSegment;
    int nWallStyle;
    int nWallpaper;
    int nFloor;
};

int SegmentIndex(int nSegment)
{
    int nIndex = 0;
    while (nSegment > 1)
    {
        nSegment >>= 1;
        nIndex++;
    }
    return nIndex;
}

unsigned int FloorQuadrant(const FTilePt *pPt, int nLevel)
{
    FTilePt ptTileScreen16;
    ptTileScreen16.nPosY16 = (signed char)(pPt->nPosX16 >> 4);
    ptTileScreen16.nPosX16 = (signed char)(pPt->nPosY16 >> 4);
    ScreenPt16 ptTileScreen = GViewer::TileToPoint(&ptTileScreen16, nLevel);
    short nTileCenterY = (short)(GViewer::GetTileHeight() / 2 + ptTileScreen.nY);
    ScreenPt16 ptSubTileScreen = GViewer::SubTileToPoint(pPt, nLevel);
    if (ptSubTileScreen.nX < ptTileScreen.nX)
        return ptSubTileScreen.nY < nTileCenterY ? 0 : 2;
    return ptSubTileScreen.nY < nTileCenterY ? 1 : 3;
}

int WallSegmentFlags(const CTilePt &tile, int nSegment)
{
    WallManager *pMgr = WallManager::Get();
    int nLevel = tile.nLevel;
    if (!pMgr || nLevel < 1 || nLevel > 2 || !pMgr->m_pWallLists[nLevel - 1])
        return -1;
    const Vec<Wall *> &walls = *pMgr->m_pWallLists[nLevel - 1];
    for (Wall *pWall : walls)
    {
        if (!pWall || pWall->m_bIconic || pWall->m_nDir < 0 || pWall->m_nDir > 7)
            continue;
        if (TileWalls::DirToWallSeg(pWall->m_nDir) != nSegment)
            continue;
        CTilePt vertex = pWall->m_TileA;
        for (size_t i = 0; i < pWall->m_SegmentsA.size(); i++)
        {
            CTilePt tileSeg = Wall::VertexToTile(vertex, pWall->m_nDir);
            if (tileSeg.nX == tile.nX && tileSeg.nY == tile.nY)
                return (unsigned char)pWall->m_SegmentsA[i].m_nFlags;
            vertex.nX += pWall->m_nStepX;
            vertex.nY += pWall->m_nStepY;
        }
    }
    return -1;
}

int WallHeightPx(const CTilePt &tile, int nSegment, int nStoryPx)
{
    int nFlags = WallSegmentFlags(tile, nSegment);
    if (nFlags >= 0 && (nFlags & 1))
        return nStoryPx / 4;
    return nStoryPx;
}

bool WallAt(const CTilePt &tile, int nSegment, int nHeight, int nStoryPx, Pick *pOut)
{
    if (!Lot()->IsInLot(tile))
        return false;
    TileWalls walls = Lot()->GetWall(tile);
    if (!(walls.m_nWallMask & nSegment))
        return false;
    if (nHeight > WallHeightPx(tile, nSegment, nStoryPx))
        return false;
    pOut->nKind = kPickWall;
    pOut->tile = tile;
    pOut->nSegment = nSegment;
    pOut->nWallStyle = walls.m_nPattern[SegmentIndex(nSegment)];
    pOut->nWallpaper = walls.GetEdgeValue(nSegment, 0);
    return true;
}

bool FindWall(ScreenPt16 pt, int nLevel, Pick *pOut, FTilePt *pFloorPt)
{
    HouseViewer *pViewer = Settings()->m_pHouseViewer;
    int nScale = GViewer::GetScale();
    if (nScale < 1 || nScale > 3)
        nScale = 1;
    int nStoryPx = pViewer->m_anLevelHeightPx[3 - nScale];
    if (nStoryPx <= 0)
        nStoryPx = 32 << nScale;
    ScreenPt16 ptRay = pt;
    ptRay.nY = (short)(pt.nY + nStoryPx);
    FTilePt prev = GViewer::PointToTile(ptRay, true);
    for (int h = nStoryPx - 1; h >= 0; h--)
    {
        ptRay.nY = (short)(pt.nY + h);
        FTilePt cur = GViewer::PointToTile(ptRay, true);
        CTilePt tilePrev(prev.nPosX16 >> 4, prev.nPosY16 >> 4, nLevel);
        CTilePt tileCur(cur.nPosX16 >> 4, cur.nPosY16 >> 4, nLevel);
        if (Lot()->IsInLot(tilePrev))
        {
            int nMask = Lot()->GetWall(tilePrev).m_nWallMask;
            if (nMask & kWallSeg_AnyDiagonal)
            {
                int nBaseX = tilePrev.nX * 16, nBaseY = tilePrev.nY * 16;
                int ax = prev.nPosX16 - nBaseX, ay = prev.nPosY16 - nBaseY;
                int bx = cur.nPosX16 - nBaseX, by = cur.nPosY16 - nBaseY;
                if (nMask & kWallSeg_DiagNE_SW)
                {
                    if (((ax - ay) > 0) != ((bx - by) > 0) && WallAt(tilePrev, kWallSeg_DiagNE_SW, h, nStoryPx, pOut))
                        return true;
                }
                if (nMask & kWallSeg_DiagNW_SE)
                {
                    if (((ax + ay - 16) > 0) != ((bx + by - 16) > 0) && WallAt(tilePrev, kWallSeg_DiagNW_SE, h, nStoryPx, pOut))
                        return true;
                }
            }
        }
        if (tileCur.nX < tilePrev.nX && WallAt(tilePrev, kWallSeg_West, h, nStoryPx, pOut))
            return true;
        if (tileCur.nY < tilePrev.nY && WallAt(tilePrev, kWallSeg_North, h, nStoryPx, pOut))
            return true;
        if (tileCur.nX < tilePrev.nX && tileCur.nY < tilePrev.nY)
        {
            if (WallAt(CTilePt(tilePrev.nX, tileCur.nY, nLevel), kWallSeg_West, h, nStoryPx, pOut))
                return true;
            if (WallAt(CTilePt(tileCur.nX, tilePrev.nY, nLevel), kWallSeg_North, h, nStoryPx, pOut))
                return true;
        }
        prev = cur;
    }
    *pFloorPt = prev;
    return false;
}

bool FindFloor(const FTilePt &pt, int nLevel, Pick *pOut)
{
    cRotatableWorld *pLot = Lot();
    CTilePt tile(pt.nPosX16 >> 4, pt.nPosY16 >> 4, nLevel);
    if (!pLot->IsInLot(tile) || nLevel < 1 || nLevel >= 3 || !pLot->m_pFloorsByLevel[nLevel])
        return false;
    CTilePt tileFloor = pLot->Rotate(tile);
    int nPattern = pLot->m_pFloorsByLevel[nLevel]->m_pRows[tileFloor.nX][tileFloor.nY];
    TileWalls walls = pLot->GetWall(tile);
    if (walls.m_nWallMask & kWallSeg_AnyDiagonal)
    {
        unsigned int nQuadrant = FloorQuadrant(&pt, nLevel);
        int nSide;
        if (walls.m_nWallMask & kWallSeg_DiagNE_SW)
            nSide = (nQuadrant == 0 || nQuadrant == 1) ? 2 : 4;
        else
            nSide = (nQuadrant == 0 || nQuadrant == 2) ? 1 : 3;
        nPattern = walls.GetDiagonalFloorPattern(nSide);
    }
    if (nPattern <= 0 || nPattern == 255)
        return false;
    pOut->nKind = kPickFloor;
    pOut->tile = tile;
    pOut->nFloor = nPattern;
    return true;
}

cXObject *ObjectUnder(ScreenPt16 pt, int nLevel)
{
    HouseViewer *pViewer = Settings()->m_pHouseViewer;
    if (pt.nX < pViewer->m_ViewRect.left || pt.nX >= pViewer->m_ViewRect.right || pt.nY < pViewer->m_ViewRect.top ||
        pt.nY >= pViewer->m_ViewRect.bottom)
        return 0;
#if GAME_CC
    int nId = GViewer::PointToObjectID(pt, nLevel);
    cXObject *pObject = nId > 0 ? cTool::ToolGetObject(nId) : 0;
#else
    (void)nLevel;
    PickHitResult hit;
    if (!pViewer->m_RenderContext.HitTestScreenPoint(pt.nX, pt.nY, &hit) || !hit.m_pRecord || hit.m_fDepth >= 1.0f)
        return 0;
    if (hit.m_pRecord->m_pMeshItem || !hit.m_pRecord->m_pDrawItem)
        return 0;
    cXObject *pObject = reinterpret_cast<ObjectHandle *>(hit.m_pRecord->m_pDrawItem)->Resolve();
#endif
    if (!pObject || DynCast(pObject, A::TD_cXPerson))
        return 0;
    return pObject;
}

FTilePt ObjectViewPos(cXObject *pObject)
{
    FTilePt pos;
    pos.nPosY16 = pObject->m_nPosY16;
    pos.nPosX16 = pObject->m_nPosX16;
    Lot()->RotateCoords(&pos.nPosX16, &pos.nPosY16, Lot()->GetToViewRotation(), 1024);
    return pos;
}

bool ObjectBehindWall(cXObject *pObject, const Pick &wall)
{
    FTilePt pos = ObjectViewPos(pObject);
    int nX = pos.nPosX16 >> 4, nY = pos.nPosY16 >> 4;
    if (wall.nSegment == kWallSeg_West)
        return nX < wall.tile.nX;
    if (wall.nSegment == kWallSeg_North)
        return nY < wall.tile.nY;
    return false;
}

Product *ProductOfObject(Catalog *pCatalog, cXObject *pObject);

bool IsInWall(cXObject *pObject)
{
    CPState *pState = SimsApp() ? SimsApp()->m_pCPState : 0;
    Catalog *pCatalog = pState ? pState->GetCatalog() : 0;
    Product *pProduct = pCatalog ? ProductOfObject(pCatalog, pObject) : 0;
    return pProduct && (pProduct->TestArchTool(8) || pProduct->TestArchTool(9));
}

Pick PickAt(ScreenPt16 pt)
{
    Pick pick;
    memset(&pick, 0, sizeof(pick));
    if (!Lot() || !Settings() || !Settings()->m_pHouseViewer)
        return pick;
    int nLevel = cTool::GetCurrentLevel();
    if (nLevel < 1)
        nLevel = 1;
    cXObject *pObject = ObjectUnder(pt, nLevel);
    Pick wall;
    memset(&wall, 0, sizeof(wall));
    FTilePt floorPt;
    bool bWall = FindWall(pt, nLevel, &wall, &floorPt);
    if (pObject && bWall && ObjectBehindWall(pObject, wall) && !IsInWall(pObject))
        pObject = 0;
    if (pObject)
    {
        pick.nKind = kPickObject;
        pick.pObject = pObject;
        return pick;
    }
    if (bWall)
        return wall;
    FindFloor(floorPt, nLevel, &pick);
    return pick;
}

const int kArchToolsForObjects[] = { 8, 9, 4, 5, 6, 1, 2 };

Product *ProductOfObject(Catalog *pCatalog, cXObject *pObject)
{
    ObjSelector *aSelectors[3] = { pObject->m_pSelector, 0, 0 };
    if (pObject->m_pSelector)
        aSelectors[1] = pObject->m_pSelector->m_pMasterSelector;
    if (cXMTObject *pPart = (cXMTObject *)DynCast(pObject, A::TD_cXMTObject))
    {
        if (pPart->m_pMaster)
            aSelectors[2] = pPart->m_pMaster->m_pSelector;
    }
    for (ObjSelector *pSelector : aSelectors)
    {
        if (!pSelector)
            continue;
        if (Product *pProduct = pCatalog->FindProduct(pSelector))
            return pProduct;
    }
    return 0;
}

Product *FindProductOfKind(Catalog *pCatalog, int nKind, int nValue)
{
    for (Product **it = pCatalog->begin(); it != pCatalog->end(); ++it)
    {
        Product *pProduct = *it;
        if (!pProduct || pProduct->m_nKind != nKind)
            continue;
        if ((nKind == kProductEntry_Floor && pProduct->m_nFloorId == nValue) ||
            (nKind == kProductEntry_WallPattern && pProduct->m_nWallId == nValue) ||
            (nKind == kProductEntry_Wall && pProduct->m_nWallStyle == nValue))
            return pProduct;
    }
    return 0;
}

Product *ProductOfPick(const Pick &pick, bool bWallType, int *pnArchTool)
{
    *pnArchTool = -1;
    CPState *pState = SimsApp() ? SimsApp()->m_pCPState : 0;
    if (!pState)
        return 0;
    Catalog *pCatalog = pState->GetCatalog();
    if (!pCatalog)
        return 0;
    Product *pProduct = 0;
    switch (pick.nKind)
    {
    case kPickObject:
        pProduct = ProductOfObject(pCatalog, pick.pObject);
        if (pProduct)
        {
            for (int nTool : kArchToolsForObjects)
            {
                if (pProduct->TestArchTool(nTool))
                {
                    *pnArchTool = nTool;
                    break;
                }
            }
        }
        break;
    case kPickWall:
        if (!bWallType && !IsFenceStyle(pick.nWallStyle))
            pProduct = FindProductOfKind(pCatalog, kProductEntry_WallPattern, pick.nWallpaper);
        if (pProduct)
            *pnArchTool = 3;
        else
        {
            pProduct = FindProductOfKind(pCatalog, kProductEntry_Wall, pick.nWallStyle);
            if (pProduct)
                *pnArchTool = 2;
        }
        break;
    case kPickFloor:
        pProduct = FindProductOfKind(pCatalog, kProductEntry_Floor, pick.nFloor);
        if (pProduct)
            *pnArchTool = 7;
        break;
    default:
        break;
    }
    if (pProduct && *pnArchTool >= 0 && !pProduct->TestArchTool(*pnArchTool))
        pProduct = 0;
    return pProduct;
}

void SelectInBuildMode(CPState *pState, Product *pProduct, int nArchTool)
{
    if (pState->GetMode() != CPState::kBuild)
    {
        if (pState->GetCPMode() == 1)
            pState->SetCPMode(0, 0);
        pState->SetMode(CPState::kBuild, 0);
    }
    pState->m_pModeProducts[nArchTool] = pProduct;
    pState->ArchSetCurrentTool(nArchTool);
    pState->m_dwFlags |= CPState::kDirtyArchSubtool | CPState::kDirtyTool;
    cWinArch *pArch = SimsApp()->m_pCPanel ? SimsApp()->m_pCPanel->m_pArch : 0;
    if (pArch && nArchTool != 10)
    {
        // the panel turns to the product's page only on a tool change
        if (pArch->m_nMode == nArchTool)
            pArch->m_nMode = nArchTool == 0 ? 1 : 0;
        pArch->UpdateViewFromCPState(pState);
    }
}

bool ListedUnder(Product *p, int nGroup, int nCat, int nSub)
{
    typedef cWinCatalogStatic W;
    switch (nGroup)
    {
    case kCatalogGroup_Room:
        return p->HasRoomSort() && p->TestInRoom(W::GetRoomMask(nCat)) && p->TestHasFunction(W::GetFunctionMask(nSub));
    case kCatalogGroup_Function:
        return p->HasRoomSort() && p->TestHasFunction(W::GetFunctionMask(nCat), W::GetFunctionSubMask(nSub));
    case kCatalogGroup_Downtown:
        return p->HasDowntownSort() && p->TestInDowntownRoom(W::GetDowntownMask(nCat)) &&
               p->TestHasFunction(W::GetFunctionMask(nSub));
    case kCatalogGroup_Vacation:
        return p->HasVacationSort() && p->TestInVacationRoom(W::GetVacationMask(nCat)) &&
               p->TestHasFunction(W::GetFunctionMask(nSub));
    case kCatalogGroup_Community:
        return p->HasCommunitySort() && p->TestInCommunityRoom(W::GetCommunityMask(nCat)) &&
               p->TestHasFunction(W::GetFunctionMask(nSub));
    case kCatalogGroup_Studiotown:
        return p->HasStudiotownSort() && p->TestInStudiotownRoom(W::GetStudiotownMask(nCat)) &&
               p->TestHasFunction(W::GetFunctionMask(nSub));
    case kCatalogGroup_Magictown:
        return p->HasMagictownSort() && p->TestInMagictownRoom(W::GetMagictownMask(nCat)) &&
               p->TestHasFunction(W::GetFunctionMask(nSub));
    }
    return false;
}

bool FindBuyListing(Product *pProduct, int nGroup, int *pnCat, int *pnSub)
{
    static const int kFunctionSubs[] = { 7, 0, 1, 2, 3, 4, 5, 6 };
    static const int kSubs[] = { 0, 1, 2, 3, 4, 5, 6, 7 };
    const int *pSubs = nGroup == kCatalogGroup_Function ? kFunctionSubs : kSubs;
    for (int nCat = 0; nCat < 8; nCat++)
        for (int i = 0; i < 8; i++)
            if (ListedUnder(pProduct, nGroup, nCat, pSubs[i]))
            {
                *pnCat = nCat;
                *pnSub = pSubs[i];
                return true;
            }
    return false;
}

bool SelectInBuyMode(CPState *pState, Product *pProduct)
{
    cWinCPanel *pPanel = SimsApp() ? SimsApp()->m_pCPanel : 0;
    cWinCatalog *pWin = pPanel ? pPanel->m_pCatalog : 0;
    if (!pWin)
        return false;
    if (pState->GetMode() != CPState::kBuy)
    {
        if (pState->GetCPMode() == 1)
            pState->SetCPMode(0, 0);
        pState->SetMode(CPState::kBuy, 0);
        pState->ResetPreviousPages();
    }
    int aGroups[3] = { pState->m_nCatalogGroup, kCatalogGroup_Function, kCatalogGroup_Room };
    int nGroup = -1, nCat = 0, nSub = 0;
    for (int g : aGroups)
    {
        if (FindBuyListing(pProduct, g, &nCat, &nSub))
        {
            nGroup = g;
            break;
        }
    }
    if (nGroup < 0)
        return false;
    if (pState->m_nCatalogGroup != nGroup)
        pState->ShopSetSort(nGroup);
    pState->m_CatalogSel[0] = 1;
    pState->m_CatalogSel[2 * nGroup + 1] = nCat;
    pState->m_CatalogSel[2 * nGroup + 2] = nSub;
    Catalog *pCatalog = pState->GetCatalog();
    int nIndex = 0;
    for (Product **it = pCatalog->begin(); it != pCatalog->end(); ++it)
    {
        if (*it == pProduct)
            break;
        if (*it && ListedUnder(*it, nGroup, nCat, nSub))
            nIndex++;
    }
    pState->m_nCatalogPage = 0;
    pState->m_dwFlags |= CPState::kDirtyCatalog;
    pWin->UpdateViewFromCPState(pState);
    int nPerPage = pWin->m_nItemsPerPage > 0 ? pWin->m_nItemsPerPage : 1;
    pState->m_nCatalogPage = nIndex / nPerPage;
    pState->m_dwFlags |= CPState::kDirtyCatalog;
    pWin->UpdateViewFromCPState(pState);
    pWin->SetProduct(pProduct);
    return true;
}

const unsigned long kEyedropperMsg_EnterWorld = 0x45594544;
const unsigned int kToolUpdateFlags = 0x630;

// [0] the game's RTTI locator, then the game's slots with some replaced (thiscall as __fastcall, edx unused)
void *s_ToolVtbl[1 + A::cTool__vftable_Slots];
void *s_BtnVtbl[1 + A::cTSWinBtn__vftable_Slots];
void *s_pfnBtnTSOnCommand, *s_pfnBtnTSPaint;

struct EyeBtn;
EyeBtn *s_pButton;

void EnterWorldNow()
{
    CPState *pState = SimsApp() ? SimsApp()->m_pCPState : 0;
    if (!pState || !pState->m_pCurrentTool)
        return;
    pState->UpdateWorldFromCPState(CPState::kDirtyTool);
    long nX = 0, nY = 0;
    WinMgr()->GetCursorScreenPosition(nX, nY);
    if (SimsApp()->m_pLotView && WinMgr()->GetWindowFromPoint(nX, nY) == (cTSWin *)SimsApp()->m_pLotView)
        pState->MouseEnteredWorld();
}
void AfterPick();

bool ShiftDown()
{
    return (GetKeyState(VK_SHIFT) & 0x8000) != 0;
}

int EyedropperCursor()
{
    static int s_nCursor = -1;
    if (s_nCursor >= 0)
        return s_nCursor;
    s_nCursor = kCursor_Finger;
    char szPath[MAX_PATH];
    _snprintf_s(szPath, sizeof szPath, _TRUNCATE, "%s\\UIGraphics\\Eyedropper\\Eyedropper.cur", s_api->exeDir);
    if (GetFileAttributesA(szPath) != INVALID_FILE_ATTRIBUTES)
    {
        cTSCursorManager *pMgr = WinMgr() ? WinMgr()->m_pCursorMgr : 0;
        if (pMgr && (pMgr->GetCursor(kEyedropperCursorId) || pMgr->AddCursorFromFile(szPath, kEyedropperCursorId)))
            s_nCursor = kEyedropperCursorId;
    }
    return s_nCursor;
}

struct EyeTool
{
    unsigned char base[cTool::kSize];
    bool m_bArmed;
    Product *m_pHoverProduct;
    cTool *Tool() { return (cTool *)this; }
};
EyeTool *s_pTool;

void *__fastcall T_Dtor(EyeTool *self, void *, unsigned)
{
    return self;
}
void __fastcall T_Select(EyeTool *self, void *)
{
    self->m_bArmed = false;
    self->m_pHoverProduct = 0;
    cTool::SetCursor(EyedropperCursor());
}
void __fastcall T_Deselect(EyeTool *self, void *)
{
    self->m_bArmed = false;
    self->m_pHoverProduct = 0;
    cTool::SetToolTip(0);
    if (SimsApp() && SimsApp()->m_pCPState)
        SimsApp()->m_pCPState->m_dwFlags |= CPState::kDirtyTool;
}
void __fastcall T_Float(EyeTool *self, void *, FTilePt, short, EventRecord *pEvent)
{
    int nArchTool;
    Pick pick = PickAt(pEvent->m_ptWhere);
    Product *pProduct = ProductOfPick(pick, ShiftDown(), &nArchTool);
    if (pProduct != self->m_pHoverProduct)
    {
        self->m_pHoverProduct = pProduct;
        cTool::SetToolTip(0);
    }
    if (pProduct)
        cTool::SetToolTip(pProduct->GetProductName().c_str());
}
void __fastcall T_Click(EyeTool *self, void *, FTilePt, short, EventRecord *)
{
    self->m_bArmed = true;
}
void __fastcall T_Release(EyeTool *self, void *, FTilePt, short, EventRecord *pEvent)
{
    if (!self->m_bArmed)
        return;
    self->m_bArmed = false;
    CPState *pState = SimsApp() ? SimsApp()->m_pCPState : 0;
    if (!pState)
        return;
    int nArchTool;
    Pick pick = PickAt(pEvent->m_ptWhere);
    Product *pProduct = ProductOfPick(pick, ShiftDown(), &nArchTool);
    if (!pProduct)
    {
        self->Tool()->DoDeniedSound();
        return;
    }
    cTool::SetToolTip(0);
    if (Settings()->m_pSoundSys)
        Settings()->m_pSoundSys->PlayBySource("UI_object_move", -4);
    if (nArchTool >= 0)
        SelectInBuildMode(pState, pProduct, nArchTool);
    else if (!SelectInBuyMode(pState, pProduct))
    {
        self->Tool()->DoDeniedSound();
        return;
    }
    AfterPick();
}
bool __fastcall T_OnKeyDown(EyeTool *, void *, unsigned long nKey, unsigned long)
{
    if (nKey == VK_ESCAPE && SimsApp() && SimsApp()->m_pCPState)
    {
        Eyedropper_Cancel(SimsApp()->m_pCPState);
        return true;
    }
    return false;
}
void __fastcall T_OnEnterWorld(EyeTool *, void *)
{
    cTool::SetCursor(EyedropperCursor());
}
void __fastcall T_OnExitWorld(EyeTool *self, void *)
{
    self->m_pHoverProduct = 0;
    cTool::SetToolTip(0);
}
void __fastcall T_Cancel(EyeTool *self, void *)
{
    self->m_bArmed = false;
}
bool __fastcall T_ControlsOwnToolTips(EyeTool *, void *)
{
    return true;
}

const long kUndoY = 7, kRedoY = 38, kEyedropperY = 69;

struct EyeBtn
{
    unsigned char base[cTSWinBtn::kSize];
    cWinArch *m_pArch;
    int m_nEnterWorldRetries;
    cTSWinBtn *Btn() { return (cTSWinBtn *)this; }
};

void *__fastcall B_Dtor(EyeBtn *self, void *, unsigned nFlags)
{
    if (s_pButton == self)
        s_pButton = 0;
    ThisCall<void>(A::cTSWinBtn___cTSWinBtn, self);
    if (nFlags & 1)
        free(self);
    return self;
}
bool __fastcall B_TSOnCommand(EyeBtn *self, void *, unsigned long nCommand, TSMsgParam nParam)
{
    if (nCommand == kWinMsg_ButtonClick && (EyeBtn *)nParam == self)
    {
        if (SimsApp() && SimsApp()->m_pCPState)
            Eyedropper_Toggle(SimsApp()->m_pCPState);
        self->Btn()->SetState(Eyedropper_IsActive());
        return true;
    }
    if (nCommand == kEyedropperMsg_EnterWorld)
    {
        CPState *pState = SimsApp() ? SimsApp()->m_pCPState : 0;
        // wait until CPState has applied the pending tool change, or it cancels the new tool
        if (pState && (pState->m_dwFlags & kToolUpdateFlags) && self->m_nEnterWorldRetries < 30)
        {
            self->m_nEnterWorldRetries++;
            WinMgr()->PostMessageToTarget(self->Btn(), kTSMsg_Command, kEyedropperMsg_EnterWorld, (TSMsgParam)self, 0);
            return true;
        }
        self->m_nEnterWorldRetries = 0;
        EnterWorldNow();
        return true;
    }
    return ((bool(__thiscall *)(EyeBtn *, unsigned long, TSMsgParam))s_pfnBtnTSOnCommand)(self, nCommand, nParam);
}
bool __fastcall B_TSPaint(EyeBtn *self, void *, bool bParentPainted)
{
    cTSWinBtn *pUndo = self->m_pArch->m_pUndoBtn, *pRedo = self->m_pArch->m_pRedoBtn;
    if (pUndo && pRedo && self->Btn()->GetFlag(kWinFlag_Visible) && pUndo->m_rcArea.top != kUndoY)
    {
        long nX = pUndo->m_rcArea.left;
        pUndo->TSWinMoveTo(nX, kUndoY);
        pRedo->TSWinMoveTo(pRedo->m_rcArea.left, kRedoY);
        self->Btn()->TSWinMoveTo(nX, kEyedropperY);
    }
    self->Btn()->SetState(Eyedropper_IsActive());
    return ((bool(__thiscall *)(EyeBtn *, bool))s_pfnBtnTSPaint)(self, bParentPainted);
}

const char *Eyedropper_ToolTip()
{
    switch (Settings() ? Settings()->m_nCurrentLanguage : (signed char)LANG8_USEnglish)
    {
    case LANG8_French:              return "Pipette (E)";
    case LANG8_German:              return "Pipette (E)";
    case LANG8_Italian:             return "Contagocce (E)";
    case LANG8_Spanish:             return "Cuentagotas (E)";
    case LANG8_Dutch:               return "Pipet (E)";
    case LANG8_Danish:              return "Pipette (E)";
    case LANG8_Swedish:             return "Pipett (E)";
    case LANG8_Norwegian:           return "Pipette (E)";
    case LANG8_Finnish:             return "Pipetti (E)";
    case LANG8_Hebrew:              return "\xE8\xF4\xE8\xF4\xFA (E)";
    case LANG8_Russian:             return "\xCF\xE8\xEF\xE5\xF2\xEA\xE0 (E)";
    case LANG8_Portuguese:          return "Conta-gotas (E)";
    case LANG8_Japanese:            return "\x83X\x83|\x83" "C\x83g (E)";
    case LANG8_Polish:              return "Pipeta (E)";
    case LANG8_SimplifiedChinese:   return "\xCE\xFC\xB9\xDC (E)";
    case LANG8_TraditionalChinese:  return "\xBAw\xBA\xDE (E)";
    case LANG8_Thai:                return "\xCB\xC5\xCD\xB4\xCB\xC2\xB4 (E)";
    case LANG8_Korean:              return "\xBD\xBA\xC6\xF7\xC0\xCC\xC6\xAE (E)";
    default:                        return "Eyedropper (E)";
    }
}

EyeBtn *EnsureButton(cWinArch *pArch)
{
    if (s_pButton && s_pButton->m_pArch == pArch)
        return s_pButton;
    EyeBtn *pBtn = (EyeBtn *)calloc(1, sizeof(EyeBtn));
    if (!pBtn)
        return 0;
    ThisCall<void *>(A::cTSWinBtn__cTSWinBtn, pBtn);
    *(void **)pBtn = &s_BtnVtbl[1];
    pBtn->m_pArch = pArch;
    pBtn->m_nEnterWorldRetries = 0;
    s_pButton = pBtn;
    cTSWinBtn *pWin = pBtn->Btn();
    bool bHaveImage = pWin->SetImageById(kEyedropperBtnImage, 4, 1);
    pWin->m_nCommandTrigger = kBtnTrigger_OnPress;
    pWin->m_nMode = 1;
    pWin->SetToolTipsText(cTSString(Eyedropper_ToolTip()));
    pArch->ChildAdd(pWin);
    pWin->m_pMsgTarget = pWin;
    if (pArch->m_pUndoBtn)
        pWin->TSWinMoveTo(pArch->m_pUndoBtn->m_rcArea.left, kEyedropperY);
    if (!bHaveImage)
        pWin->HideWindow();
    else
        pArch->InvalidateSelf();
    return pBtn;
}

void AfterPick()
{
    if (EyeBtn *pBtn = EnsureButton(SimsApp()->m_pCPanel->m_pArch))
        WinMgr()->PostMessageToTarget(pBtn->Btn(), kTSMsg_Command, kEyedropperMsg_EnterWorld, (TSMsgParam)pBtn, 0);
}

}

static cTool *Eyedropper_GetTool()
{
    if (!s_pTool)
    {
        static EyeTool s_tool;
        ThisCall<void *>(A::cTool__cTool, &s_tool);
        *(void **)&s_tool = &s_ToolVtbl[1];
        s_tool.m_bArmed = false;
        s_tool.m_pHoverProduct = 0;
        s_pTool = &s_tool;
    }
    return s_pTool->Tool();
}

static bool Eyedropper_IsActive()
{
    CPState *pState = SimsApp() ? SimsApp()->m_pCPState : 0;
    return s_pTool && pState && pState->m_pCurrentTool == s_pTool->Tool();
}

static void Eyedropper_Activate(CPState *pState)
{
    int nMode = pState->GetMode();
    if ((nMode != CPState::kBuild && nMode != CPState::kBuy) || pState->IsBuyAndBuildDisabled())
        return;
    pState->SetCurrentTool(Eyedropper_GetTool(), 0);
    pState->m_dwFlags |= CPState::kDirtyTool;
}

static void Eyedropper_Cancel(CPState *pState)
{
    if (!Eyedropper_IsActive())
        return;
    if (pState->GetMode() == CPState::kBuild)
        pState->ArchSetCurrentTool(pState->ArchGetCurrentTool());
    else
        pState->ShopSetState(pState->m_nShoppingState);
    pState->m_dwFlags |= CPState::kDirtyTool;
}

static bool Eyedropper_Toggle(CPState *pState)
{
    if (!pState)
        return false;
    int nMode = pState->GetMode();
    if (nMode != CPState::kBuild && nMode != CPState::kBuy)
        return false;
    if (Eyedropper_IsActive())
        Eyedropper_Cancel(pState);
    else
        Eyedropper_Activate(pState);
    return true;
}

ORIG(UpdateViewFromCPState, void(__thiscall *)(cWinArch *, CPState *));
static void __fastcall W_UpdateViewFromCPState(cWinArch *pArch, void *, CPState *pState)
{
    o_UpdateViewFromCPState(pArch, pState);
    if (s_pButton && s_pButton->m_pArch == pArch && (pState->m_dwFlags & CPState::kDirtyTool))
        s_pButton->Btn()->SetState(Eyedropper_IsActive());
    EnsureButton(pArch);
}

ORIG(TSOnCharacter, bool(__thiscall *)(cDDDSimsView *, char));
static bool __fastcall W_TSOnCharacter(cDDDSimsView *pView, void *, char ch)
{
    if ((ch == 'e' || ch == 'E') && Eyedropper_Toggle(pView->m_pCPState))
        return true;
    return o_TSOnCharacter(pView, ch);
}

struct Hook
{
    const char *pszName;
    unsigned va;
    void *pFn;
    void **ppOrig;
};
#define WRAP(site, fn, orig) { #site, A::site, (void *)&fn, (void **)&orig }
static const Hook s_Hooks[] = {
    WRAP(cWinArch__UpdateViewFromCPState,   W_UpdateViewFromCPState, o_UpdateViewFromCPState),
    WRAP(cDDDSimsView__TSOnCharacter,       W_TSOnCharacter, o_TSOnCharacter),
};

struct VSlot
{
    const char *pszName;
    unsigned vtbl, slot, va;
};
static const VSlot s_VSlots[] = {
#define GA_FN(n, va)
#define GA_DATA(n, va)
#define GA_CRT(n, va, site)
#define GA_RTTI(n, va, s)
#define GA_VTBL(n, va, slots)
#define GA_VSLOT(n, vt, slot, va) { #n, A::vt, slot, va },
#include GA_ADDR_FILE
#undef GA_FN
#undef GA_DATA
#undef GA_CRT
#undef GA_RTTI
#undef GA_VTBL
#undef GA_VSLOT
};

static const unsigned s_NoSigCheck[] = {
    A::CPState__SetMode,                // wrapped by visit_lots: its bytes depend on load order
};

static int CheckGame()
{
    int nBad = 0;
    for (const GenSig &s : g_GenSigs)
    {
        bool bSkip = false;
        for (unsigned va : s_NoSigCheck)
            bSkip |= va == s.va;
        if (bSkip)
            continue;
        char why[160] = "";
        if (!s_api->SigCheck(&s.sig, why, sizeof why))
        {
            s_api->Log("[eyedropper] function @ 0x%06x: SIGNATURE MISMATCH (%s)", s.va, why);
            ++nBad;
        }
    }
    for (const VSlot &v : s_VSlots)
        if (((unsigned *)Va(v.vtbl))[v.slot] != (unsigned)Va(v.va))
        {
            s_api->Log("[eyedropper] %s: vtable 0x%06x slot %u is not 0x%06x", v.pszName, v.vtbl, v.slot, v.va);
            ++nBad;
        }
    const unsigned char *pSite = (const unsigned char *)Va(A::RTDynamicCast_Site);
    if (pSite[0] != 0xE8 || pSite + 5 + *(const int *)(pSite + 1) != Va(A::RTDynamicCast))
    {
        s_api->Log("[eyedropper] __RTDynamicCast: the call at 0x%06x does not reach 0x%06x", A::RTDynamicCast_Site, A::RTDynamicCast);
        ++nBad;
    }
    static const struct { unsigned td; const char *psz; } s_Rtti[] = {
        { A::TD_cXObject, ".?AVcXObject@@" }, { A::TD_cXPerson, ".?AVcXPerson@@" }, { A::TD_cXMTObject, ".?AVcXMTObject@@" },
    };
    for (auto &r : s_Rtti)
        if (strcmp((const char *)Va(r.td) + 8, r.psz))
        {
            s_api->Log("[eyedropper] RTTI 0x%06x is not %s", r.td, r.psz);
            ++nBad;
        }
    return nBad;
}

static void CopyVtbl(void **pDst, unsigned va, unsigned nSlots)
{
    memcpy(pDst, (void **)Va(va) - 1, (nSlots + 1) * sizeof(void *));
}

static void BuildVtables()
{
    CopyVtbl(s_ToolVtbl, A::cTool__vftable, A::cTool__vftable_Slots);
    void **t = s_ToolVtbl + 1;
    t[A::Slot_Tool_Dtor] = (void *)&T_Dtor;
    t[A::Slot_Tool_Select] = (void *)&T_Select;
    t[A::Slot_Tool_Deselect] = (void *)&T_Deselect;
    t[A::Slot_Tool_Float] = (void *)&T_Float;
    t[A::Slot_Tool_Click] = (void *)&T_Click;
    t[A::Slot_Tool_Release] = (void *)&T_Release;
    t[A::Slot_Tool_OnKeyDown] = (void *)&T_OnKeyDown;
    t[A::Slot_Tool_OnEnterWorld] = (void *)&T_OnEnterWorld;
    t[A::Slot_Tool_OnExitWorld] = (void *)&T_OnExitWorld;
    t[A::Slot_Tool_Cancel] = (void *)&T_Cancel;
    t[A::Slot_Tool_ControlsOwnToolTips] = (void *)&T_ControlsOwnToolTips;
    CopyVtbl(s_BtnVtbl, A::cTSWinBtn__vftable, A::cTSWinBtn__vftable_Slots);
    void **b = s_BtnVtbl + 1;
    s_pfnBtnTSOnCommand = b[A::Slot_Win_TSOnCommand];
    s_pfnBtnTSPaint = b[A::Slot_Win_TSPaint];
    b[A::Slot_Win_Dtor] = (void *)&B_Dtor;
    b[A::Slot_Win_TSOnCommand] = (void *)&B_TSOnCommand;
    b[A::Slot_Win_TSPaint] = (void *)&B_TSPaint;
}

static void LogDataFiles()
{
    static const char *const s_files[] = { "UIGraphics\\Res_Eyedropper.RT", "UIGraphics\\Res_Eyedropper.h",
                                           "UIGraphics\\Eyedropper\\EyedropperBtn.bmp", "UIGraphics\\Eyedropper\\Eyedropper.cur" };
    for (const char *pszFile : s_files)
    {
        char szPath[MAX_PATH];
        _snprintf_s(szPath, sizeof szPath, _TRUNCATE, "%s\\%s", s_api->exeDir, pszFile);
        if (GetFileAttributesA(szPath) == INVALID_FILE_ATTRIBUTES)
            s_api->Log("[eyedropper] %s missing: no build panel button / the pointing-hand cursor (the E key still works)", pszFile);
    }
}

extern "C" __declspec(dllexport) int __cdecl SimsMod_Init(const SimsLoaderApi *api)
{
    if (api->version != SIMSMOD_API_VERSION)
        return 1;
    s_api = api;
    ga::g_exeBase = api->exeBase;
    if (int nBad = CheckGame())
    {
        api->Log("[eyedropper] %d checks do not match this Sims.exe: nothing patched, the mod is off", nBad);
        return 2;
    }
    BuildVtables();
    LogDataFiles();
    int nOk = 0, nFailed = 0;
    for (const Hook &h : s_Hooks)
    {
        const GenSig *pSig = 0;
        for (const GenSig &s : g_GenSigs)
            if (s.va == h.va)
                pSig = &s;
        if (pSig && api->Detour(h.pszName, &pSig->sig, h.pFn, h.ppOrig) && *h.ppOrig)
            ++nOk;
        else
            ++nFailed;
    }
    api->Log("[eyedropper] %d hooks installed, %d failed", nOk, nFailed);
    return nFailed ? 3 : 0;
}

BOOL WINAPI DllMain(HINSTANCE hinst, DWORD reason, LPVOID)
{
    if (reason == DLL_PROCESS_ATTACH)
        DisableThreadLibraryCalls(hinst);
    return TRUE;
}
