// game_api.h - the Sims.exe classes, fields (explicit offsets) and functions (address thunks) the mod uses.
#pragma once
#include <stddef.h>

#pragma warning(push)
#pragma warning(disable : 4201)

#define GA_F(off, type, name) struct { char _o_##name[off]; type name; };
#define GA_F0(type, name) struct { type name; };
#define GA_A(off, type, name, n) struct { char _o_##name[off]; type name[n]; };
#define GA_SIZE(n) char _ga_size[n];

namespace ga
{
    namespace A
    {
        enum : unsigned
        {
#define GA_FN(n, va) n = va,
#define GA_DATA(n, va) n = va,
#define GA_CRT(n, va, site) n = va,
#define GA_RTTI(n, va, s) n = va,
#define GA_VTBL(n, va, slots) n = va,
#define GA_VSLOT(n, vt, slot, va) n = slot,
#include "game_addr.h"
#undef GA_FN
#undef GA_DATA
#undef GA_CRT
#undef GA_RTTI
#undef GA_VTBL
#undef GA_VSLOT
        };
    }

    extern unsigned g_exeBase;
    inline void *Va(unsigned va) { return (void *)(va - 0x400000u + g_exeBase); }
    template <class T> inline T &Global(unsigned va) { return *(T *)Va(va); }

    template <class R, class... T> inline R Call(unsigned va, T... a) { return ((R(__cdecl *)(T...))Va(va))(a...); }
    template <class R, class... T> inline R ThisCall(unsigned va, const void *self, T... a)
    {
        return ((R(__thiscall *)(const void *, T...))Va(va))(self, a...);
    }
    template <class R, class... T> inline R VCall(const void *self, int slot, T... a)
    {
        return ((R(__thiscall *)(const void *, T...))(*(void ***)self)[slot])(self, a...);
    }

    template <class T> struct Vec
    {
        T *_Myfirst, *_Mylast, *_Myend;
        size_t size() const { return (size_t)(_Mylast - _Myfirst); }
        T &operator[](size_t i) const { return _Myfirst[i]; }
        T *begin() const { return _Myfirst; }
        T *end() const { return _Mylast; }
    };

    struct ScreenPt16 { short nY, nX; };
    struct FTilePt { int nPosY16, nPosX16; };
    struct CTilePt
    {
        signed char nX, nY, nLevel;
        CTilePt() : nX(0), nY(0), nLevel(0) {}
        CTilePt(int x, int y, int level) : nX((signed char)x), nY((signed char)y), nLevel((signed char)level) {}
        unsigned Packed() const { return (unsigned char)nX | (unsigned char)nY << 8 | (unsigned char)nLevel << 16; }
    };
    struct TileRotateEntry { signed char x, y; };
    struct Rect { long left, top, right, bottom; };

    struct EventRecord
    {
        union { GA_F(0x0C, ScreenPt16, m_ptWhere) GA_SIZE(20) };
    };

    class cTSString
    {
    public:
        union { GA_F0(char, m_szBuf[16]) GA_F0(char *, m_pszPtr) GA_F(0x10, unsigned, m_nSize) GA_F(0x14, unsigned, m_nRes) GA_SIZE(24) };
        explicit cTSString(const char *psz) { ThisCall<void>(A::cTSString__cTSString, this, psz); }
        ~cTSString() { ThisCall<void>(A::cTSString___cTSString, this); }
        cTSString(const cTSString &) = delete;
        cTSString &operator=(const cTSString &) = delete;
        const char *c_str() const { return m_nRes > 15 ? m_pszPtr : m_szBuf; }
    };

    struct ImageRes;
    class cTSBufferRef
    {
    public:
        ImageRes *m_p;
        cTSBufferRef() : m_p(0) {}
        ~cTSBufferRef() { ThisCall<void>(A::cTSBufferRef___cTSBufferRef, this); }
        cTSBufferRef(cTSBufferRef &&o) : m_p(o.m_p) { o.m_p = 0; }
        cTSBufferRef(const cTSBufferRef &) = delete;
        cTSBufferRef &operator=(const cTSBufferRef &) = delete;
        static cTSBufferRef LoadById(long nResId)
        {
            cTSBufferRef h;
            Call<void *>(A::Image_LoadById, &h, nResId, Va(A::g_ImageResDescriptor));
            return h;
        }
    };

    enum eWallSegment
    {
        kWallSeg_West = 0x01,
        kWallSeg_North = 0x02,
        kWallSeg_DiagNE_SW = 0x10,
        kWallSeg_DiagNW_SE = 0x20,
        kWallSeg_AnyDiagonal = 0x30,
    };

    struct TileWalls
    {
        unsigned int m_nWallMask;
        int m_nEdgeStyles;
        unsigned short m_nPattern[6];
        unsigned short m_nFacePattern[8];
        int m_nRotation;
        int m_nNeighborDiag;
        int GetEdgeValue(int eSegment, int eDiagSide) const
        {
            if (eSegment == 16 && !eDiagSide)
                eDiagSide = 4;
            else if (eSegment == 32 && !eDiagSide)
                eDiagSide = 1;
            int nIndex = 0;
            while (!(eSegment & 1))
            {
                eSegment >>= 1;
                nIndex++;
            }
            if (eDiagSide == 1 || eDiagSide == 4)
                nIndex++;
            else if (eDiagSide == 3)
                nIndex += 2;
            return m_nFacePattern[nIndex];
        }
        int GetDiagonalFloorPattern(int nSide) { return ThisCall<int>(A::TileWalls__GetDiagonalFloorPattern, this, nSide); }
        static int DirToWallSeg(int eDir) { return Call<int>(A::TileWalls__DirToWallSeg, eDir); }
    };
    static_assert(sizeof(TileWalls) == 44, "TileWalls");
    inline bool IsFenceStyle(int n) { return n == 2 || n == 12 || n == 13 || n == 14; }

    struct c2DArrayU16
    {
        union { GA_F(0x0C, unsigned short **, m_pRows) };
    };

    class cRotatableWorld
    {
    public:
        union
        {
            GA_A(0x20, c2DArrayU16 *, m_pFloorsByLevel, 3)
            GA_F(0x70, int, m_nRotMinX) GA_F(0x74, int, m_nRotMaxX) GA_F(0x78, int, m_nRotMinY) GA_F(0x7C, int, m_nRotMaxY)
            GA_F(0x84, int, m_nRotation) GA_SIZE(0x88)
        };
        bool IsInLot(const CTilePt &pt) const
        {
            return pt.nX >= m_nRotMinX + 1 && pt.nX <= m_nRotMaxX - 1 && pt.nY >= m_nRotMinY + 1 && pt.nY <= m_nRotMaxY - 1;
        }
        int GetToViewRotation() const { return m_nRotation; }
        CTilePt Rotate(const CTilePt &pt) const
        {
            CTilePt r = pt;
            int nRot = -m_nRotation & 3;
            if (nRot)
            {
                const TileRotateEntry &e = (&Global<TileRotateEntry>(A::g_TileRotateLUT))[((nRot - 1) * 64 + pt.nX) * 64 + pt.nY];
                r.nX = e.x;
                r.nY = e.y;
            }
            return r;
        }
        TileWalls GetWall(const CTilePt &pt)
        {
            TileWalls w;
            ThisCall<TileWalls *>(A::cRotatableWorld__GetWall, this, &w, pt.Packed());
            return w;
        }
        void RotateCoords(int *pX, int *pY, int nRotation, int nSize)
        {
            ThisCall<void>(A::cRotatableWorld__RotateCoords, this, pX, pY, nRotation, nSize);
        }
    };

    struct WallSegment
    {
        char m_nFlags;
        unsigned char m_nStyle;
        unsigned short m_anPattern[2];
    };
    class Wall
    {
    public:
        union
        {
            GA_F(0x04, CTilePt, m_TileA) GA_F(0x0A, signed char, m_nStepX) GA_F(0x0B, signed char, m_nStepY)
            GA_F(0x10, int, m_nDir) GA_F(0x30, Vec<WallSegment>, m_SegmentsA) GA_F(0x48, char, m_bIconic)
        };
        static CTilePt VertexToTile(const CTilePt &vertex, int nDir)
        {
            CTilePt t;
            Call<CTilePt *>(A::Wall__VertexToTile, &t, &vertex, nDir);
            return t;
        }
    };
    class WallManager
    {
    public:
        union { GA_A(0x10, Vec<Wall *> *, m_pWallLists, 2) };
        static WallManager *Get() { return Call<WallManager *>(A::WallManager__GetWallManager); }
    };

    struct GViewer
    {
        static int GetScale() { return Call<int>(A::GViewer_GetScale); }
        static int GetTileHeight() { return Call<int>(A::GViewer_GetTileHeight); }
        static FTilePt PointToTile(const ScreenPt16 &pt, bool bSkipPick)
        {
            FTilePt r;
            Call<FTilePt *>(A::GViewer_PointToTile, &r, &pt, bSkipPick);
            return r;
        }
        static ScreenPt16 TileToPoint(const FTilePt *p, int nLevel) { return Call<ScreenPt16>(A::GViewer_TileToPoint, p, nLevel); }
        static ScreenPt16 SubTileToPoint(const FTilePt *p, int nLevel) { return Call<ScreenPt16>(A::GViewer_SubTileToPoint, p, nLevel); }
    };
    static_assert(sizeof(ScreenPt16) == 4 && sizeof(FTilePt) == 8 && sizeof(CTilePt) == 3, "point sizes");

    class ObjSelector
    {
    public:
        union { GA_F(0x8C, ObjSelector *, m_pMasterSelector) };
    };
    class cXObject
    {
    public:
        union { GA_F(0x0F4, int, m_nPosY16) GA_F(0x0F8, int, m_nPosX16) GA_F(0x120, ObjSelector *, m_pSelector) };
    };
    class cXMTObject
    {
    public:
        union { GA_F(0x120, ObjSelector *, m_pSelector) GA_F(0x194, cXMTObject *, m_pMaster) };
    };

    inline void *DynCast(cXObject *p, unsigned tdTo)
    {
        return p ? Call<void *>(A::RTDynamicCast, (void *)p, 0L, Va(A::TD_cXObject), Va(tdTo), 0) : 0;
    }

    struct ObjectHandle
    {
        cXObject *Resolve() const { return ThisCall<cXObject *>(A::ObjectHandle__Resolve, this); }
    };
    struct PickRecord
    {
        void *m_pMeshItem;
        void *m_pDrawItem;
    };
    struct PickHitResult
    {
        float m_fDepth;
        PickRecord *m_pRecord;
    };
    struct RenderContext
    {
        bool HitTestScreenPoint(int x, int y, PickHitResult *pHit)
        {
            return ThisCall<bool>(A::RenderContext__HitTestScreenPoint, this, x, y, pHit);
        }
    };
    class HouseViewer
    {
    public:
        union
        {
            GA_F(0x028, Rect, m_ViewRect) GA_A(0x0D4 + 0x6C, int, m_anLevelHeightPx, 3)
            GA_F(0x5C4, RenderContext, m_RenderContext)
        };
    };

    class cSoundPlayer
    {
    public:
        bool PlayBySource(const char *pszName, short nSource) { return ThisCall<bool>(A::cSoundPlayer__PlayBySource, this, pszName, nSource); }
    };

    class cSimsSettings
    {
    public:
        union { GA_F(0x04, HouseViewer *, m_pHouseViewer) GA_F(0x10, cSoundPlayer *, m_pSoundSys) GA_F(0x68, signed char, m_nCurrentLanguage) };
    };

    enum eProductKind { kProductEntry_Object = 1, kProductEntry_Floor = 2, kProductEntry_Wall = 3, kProductEntry_WallPattern = 4 };
    enum eCatalogGroup
    {
        kCatalogGroup_Room, kCatalogGroup_Function, kCatalogGroup_Downtown, kCatalogGroup_Vacation, kCatalogGroup_Community,
        kCatalogGroup_Studiotown, kCatalogGroup_Magictown,
    };

    class Product
    {
    public:
        union { GA_F(0x3C, int, m_nKind) GA_F(0x54, int, m_nWallId) GA_F(0x58, int, m_nWallStyle) GA_F(0x5C, int, m_nFloorId) };
#define GA_PB(name) bool name() { return ThisCall<bool>(A::Product__##name, this); }
#define GA_PI(name) bool name(int n) { return ThisCall<bool>(A::Product__##name, this, n); }
        GA_PI(TestArchTool)
        GA_PB(HasRoomSort) GA_PB(HasDowntownSort) GA_PB(HasVacationSort) GA_PB(HasCommunitySort) GA_PB(HasStudiotownSort)
        GA_PB(HasMagictownSort)
        GA_PI(TestInRoom) GA_PI(TestInDowntownRoom) GA_PI(TestInVacationRoom) GA_PI(TestInCommunityRoom)
        GA_PI(TestInStudiotownRoom) GA_PI(TestInMagictownRoom) GA_PI(TestHasFunction)
#undef GA_PB
#undef GA_PI
        bool TestHasFunction(int nMask, int nSubMask) { return ThisCall<bool>(A::Product__TestHasFunction2, this, nMask, nSubMask); }
        const cTSString &GetProductName() { return *ThisCall<cTSString *>(A::Product__GetProductName, this); }
    };

    class Catalog
    {
    public:
        union { GA_F(0x04, Vec<Product *>, m_Products) };
        Product **begin() const { return m_Products.begin(); }
        Product **end() const { return m_Products.end(); }
        Product *FindProduct(ObjSelector *p) { return ThisCall<Product *>(A::Catalog__FindProduct, this, p); }
    };

    struct cWinCatalogStatic
    {
        static int GetRoomMask(int i) { return Call<int>(A::cWinCatalog__GetRoomMask, i); }
        static int GetFunctionMask(int i) { return Call<int>(A::cWinCatalog__GetFunctionMask, i); }
        static int GetFunctionSubMask(int i) { return Call<int>(A::cWinCatalog__GetFunctionSubMask, i); }
        static int GetDowntownMask(int i) { return Call<int>(A::cWinCatalog__GetDowntownMask, i); }
        static int GetVacationMask(int i) { return Call<int>(A::cWinCatalog__GetVacationMask, i); }
        static int GetCommunityMask(int i) { return Call<int>(A::cWinCatalog__GetCommunityMask, i); }
        static int GetStudiotownMask(int i) { return Call<int>(A::cWinCatalog__GetStudiotownMask, i); }
        static int GetMagictownMask(int i) { return Call<int>(A::cWinCatalog__GetMagictownMask, i); }
    };

    class cTool
    {
    public:
        enum { kSize = 44 };
        static int GetCurrentLevel() { return Call<int>(A::cTool__GetCurrentLevel); }
        static void SetToolTip(const char *psz) { Call<void>(A::cTool__SetToolTip, psz); }
        static void SetCursor(int nCursor) { Call<void>(A::cTool__SetCursor, nCursor); }
        void DoDeniedSound() { ThisCall<void>(A::cTool__DoDeniedSound, this); }
    };
    enum { kCursor_Finger = 4 };

    class CPState
    {
    public:
        enum Mode { kBuild = 0, kBuy = 1, kLive = 2 };
        enum { kDirtyTool = 0x10, kDirtyArchSubtool = 0x20, kDirtyCatalog = 0x400 };
        union
        {
            GA_F(0x0C, int, m_dwFlags) GA_F(0x64, cTool *, m_pCurrentTool) GA_A(0x84, Product *, m_pModeProducts, 12)
            GA_F(0xB4, int, m_nShoppingState) GA_F(0xB8, int, m_nCatalogGroup) GA_A(0xBC, int, m_CatalogSel, 15)
            GA_F(0xF8, int, m_nCatalogPage)
        };
        Catalog *GetCatalog() { return ThisCall<Catalog *>(A::CPState__GetCatalog, this); }
        int GetMode() { return ThisCall<int>(A::CPState__GetMode, this); }
        int GetCPMode() { return ThisCall<int>(A::CPState__GetCPMode, this); }
        void SetCPMode(int nMode, bool bQuiet) { ThisCall<void>(A::CPState__SetCPMode, this, nMode, bQuiet); }
        void SetMode(int nMode, bool bForce) { ThisCall<void>(A::CPState__SetMode, this, nMode, bForce); }
        int ArchGetCurrentTool() { return ThisCall<int>(A::CPState__ArchGetCurrentTool, this); }
        void ArchSetCurrentTool(int n) { ThisCall<void>(A::CPState__ArchSetCurrentTool, this, n); }
        void ResetPreviousPages() { ThisCall<void>(A::CPState__ResetPreviousPages, this); }
        void ShopSetSort(int nGroup) { ThisCall<void>(A::CPState__ShopSetSort, this, nGroup); }
        void ShopSetState(int n) { ThisCall<void>(A::CPState__ShopSetState, this, n); }
        void UpdateWorldFromCPState(unsigned nMask) { ThisCall<void>(A::CPState__UpdateWorldFromCPState, this, nMask); }
        void MouseEnteredWorld() { ThisCall<void>(A::CPState__MouseEnteredWorld, this); }
        bool IsBuyAndBuildDisabled() { return ThisCall<bool>(A::CPState__IsBuyAndBuildDisabled, this); }
        void SetCurrentTool(cTool *p, int nSubId) { ThisCall<void>(A::CPState__SetCurrentTool, this, p, nSubId); }
    };

    typedef unsigned int TSMsgParam;
    enum { kTSMsg_Command = 3, kWinMsg_ButtonClick = 1, kWinFlag_Visible = 0x1, kBtnTrigger_OnPress = 1 };

    class cTSWin
    {
    public:
        union { GA_F(0x4C, cTSWin *, m_pMsgTarget) GA_F(0xD4, Rect, m_rcArea) GA_F(0xF4, unsigned long, m_nFlags) GA_SIZE(0x128) };
        bool GetFlag(unsigned long n) const { return (m_nFlags & n) != 0; }
        void SetToolTipsText(const cTSString &s) { ThisCall<void>(A::cTSWin__SetToolTipsText, this, &s); }
        void TSWinMoveTo(long x, long y) { ThisCall<void>(A::cTSWin__TSWinMoveTo, this, x, y); }
        bool ChildAdd(cTSWin *p) { return VCall<bool>(this, A::Slot_Win_ChildAdd, p); }
        void HideWindow() { VCall<void>(this, A::Slot_Win_HideWindow); }
        void InvalidateSelf() { VCall<void>(this, A::Slot_Win_InvalidateSelf); }
    };

    class cTSWinBtn : public cTSWin
    {
    public:
        enum { kSize = 0x1E0 };
        union { GA_F(0x144 - 0x128, long, m_nCommandTrigger) GA_F(0x148 - 0x128, long, m_nMode) GA_SIZE(kSize - 0x128) };
        void SetState(long n) { ThisCall<void>(A::cTSWinBtn__SetState, this, n); }
        // the handle is passed by value and the callee releases it, so it is moved in
        bool SetImage(cTSBufferRef &h, long nCols, long nRows)
        {
            ImageRes *p = h.m_p;
            h.m_p = 0;
            return VCall<bool>(this, A::Slot_Win_SetImage, p, nCols, nRows);
        }
    };
    static_assert(sizeof(cTSWin) == 0x128 && sizeof(cTSWinBtn) == 0x1E0, "window sizes");

    class cWinArch : public cTSWin
    {
    public:
        union { GA_F(0x12C - 0x128, int, m_nMode) GA_F(0x138 - 0x128, cTSWinBtn *, m_pUndoBtn) GA_F(0x13C - 0x128, cTSWinBtn *, m_pRedoBtn) };
        void UpdateViewFromCPState(CPState *p) { ThisCall<void>(A::cWinArch__UpdateViewFromCPState, this, p); }
    };
    class cWinCatalog : public cTSWin
    {
    public:
        union { GA_F(0x4B8 - 0x128, int, m_nItemsPerPage) };
        void UpdateViewFromCPState(CPState *p) { ThisCall<void>(A::cWinCatalog__UpdateViewFromCPState, this, p); }
        void SetProduct(Product *p) { ThisCall<void>(A::cWinCatalog__SetProduct, this, p); }
    };
    class cWinCPanel
    {
    public:
        union { GA_F(0x134, cWinCatalog *, m_pCatalog) GA_F(0x13C, cWinArch *, m_pArch) };
    };
    class cDDDSimsView
    {
    public:
        union { GA_F(0x164, CPState *, m_pCPState) };
    };
    class cSimsApp
    {
    public:
        union { GA_F(0x8C, cDDDSimsView *, m_pLotView) GA_F(0x90, cWinCPanel *, m_pCPanel) GA_F(0xC4, CPState *, m_pCPState) };
    };

    class cTSCursor;
    class cTSCursorManager
    {
    public:
        cTSCursor *GetCursor(unsigned long n) { return ThisCall<cTSCursor *>(A::cTSCursorManager__GetCursor, this, n); }
        bool AddCursor(const cTSString &path, unsigned long n) { return ThisCall<bool>(A::cTSCursorManager__AddCursor, this, &path, n); }
    };
    class cTSWinMgrW95
    {
    public:
        union { GA_F(0x178, cTSCursorManager *, m_pCursorMgr) };
        void GetCursorScreenPosition(long &x, long &y) { ThisCall<void>(A::cTSWinMgrW95__GetCursorScreenPosition, this, &x, &y); }
        cTSWin *GetWindowFromPoint(long x, long y) { return ThisCall<cTSWin *>(A::cTSWinMgrW95__GetWindowFromPoint, this, x, y); }
        void PostMessageToTarget(cTSWin *p, unsigned long nClass, TSMsgParam a, TSMsgParam b, TSMsgParam c)
        {
            ThisCall<void>(A::cTSWinMgrW95__PostMessageToTarget, this, p, nClass, a, b, c);
        }
    };

    inline cSimsSettings *Settings() { return Global<cSimsSettings *>(A::g_pAppSettings); }
    inline cSimsApp *SimsApp() { return Global<cSimsApp *>(A::g_pSimsApp); }
    inline cTSWinMgrW95 *WinMgr() { return Global<cTSWinMgrW95 *>(A::g_pWinMgr); }
    inline cRotatableWorld *Lot() { return Global<cRotatableWorld *>(A::g_pLot); }

    enum eSimsLanguage8
    {
        LANG8_USEnglish = 1, LANG8_UKEnglish, LANG8_French, LANG8_German, LANG8_Italian, LANG8_Spanish, LANG8_Dutch,
        LANG8_Danish, LANG8_Swedish, LANG8_Norwegian, LANG8_Finnish, LANG8_Hebrew, LANG8_Russian, LANG8_Portuguese,
        LANG8_Japanese, LANG8_Polish, LANG8_SimplifiedChinese, LANG8_TraditionalChinese, LANG8_Thai, LANG8_Korean,
    };
}

#pragma warning(pop)

#ifndef GA_NO_USING
using namespace ga;
#endif
