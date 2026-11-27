// seasons.cpp - mods\seasons_steam.dll (seasons_cc.dll for the 2003 Complete Collection): the engine side of
// "Seasons", the unofficial expansion (seasons, weather, the season icon, raincoats and umbrellas, dressing at the door,
// snow locations, the ExpansionPack8 objects folder and the extra careers), as a patch mod.
//
// Every original function keeps running as the exe's own code. The mod changes it in one of three ways:
//   wrapper  - the function's entry is detoured to a W_* function, which runs the feature's part before and / or after
//              the original (called through the loader's trampoline, o_*), or returns early;
//   call     - one `call X` inside an original function is redirected to a C_* function with X's calling convention,
//              which does the feature's part and calls X;
//   stub     - a few bytes inside an original function become a jmp to a naked S_* asm stub (// ASM: says why) that
//              resumes at a GA_LABEL of the same function.
// A wrapper's code after the original does not run when the original throws. Each site has a 16-byte signature
// (gen_sigs.h, gen_sigs_ea.h for the EA app build, made from the game build this mod is for);
// all of them are checked on the running game first, and nothing is patched unless every one matches.
#include <windows.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <new>
#include "simsmod.h"
#include "Seasons.h"
#include "SeasonsHooks.h"
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

void Seasons_Log(const char *fmt, ...)
{
    if (!s_api)
        return;
    char sz[512];
    va_list ap;
    va_start(ap, fmt);
    _vsnprintf_s(sz, sizeof sz, _TRUNCATE, fmt, ap);
    va_end(ap);
    s_api->Log("[seasons] %s", sz);
}

#if !GAME_CC
// ---- one heap with the exe ---------------------------------------------------------------------------------------
// operator new / delete are the exe's (game_addr.h OperatorNew / OperatorDelete), so a block this DLL allocates can be freed by
// the game and the other way round: game vectors grown here, careers created here and deleted by LoadCareers. Looked up
// from the main module on first use (static constructors run before SimsMod_Init).
static void *ExeFn(unsigned va)
{
    static unsigned s_nBase;
    if (!s_nBase)
        s_nBase = (unsigned)GetModuleHandleA(0);
    return (void *)(va - 0x400000u + s_nBase);
}
void *operator new(size_t n) { return ((void *(__cdecl *)(size_t))ExeFn(A::OperatorNew))(n); }
void *operator new[](size_t n) { return ((void *(__cdecl *)(size_t))ExeFn(A::OperatorNew))(n); }
void operator delete(void *p) noexcept { ((void(__cdecl *)(void *, size_t))ExeFn(A::OperatorDelete))(p, 0); }
void operator delete[](void *p) noexcept { ((void(__cdecl *)(void *, size_t))ExeFn(A::OperatorDelete))(p, 0); }
void operator delete(void *p, size_t n) noexcept { ((void(__cdecl *)(void *, size_t))ExeFn(A::OperatorDelete))(p, n); }
void operator delete[](void *p, size_t n) noexcept { ((void(__cdecl *)(void *, size_t))ExeFn(A::OperatorDelete))(p, n); }
void *operator new(size_t n, const std::nothrow_t &) noexcept
{
    try { return operator new(n); } catch (...) { return 0; }
}
#endif
// 2003: the exe's STL and allocator are not this DLL's (game_api.h), so the DLL keeps its own heap; what the game frees
// is allocated with the exe's own operator new / push_back / string functions where it is created.

// ---- the game edition ---------------------------------------------------------------------------------------------

// cSimulator::Init sets global GameEdition from the installed packs; Seasons adds its bit after Makin' Magic's (0x80),
// in every version, so objects can test for it. A lot load keeps the value.
ORIG(SimulatorInit, void(__thiscall *)(cSimulator *, ObjectModule *));
static void __fastcall W_SimulatorInit(cSimulator *s, void *, ObjectModule *pModule)
{
    o_SimulatorInit(s, pModule);
    s->m_Globals.GameEdition |= 0x100;
}

// ---- the seasonal ground -----------------------------------------------------------------------------------------

// cTerrain::SetColor: on normal-ground lots the seasonal palette state that MakeDirt's PreCompute then uses
ORIG(SetColor, void(__thiscall *)(cTerrain *));
static void __fastcall W_SetColor(cTerrain *t, void *)
{
    Seasons_TerrainSetColorPrefix(t);
    o_SetColor(t);
}

struct WideColorChunk;
class cColorSwatch
{
public:
    union
    {
        GA_A(0x20, int, m_aShadeOffsets, 4) GA_F(0x4C, int, m_nShadeCount) GA_F(0x70, WideColorChunk **, m_ppColorTableRows)
#if GAME_CC
        GA_F(0x7C, int, fNumColors)                     // 2003: no atlas, no tile mesh scratch before it
#else
        GA_A(0x7C, cTSBufferRef, m_Atlas, 5) GA_F(0x3E8, int, fNumColors) GA_F(0x438, int, m_nQualityLevel)
#endif
    };
    cColorSwatch() = delete;
};
#if !GAME_CC
struct TextureAtlasSet { cTSBufferRef m_h[5]; };
#endif

// cColorSwatch::PreCompute: with the seasonal palette on, every colour row is built again from Seasons_GetPaletteRow's
// light / dark pair (the original loop's BuildRamp call) and the atlas again from the new rows (the original's last
// statement). Off: nothing, the original result stands. 2003: the software terrain fill reads the colour rows
// directly, there is no atlas to rebuild.
ORIG(PreCompute, void(__thiscall *)(cColorSwatch *));
static void __fastcall W_PreCompute(cColorSwatch *s, void *)
{
    o_PreCompute(s);
    int nColors = s->fNumColors;
    bool bChanged = false;
    for (int iColor = 0; iColor < nColors; iColor++)
    {
        unsigned long nLight, nDark;
        if (!Seasons_GetPaletteRow(iColor, nColors, &nLight, &nDark))
            return;                                     // seasonal palette off (same answer for every row)
        Call<void>(A::BuildRamp_WideColorChunk, s->m_ppColorTableRows[iColor], s->m_nShadeCount, nLight, nDark,
                   s->m_aShadeOffsets, false);
        bChanged = true;
    }
#if !GAME_CC
    if (bChanged)
    {
        const int *pZoom = &Global<int>(A::g_SwatchParamsActive);
        TextureAtlasSet atlas;
        CallRet(&atlas, A::TextureAtlasSet_Build, (unsigned char **)s->m_ppColorTableRows, 32, 128, pZoom[0], pZoom[1],
                pZoom[2], s->m_nQualityLevel <= 1);
        for (int i = 0; i < 5; i++)
            s->m_Atlas[i] = std::move(atlas.m_h[i]);
    }
#else
    (void)bChanged;
#endif
}

// cColorSwatch::ComputeMeshPointColorIndex: the grass colour row of a mesh corner, remapped for the season
ORIG(ComputeMeshPointColorIndex, int(__thiscall *)(cColorSwatch *, signed char, signed char));
static int __fastcall W_ComputeMeshPointColorIndex(cColorSwatch *s, void *, signed char x, signed char y)
{
    int n = o_ComputeMeshPointColorIndex(s, x, y);
    return Seasons_AdjustColorIndex(x, y, n, s->fNumColors);
}

#if !GAME_CC
// The snow per terrain corner (vertex colours of the tile mesh, grass blades under snow) is the Legacy renderer's: the
// 2003 terrain is a software fill from the colour rows, where the frost of Seasons_GetPaletteRow is the snow.
struct TerrainVertex { float x, y, z; unsigned int nColor; float m_field_10; };
class TerrainTileScratch
{
public:
    union
    {
        GA_A0(float, m_CornerPos, 12) GA_A(0x30, unsigned char, m_CornerColorIdx, 4) GA_A(0x34, unsigned char, m_CornerShadeIdx, 4)
        GA_A(0x58, float, m_fCornerDetailAlpha, 4) GA_A(0x8C, TerrainVertex, m_TerrainVerts, 4)
    };
};
class RenderParam;

// TerrainTileScratch::ComputeDetailFade: the grass-blade detail of the four corners fades under snow
ORIG(ComputeDetailFade, void(__thiscall *)(TerrainTileScratch *, int, int, int, const void *));
static void __fastcall W_ComputeDetailFade(TerrainTileScratch *t, void *, int nX, int nY, int nSeed, const void *pViewProj)
{
    o_ComputeDetailFade(t, nX, nY, nSeed, pViewProj);
    Seasons_TerrainDetailAlpha(t->m_CornerPos, t->m_fCornerDetailAlpha);
}

// EmitAll's `call EmitQuad` (the inlined BuildVertexColors left the four corner colours in m_TerrainVerts[i].nColor):
// they are blended toward snow per corner first
static void __fastcall C_EmitQuad(TerrainTileScratch *t, void *, RenderParam *pParam, int nRot)
{
    unsigned int aColor[4];
    for (int i = 0; i < 4; i++)
        aColor[i] = t->m_TerrainVerts[i].nColor;
    Seasons_TerrainCornerColors(t->m_CornerPos, t->m_CornerColorIdx, t->m_CornerShadeIdx, aColor);
    for (int i = 0; i < 4; i++)
        t->m_TerrainVerts[i].nColor = aColor[i];
    ThisCall<void>(A::TerrainTileScratch__EmitQuad, t, pParam, nRot);
}
#endif

// cDDDSimsView::TSPaint: when this call draws the scene (m_bInDraw clear), the rain / snow overlay over the view after
// it (Draw is inlined into TSPaint, so the overlay also covers the edge-scroll image and the pause text). 2003: the
// view redraws only damaged rectangles, so before the scene is drawn the whole view is damaged while the overlay shows
// (Seasons_PrePaint), and the overlay is then blended into the buffer the scene was drawn into.
class cDDDSimsView
{
public:
#if GAME_CC
    union { GA_F(0x1C, cTSRect, m_rcDraw) GA_F(0x5C, cTSBuffer *, m_pBuffer) GA_F(0x11A, bool, m_bInDraw) };
#else
    union { GA_F(0x1C, cTSRect, m_rcDraw) GA_F(0x162, bool, m_bInDraw) };
#endif
};
ORIG(TSPaint, bool(__thiscall *)(cDDDSimsView *, bool));
static bool __fastcall W_TSPaint(cDDDSimsView *v, void *, bool bParentPainted)
{
    bool bDraws = !v->m_bInDraw;
#if GAME_CC
    if (bDraws)
        Seasons_PrePaint();
#endif
    bool r = o_TSPaint(v, bParentPainted);
    if (bDraws)
#if GAME_CC
        Seasons_PaintOverlay(v->m_pBuffer, v->m_rcDraw.left, v->m_rcDraw.top, v->m_rcDraw.right, v->m_rcDraw.bottom);
#else
        Seasons_PaintOverlay(v->m_rcDraw.left, v->m_rcDraw.top, v->m_rcDraw.right, v->m_rcDraw.bottom);
#endif
    return r;
}

// ---- SimAntics ---------------------------------------------------------------------------------------------------

// cXObject::InterpValue: globals 38..47 are the seasons contract, writable from trees so the Seasons Controller object
// can set the season and weather: for a destination read of one of them, the original (which only reads them) gets the
// global's address as its destination, as it does for EnableGrassSimulation. The value read is unchanged.
ORIG(InterpValue, short(__thiscall *)(cXObject *, int, short, bool));
static short __fastcall W_InterpValue(cXObject *p, void *, int nOwner, short nIndex, bool bWantDest)
{
    short r = o_InterpValue(p, nOwner, nIndex, bWantDest);
    if ((short)nOwner == 6 /* DO_Global_from_Simulation */ && nIndex >= kGlobalSeasonsFirst && nIndex <= kGlobalSeasonsLast && bWantDest)
        *p->m_pInterpDest = &Settings()->m_pSimulator->m_Globals.a[(unsigned char)nIndex];
    return r;
}

// cXObject::TryFindGoodLocation: find_location_for modes 7..11 (snow) are the mod's; any other mode runs the original
ORIG(TryFindGoodLocation, int(__thiscall *)(cXObject *, StackElem *, BehaviorNodeParam *));
static int __fastcall W_TryFindGoodLocation(cXObject *p, void *, StackElem *pFrame, BehaviorNodeParam *pParams)
{
    int nResult;
    if (Seasons_FindGoodLocation(p, pFrame, pParams, &nResult))
        return nResult;
    return o_TryFindGoodLocation(p, pFrame, pParams);
}

// ---- dressing ----------------------------------------------------------------------------------------------------

// cXObject::SetLocation: a person whose room changed is reported (dressing at the door). A slotted child's
// SetLocation, run inside, reports its own change before this one.
ORIG(SetLocation, void(__thiscall *)(cXObject *, const FTilePt *, int, int));
static void __fastcall W_SetLocation(cXObject *p, void *, const FTilePt *pPos, int nLevel, int nRecursion)
{
    short nOldRoom = p->m_nRoom;
    o_SetLocation(p, pPos, nLevel, nRecursion);
    if (nOldRoom != p->m_nRoom && p->m_pObjdData->type == kObjType_Person)
        Seasons_PersonRoomChanged(p, nOldRoom, p->m_nRoom);
}

// cXPerson::TryChangeSuit: a change into the raincoat outfit (61; Op[1] == 1 without Op[2] bit 0, the path that picks a
// random costume index) keeps a raincoat try-on (offset 100 + coat code) and is 0 otherwise. The original's random
// draws still happen, as before.
ORIG(TryChangeSuit, int(__thiscall *)(cXPerson *, StackElem *, BehaviorNodeParam *));
static int __fastcall W_TryChangeSuit(cXPerson *p, void *, StackElem *pFrame, BehaviorNodeParam *pParams)
{
    short nOldOffset = p->m_PersonData[kPersonDataCostumeTrunkOffset];
    int r = o_TryChangeSuit(p, pFrame, pParams);
    if (pParams->m_Op[1] == 1 && !(pParams->m_Op[2] & 1) && pParams->m_Op[0] == kOutfitRainwear)
        p->m_PersonData[kPersonDataCostumeTrunkOffset] = nOldOffset >= 100 ? nOldOffset : 0;
    return r;
}

// cXPerson::TryGotoRoutingSlot(BehaviorNodeParam): a target that was deleted while the Sim walked to it fails the route
// (returns 0) instead of raising "Object for routing either does not exist or is out of bounds". A deleted object's
// id is a null slot inside m_Objects (the table never shrinks), so only that case is caught; an id <= 0 or past the
// table still throws in the original.
ORIG(TryGotoRoutingSlotParam, int(__thiscall *)(cXPerson *, StackElem *, BehaviorNodeParam *));
static int __fastcall W_TryGotoRoutingSlotParam(cXPerson *p, void *, StackElem *pFrame, BehaviorNodeParam *pParams)
{
    if (!pFrame->m_nYieldState)
    {
        int nId = pFrame->m_nStackObjectId;
        if (nId > 0 && (unsigned)nId <= p->m_pModule->m_Objects.size() && !p->m_pModule->GetObject(nId))
            return 0;
    }
    return o_TryGotoRoutingSlotParam(p, pFrame, pParams);
}

// cXPerson::TryGotoRoutingSlot(RoutingSlot), state 2 (back from a portal tree): through a door into or out of winter
// (or the rain), spin into the outfit as part of the walk, the way the door's own portal tree ran; the route resumes in
// state 2 afterwards (the outfit was taken, so it goes on as before).
static bool __cdecl Q_DoorDress(cXPerson *p)
{
    short nOutfit;
    if (!Seasons_TakeDoorOutfit(p, &nOutfit))
        return false;
    short aParams[4] = { nOutfit, 0, 0, 0 };
    return p->GosubObjectTree(p, aParams, kGlobalTreeDressInSpecifiedOutfit, false);
}
static void *s_vaGrsState2Next, *s_vaGrsYield;
#if GAME_CC
// ASM: replaces `mov eax, [ebp-0x1c]; mov ax, [eax+0x84]` (10 bytes: the load of pStackObj and its room for the room
// compare) in state 2, after the ShouldInterrupt test. esi = this, [ebp-0x1c] = pStackObj, [ebp-0x10] = the state
// variable (the yield tail stores it as pFrame->m_nYieldState), [ebp-0x18] = nResult; ebx (0) and edi (the route) stay
// live, so are kept; ecx / edx are kept too. Dressing: nResult = 3, state 2, then the yield tail (route fields written
// back, return nResult), as the portal-route start does. Otherwise the replaced instructions, then back to the compare.
__declspec(naked) static void S_GrsState2()
{
    __asm
    {
        push ecx
        push edx
        push esi
        call Q_DoorDress
        add esp, 4
        pop edx
        pop ecx
        test al, al
        jnz dress
        mov eax, [ebp - 0x1c]
        mov ax, word ptr [eax + 0x84]
        jmp dword ptr [s_vaGrsState2Next]
    dress:
        mov dword ptr [ebp - 0x18], 3
        mov dword ptr [ebp - 0x10], 2
        jmp dword ptr [s_vaGrsYield]
    }
}
#else
// ASM: replaces `mov ecx, [ebp-0x170]` (6 bytes at 0x5f7d96, the load of pStackObj for the room compare) in state 2,
// after the ShouldInterrupt test. edi = this, [ebp-0x178] = pFrame, [ebp-0x180] = nResult; eax / ecx / edx are dead
// here (ecx is loaded by the replaced instruction, eax next, edx reloaded or clobbered by a call on every path).
// Dressing: nResult = 3, pFrame->m_nYieldState = 2, then the yield tail (0x5f8b55: route fields written back, return
// nResult), as the portal-route start does. Otherwise the replaced instruction, then back to 0x5f7d9c.
__declspec(naked) static void S_GrsState2()
{
    __asm
    {
        push edi
        call Q_DoorDress
        add esp, 4
        test al, al
        jnz dress
        mov ecx, [ebp - 0x170]
        jmp dword ptr [s_vaGrsState2Next]
    dress:
        mov dword ptr [ebp - 0x180], 3
        mov eax, [ebp - 0x178]
        mov dword ptr [eax + 8], 2
        jmp dword ptr [s_vaGrsYield]
    }
}
#endif

// SAnimator::UpdateOutfitAndHands: outfit 61 is not in the original's outfit switch, so the original dresses a Sim in
// it in everyday clothes. Afterwards the body slot gets the Sim's raincoat when it is not already wearing it; with no
// raincoat installed for the Sim it stays in everyday clothes and is put in outfit 0. Not for the VitaBoy-flag-8
// objects, and only while the animator shows outfit 61.
ORIG(UpdateOutfitAndHands, void(__thiscall *)(SAnimator *, bool));
static void __fastcall W_UpdateOutfitAndHands(SAnimator *a, void *, bool bForce)
{
    o_UpdateOutfitAndHands(a, bForce);
    cXPerson *pPerson = a->m_pPerson;
    if (!pPerson || (pPerson->m_pObjdData->VitaBoy_Flags & 8) != 0 || a->m_nAppliedOutfit != kOutfitRainwear)
        return;
    StackString<256> strSuit;
    StackString<256> strTex;
    if (!Seasons_GetRainwearSuitTex(pPerson, strSuit, strTex))
    {
        pPerson->m_PersonData[kPersonDataCurrentOutfit] = 0;
        a->m_nAppliedOutfit = 0;
        return;
    }
    DressingRecord *pBodySlot = a->m_HandSuits.empty() ? 0 : a->m_HandSuits[0];
    if (pBodySlot && pBodySlot->m_pBinding && !pBodySlot->m_Suit.compare(0, strSuit.length(), strSuit.c_str()) &&
        !pBodySlot->m_Skin.compare(0, strTex.length(), strTex.c_str()))
        return;                                         // already in it
    if (a->m_pAnimObject && pBodySlot)
    {
        CTGString suit(strSuit.c_str()), tex(strTex.c_str());
        pBodySlot->Redress(Global<VBAnimMgr *>(A::g_pAnimationManager), a->m_pAnimObject->m_pSkeleton, &suit, &tex);
    }
}

// cWinClothingChoiceDlg::Init: a raincoat / umbrella dialog (clothing types 6 / 7, which the original's outfit switch
// gives 0) makes the vita button's SetPerson use the pseudo-outfit while it runs
class cWinClothingChoiceDlg
{
public:
    union { GA_F(GA_O(0x130, 0xD8), int, m_eClothingType) };
};
static int s_nPendingVitaOutfit = -1;
ORIG(ClothingDlgInit, bool(__thiscall *)(cWinClothingChoiceDlg *));
static bool __fastcall W_ClothingDlgInit(cWinClothingChoiceDlg *d, void *)
{
    int nType = d->m_eClothingType;
    s_nPendingVitaOutfit = nType == kClothingTypeRainwear ? kOutfitRainwear : nType == kClothingTypeUmbrella ? kOutfitUmbrellaPreview : -1;
    struct Clear { ~Clear() { s_nPendingVitaOutfit = -1; } } clear;
    return o_ClothingDlgInit(d);
}

// cWinVitaBtnSolo: the raincoat / umbrella choice dialogs (pseudo-outfits 61 / 62)
class cWinVitaBtnSolo
{
public:
    union
    {
        GA_F(GA_O(0x200, 0x1A8), cXPerson *, m_pOwner) GA_F(GA_O(0x204, 0x1AC), void *, m_pEditPerson)
        GA_F(GA_O(0x208, 0x1B0), int, m_nCurrentOutfit)
    };
};

// SetPerson: the clothing dialog's Init passes 0 for a type the original does not know; while it runs for a raincoat /
// umbrella dialog that 0 becomes the pseudo-outfit. Afterwards: raincoats start at the coat the Sim has (then dressed
// again: the try-on code changed); umbrellas are held open over everyday clothes (the preview Sim goes back to outfit 0,
// its dialog stays in 62).
ORIG(SetPerson, void(__thiscall *)(cWinVitaBtnSolo *, cXPerson *, int));
static void __fastcall W_SetPerson(cWinVitaBtnSolo *b, void *, cXPerson *pSrcPerson, int eOutfit)
{
    if (eOutfit == 0 && s_nPendingVitaOutfit >= 0)
        eOutfit = s_nPendingVitaOutfit;
    o_SetPerson(b, pSrcPerson, eOutfit);
    cXPerson *pOwner = b->m_pOwner;
    if (!pOwner)
        return;
    if (b->m_nCurrentOutfit == kOutfitRainwear)
    {
        Seasons_RainwearDialogBegin(pOwner, pSrcPerson);
        if (SAnimator *pAnim = pOwner->GetSAnimator())
            pAnim->UpdateOutfitAndHands(false);
    }
    else if (b->m_nCurrentOutfit == kOutfitUmbrellaPreview)
    {
        pOwner->m_PersonData[kPersonDataCurrentOutfit] = 0;
        if (SAnimator *pAnim = pOwner->GetSAnimator())
            pAnim->UpdateOutfitAndHands(false);
        Seasons_UmbrellaDialogBegin(pOwner, pSrcPerson);
    }
}

// GetCurrentSuitIndex: the code of the raincoat / umbrella shown
ORIG(GetCurrentSuitIndex, int(__thiscall *)(cWinVitaBtnSolo *));
static int __fastcall W_GetCurrentSuitIndex(cWinVitaBtnSolo *b, void *)
{
    int nSuit = o_GetCurrentSuitIndex(b);
    if (b->m_nCurrentOutfit == kOutfitRainwear)
        return Seasons_RainwearDialogCode(b->m_pOwner);
    if (b->m_nCurrentOutfit == kOutfitUmbrellaPreview)
        return Seasons_UmbrellaDialogCode(b->m_pOwner);
    return nSuit;
}

// PreviousSuit / NextSuit: step to the previous / next raincoat or umbrella; the original body then marks the suit
// changed and dresses the preview (its switch has no case for these outfits)
static void VitaStep(cWinVitaBtnSolo *b, int nDir)
{
    if (!b->m_pEditPerson)
        return;
    try
    {
        if (b->m_nCurrentOutfit == kOutfitRainwear)
            Seasons_RainwearDialogStep(b->m_pOwner, nDir);
        else if (b->m_nCurrentOutfit == kOutfitUmbrellaPreview)
            Seasons_UmbrellaDialogStep(b->m_pOwner, nDir);
    }
    catch (...)
    {
    }
}
ORIG(PreviousSuit, void(__thiscall *)(cWinVitaBtnSolo *));
static void __fastcall W_PreviousSuit(cWinVitaBtnSolo *b, void *)
{
    VitaStep(b, -1);
    o_PreviousSuit(b);
}
ORIG(NextSuit, void(__thiscall *)(cWinVitaBtnSolo *));
static void __fastcall W_NextSuit(cWinVitaBtnSolo *b, void *)
{
    VitaStep(b, 1);
    o_NextSuit(b);
}

// Neighbor::GetPersistentDataFields: the raincoat and umbrella a Sim bought are kept with the neighbour (person data
// 240 / 241). Legacy: the two ends of the path that builds the list (re-built when the persistent-data version changes)
// call S_PdfBuilt; the cached path returns the list already extended. 2003: the entry is wrapped.
struct PersDataPair { int m_nField, m_nKind; };
#if GAME_CC
// 2003: no other mod touches this function; its entry is wrapped
ORIG(GetPersistentDataFields, GVec<PersDataPair> *(__cdecl *)(int));
static GVec<PersDataPair> *__cdecl W_GetPersistentDataFields(int nVersion)
{
    GVec<PersDataPair> *pFields = o_GetPersistentDataFields(nVersion);
    size_t n = pFields->size();
    if (n < 2 || (*pFields)[n - 2].m_nField != kPersonDataSeasonsRainCoat || (*pFields)[n - 1].m_nField != kPersonDataSeasonsUmbrella)
    {
        // the game's vector: grown by the exe's own push_back (its allocator)
        pFields->PushBack(A::PersDataVec__push_back, { kPersonDataSeasonsRainCoat, 1 });
        pFields->PushBack(A::PersDataVec__push_back, { kPersonDataSeasonsUmbrella, 1 });
    }
    return pFields;
}
#else
static void __cdecl Q_AppendFields()
{
    std::vector<PersDataPair> &fields = Global<std::vector<PersDataPair>>(A::s_PersistentFields);
    size_t n = fields.size();
    if (n >= 2 && fields[n - 2].m_nField == kPersonDataSeasonsRainCoat && fields[n - 1].m_nField == kPersonDataSeasonsUmbrella)
        return;
    fields.push_back({ kPersonDataSeasonsRainCoat, 1 });
    fields.push_back({ kPersonDataSeasonsUmbrella, 1 });
}
static int *s_pPersistentFieldsVersion;
// ASM: replaces `mov dword ptr [s_nPersistentFieldsVersion], edi` (6 bytes; call + nop) at both ends of the build path.
// Does the store, appends the fields, returns to the original's return sequence; eax (the result on one path), ecx and
// edx are kept.
__declspec(naked) static void S_PdfBuilt()
{
    __asm
    {
        push eax
        push ecx
        push edx
        mov eax, s_pPersistentFieldsVersion
        mov [eax], edi
        call Q_AppendFields
        pop edx
        pop ecx
        pop eax
        ret
    }
}
#endif

// ---- the hook table ------------------------------------------------------------------------------------------------

static const Hook s_CoreHooks[] = {
    WRAP(cSimulator__Init,                          W_SimulatorInit, o_SimulatorInit),
    WRAP(cTerrain__SetColor,                        W_SetColor, o_SetColor),
    WRAP(cColorSwatch__PreCompute,                  W_PreCompute, o_PreCompute),
    WRAP(cColorSwatch__ComputeMeshPointColorIndex,  W_ComputeMeshPointColorIndex, o_ComputeMeshPointColorIndex),
#if !GAME_CC
    WRAP(TerrainTileScratch__ComputeDetailFade,     W_ComputeDetailFade, o_ComputeDetailFade),
    CALL(EA_EmitQuad1,                              C_EmitQuad),
    CALL(EA_EmitQuad2,                              C_EmitQuad),
#endif
    WRAP(cDDDSimsView__TSPaint,                     W_TSPaint, o_TSPaint),
    WRAP(cXObject__InterpValue,                     W_InterpValue, o_InterpValue),
    WRAP(cXObject__TryFindGoodLocation,             W_TryFindGoodLocation, o_TryFindGoodLocation),
    WRAP(cXObject__SetLocation,                     W_SetLocation, o_SetLocation),
    WRAP(cXPerson__TryChangeSuit,                   W_TryChangeSuit, o_TryChangeSuit),
    WRAP(cXPerson__TryGotoRoutingSlot_param,        W_TryGotoRoutingSlotParam, o_TryGotoRoutingSlotParam),
    JMP_STUB(GRS_State2,                            S_GrsState2),
    WRAP(SAnimator__UpdateOutfitAndHands,           W_UpdateOutfitAndHands, o_UpdateOutfitAndHands),
    WRAP(cWinClothingChoiceDlg__Init,               W_ClothingDlgInit, o_ClothingDlgInit),
    WRAP(cWinVitaBtnSolo__SetPerson,                W_SetPerson, o_SetPerson),
    WRAP(cWinVitaBtnSolo__GetCurrentSuitIndex,      W_GetCurrentSuitIndex, o_GetCurrentSuitIndex),
    WRAP(cWinVitaBtnSolo__PreviousSuit,             W_PreviousSuit, o_PreviousSuit),
    WRAP(cWinVitaBtnSolo__NextSuit,                 W_NextSuit, o_NextSuit),
#if GAME_CC
    WRAP(Neighbor__GetPersistentDataFields,         W_GetPersistentDataFields, o_GetPersistentDataFields),
#else
    CALL_STUB(PDF_Built1,                           S_PdfBuilt),
    CALL_STUB(PDF_Built2,                           S_PdfBuilt),
#endif
};

static const HookTable s_Tables[] = {
    { s_CoreHooks, sizeof s_CoreHooks / sizeof s_CoreHooks[0] },
    { g_CareerHooks, g_nCareerHooks },
    { g_FolderHooks, g_nFolderHooks },
    { g_PanelHooks, g_nPanelHooks },
};

static const GenSig *FindSig(unsigned va)
{
    for (size_t i = 0; i < sizeof g_GenSigs / sizeof g_GenSigs[0]; ++i)
        if (g_GenSigs[i].va == va)
            return &g_GenSigs[i];
    return 0;
}

extern "C" __declspec(dllexport) int __cdecl SimsMod_Init(const SimsLoaderApi *api)
{
    if (api->version != SIMSMOD_API_VERSION)
        return 1;
    s_api = api;
    ga::g_exeBase = api->exeBase;
    s_vaGrsState2Next = Va(A::GRS_State2Next);
    s_vaGrsYield = Va(A::GRS_Yield);
#if !GAME_CC
    s_pPersistentFieldsVersion = &Global<int>(A::s_nPersistentFieldsVersion);
#endif
    // every site must match its signature, or nothing is patched
    int nBad = 0;
    unsigned nHooks = 0;
    for (const HookTable &t : s_Tables)
        for (size_t i = 0; i < t.n; ++i, ++nHooks)
        {
            const GenSig *pSig = FindSig(t.p[i].va);
            char why[160] = "no signature";
            if (!pSig || !api->SigCheck(&pSig->sig, why, sizeof why))
            {
                Seasons_Log("%s @ 0x%06x: SIGNATURE MISMATCH (%s)", t.p[i].pszName, t.p[i].va, why);
                ++nBad;
            }
        }
    if (nBad)
    {
        Seasons_Log("%d of %u hook sites do not match this Sims.exe: nothing patched, the mod is off", nBad, nHooks);
        return 2;
    }
    int nOk = 0, nFailed = 0;
    for (const HookTable &t : s_Tables)
        for (size_t i = 0; i < t.n; ++i)
        {
            const Hook &h = t.p[i];
            const GenSig *pSig = FindSig(h.va);
            bool bOk;
            if (h.kind == kWrap)
                bOk = api->Detour(h.pszName, &pSig->sig, h.pFn, h.ppOrig) && *h.ppOrig;
            else
            {
                unsigned char patch[16];
                unsigned n = pSig->patchLen;
                memset(patch, 0x90, sizeof patch);
                patch[0] = h.kind == kJmpStub ? 0xE9 : 0xE8;
                int rel = (int)((unsigned char *)h.pFn - ((unsigned char *)Va(h.va) + 5));
                memcpy(patch + 1, &rel, 4);
                bOk = n >= 5 && n <= sizeof patch && api->WriteCode(Va(h.va), patch, n);
                if (!bOk)
                    Seasons_Log("%s @ 0x%06x: patch failed", h.pszName, h.va);
            }
            bOk ? ++nOk : ++nFailed;
        }
    Seasons_Log("%d hooks installed, %d failed", nOk, nFailed);
    return nFailed ? 3 : 0;
}

BOOL WINAPI DllMain(HINSTANCE hinst, DWORD reason, LPVOID)
{
    if (reason == DLL_PROCESS_ATTACH)
        DisableThreadLibraryCalls(hinst);
    return TRUE;
}
