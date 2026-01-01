// visitlots.cpp - mods\visit_lots.dll: visiting another family's home lot (LotVisit.h), as a patch mod.
//
// Every original function keeps running as the exe's own code. The mod changes it in one of three ways:
//   wrapper  - the function's entry is detoured to a W_* function here, which applies the feature's rule and calls
//              the original through the loader's trampoline (o_*), or returns early;
//   call     - one `call X` inside an original function is redirected to a C_* function here with X's calling
//              convention, which does the feature's part and (usually) calls X;
//   stub     - a few bytes inside an original function become a jmp/call to a naked S_* asm stub (// ASM: says why)
//              that resumes at a GA_LABEL of the same function.
// Each site has a 16-byte signature (gen_sigs.h, made from the game build this mod is for); all of them are checked
// on the running game first, and nothing is patched unless every one matches.
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include "simsmod.h"
#include "game_api.h"
#include "LotVisit.h"
#include "LotVisitText.h"
#include "gen_sigs.h"

unsigned ga::g_exeBase;
static const SimsLoaderApi *s_api;

template <class T> using Id = T;
#define ORIG(name, ...) static Id<__VA_ARGS__> o_##name    // the trampoline of a wrapped function (set at install)
class cXPortal;
class StringSet;

// Guests can't edit someone else's home.
ORIG(SetMode, void(__thiscall *)(CPState *, int, bool));
static void __fastcall W_SetMode(CPState *p, void *, int nMode, bool bForce)
{
    if ((nMode == CPState::kBuild || nMode == CPState::kBuy) && LotVisit::Active())
        return;
    o_SetMode(p, nMode, bForce);
}

// m_bBuyAndBuildDisabled |= visiting. Its only reader is outside this function, so fixing it up on return is equivalent.
ORIG(UpdateCPStateFromWorld, void(__thiscall *)(CPState *));
static void __fastcall W_UpdateCPStateFromWorld(CPState *p, void *)
{
    bool bDirty = (p->m_dwFlags & CPState::kDirtyBuyBuild) != 0;
    o_UpdateCPStateFromWorld(p);
    if (bDirty && LotVisit::Active())
        p->m_bBuyAndBuildDisabled = true;
}

// AppendInteractionsForAuto (autonomy's menu of one object): a guest gets the community-lot visitor rule (TTAB flag 5)
// and LotVisit::MayEnter instead of the room test. Constant per call, so the wrapper decides once for the two zoning calls.
static volatile int s_aifa;     // 0: not a guest, 1: a guest that may enter the object's room, 2: one that may not
ORIG(AppendInteractionsForAuto, void(__thiscall *)(ObjTestSim *, void *));
static void __fastcall W_AppendInteractionsForAuto(ObjTestSim *p, void *, void *pOut)
{
    int nSaved = s_aifa;
    s_aifa = LotVisit::IsGuest(p->m_pSim) ? (LotVisit::MayEnter(p->m_pSim, p->m_pObject) ? 1 : 2) : 0;
    o_AppendInteractionsForAuto(p, pOut);
    s_aifa = nSaved;
}
// first zoning test (`zoning != 1` selects the flag-5 visitor rule)
static int __fastcall C_AifaZoning1(Neighborhood *n, void *, int nHouse)
{
    return s_aifa ? 2 : n->GetZoningType(nHouse);
}
// second zoning test: `zoning != 1 || !personType || human || greeted || no room` -> append, else skip. For a guest the
// rule is MayEnter alone, which no return value can express, hence the stub.
static void *s_vaGetZoningType, *s_vaAifaAppend, *s_vaAifaSkip;
// ASM: replaces `call Neighborhood::GetZoningType` at 0x60abbf; a guest skips the rest of the test and continues at
// the push_back (0x60abfc) or at the next entry (0x60ac4e). Stack on entry: [ret][house]; GetZoningType is thiscall,
// so dropping both restores the caller's esp. Registers live at both targets: ebx (out), esi (entry), edi (this),
// ebp; none is touched here.
__declspec(naked) static void S_AifaZoning2()
{
    __asm
    {
        cmp s_aifa, 0
        jne guest
        jmp dword ptr [s_vaGetZoningType]   // not a guest: the original call (returns to 0x60abc4)
    guest:
        add esp, 8
        cmp s_aifa, 1
        jne skip
        jmp dword ptr [s_vaAifaAppend]
    skip:
        jmp dword ptr [s_vaAifaSkip]
    }
}

// A guest never leaves the lot on its own (BlocksInteraction) and gets the non-residential visitor rule (TTAB flag 5).
static volatile int s_tiGuest;
ORIG(TestInteraction, void(__thiscall *)(ObjTestSim *, InteractionData *, TreeTableEntry **));
static void __fastcall W_TestInteraction(ObjTestSim *p, void *, InteractionData *pAction, TreeTableEntry **ppOut)
{
    cXPerson *pSim = pAction->m_pFrom;
    if (pAction->m_pTargetObj && LotVisit::Active())
        if (TreeTableEntry *pEntry = pAction->GetEntry())
            if (LotVisit::BlocksInteraction(pSim, pAction->m_pTargetObj, pEntry->m_nActionTreeId))
            {
                // the original's "not available" path: flag 2 on, 3 and 4 off, nothing hidden, no guard tree
                pAction->m_nFlags = (pAction->m_nFlags & ~0x18u) | 0x04u;
                pSim->m_nHideInteraction = 0;
                return;
            }
    int nSaved = s_tiGuest;
    s_tiGuest = LotVisit::IsGuest(pSim);
    o_TestInteraction(p, pAction, ppOut);
    s_tiGuest = nSaved;
}
static bool __fastcall C_TiResidential(cSimulator *s, void *)
{
    return s_tiGuest ? false : s->IsCurrentLotResidential();
}

// On a visited home the selection cycles through the guests only.
ORIG(AdvanceSelectedPerson, cXPerson *(__thiscall *)(ObjectModule *));
static cXPerson *__fastcall W_AdvanceSelectedPerson(ObjectModule *m, void *)
{
    if (LotVisit::Active() && !VisitorControl())
    {
        cXPerson *pNext = LotVisit::NextGuest(m->GetSelectedPerson());
        m->SetSelectedPerson(pNext);
        return pNext;
    }
    return o_AdvanceSelectedPerson(m);
}

// A visitor script removing a guest ends the visit instead.
ORIG(AddToKillQueue, void(__thiscall *)(ObjectModule *, short, bool));
static void __fastcall W_AddToKillQueue(ObjectModule *m, void *, short nObjectId, bool bImmediate)
{
    if (LotVisit::OnGuestKillQueued(m->GetObject(nObjectId)))
        return;
    o_AddToKillQueue(m, nObjectId, bImmediate);
}

// LotVisit::Tick, then the selected-person check with the visit's rule. The original's check (the call to
// AdvanceSelectedPerson at 0x5315b1) is disabled by C_PostSimNoAdvance.
ORIG(PostSim, bool(__thiscall *)(ObjectModule *, bool));
static bool __fastcall W_PostSim(ObjectModule *m, void *, bool bSkipDeletes)
{
    bool bSituationFired = o_PostSim(m, bSkipDeletes);
    LotVisit::Tick();
    cXPerson *pSelected = m->GetSelectedPerson();
    if (!pSelected
        || (Settings()->m_pNeighborhood->GetZoningType(Settings()->m_pSimulator->m_Globals.CurrentHouse) == 1
            && !VisitorControl()
            && (LotVisit::Active() ? !LotVisit::IsGuest(pSelected) : pSelected->m_nPersonType != 0)))
        m->AdvanceSelectedPerson();
    return bSituationFired;
}
static cXPerson *__fastcall C_PostSimNoAdvance(ObjectModule *, void *)
{
    return 0;                                           // the caller ignores the result
}

// Neighbourhood picker opened from a lot (PostSim 0x530d6f: `zoning == 2` -> taxi) and its click handler
// (VisitModeLotHandler 0x4aa52d): a visitable home counts as a community lot. CanVisit implies zoning 1.
static int __fastcall C_ZoningVisitableAsCommunity(Neighborhood *n, void *, int nHouse)
{
    return LotVisit::CanVisit(nHouse) ? 2 : n->GetZoningType(nHouse);
}

// Engine extensions used by the game data: 44 LotVisitGuestInTemp0, 45 LotVisitGoHome; and call 28 (lot zoning), where
// a visited home reads as a community lot to a guest's scripts.
ORIG(TryGenericSimCall, int(__thiscall *)(cXObject *, StackElem *, BehaviorNodeParam *));
static int __fastcall W_TryGenericSimCall(cXObject *self, void *, StackElem *pFrame, BehaviorNodeParam *pParams)
{
    switch (pParams->m_Param[0])
    {
    case 44:
        self->m_Temps[0] = LotVisit::IsGuest(AsPerson(self)) ? 1 : 0;
        return 1;
    case 45:
        LotVisit::GoHome(AsPerson(self));
        return 1;
    case 28:
    {
        short nHouse = self->m_Temps[0];
        if ((unsigned short)(nHouse - 90) > 2 && (unsigned short)(nHouse - 93) > 5 && LotVisit::IsVisitedHouse(nHouse)
            && LotVisit::GuestSeesCommunity(AsPerson(self)))
        {
            self->m_Temps[0] = 1;                       // zoning 2 (community)
            return 1;
        }
        break;
    }
    }
    return o_TryGenericSimCall(self, pFrame, pParams);
}

// the player's household on a visited lot is the guests
ORIG(CountPeopleInHouse, int(__cdecl *)());
static int __cdecl W_CountPeopleInHouse()
{
    return LotVisit::Active() ? LotVisit::GuestCount() : o_CountPeopleInHouse();
}
ORIG(GetPersonInHouseIndex, int(__cdecl *)(int));
static int __cdecl W_GetPersonInHouseIndex(int nIndex)
{
    if (LotVisit::Active())
    {
        int nGuestGuid = LotVisit::GuestGuid(nIndex);
        return nGuestGuid ? PersonFinder::GetHandle(nGuestGuid) : -1;
    }
    return o_GetPersonInHouseIndex(nIndex);
}
// the exe inlined GetPersonInHouseIndex here, so the guest walk is done in the wrapper
ORIG(GetIndexFromPersonHandle, int(__cdecl *)(int));
static int __cdecl W_GetIndexFromPersonHandle(int nHandle)
{
    if (!LotVisit::Active())
        return o_GetIndexFromPersonHandle(nHandle);
    if (!Settings()->m_pGame->m_pFamily)
        return -1;
    int nCount = LotVisit::GuestCount();
    for (int i = 0; i < nCount; ++i)
        if (PersonFinder::GetPersonInHouseIndex(i) == nHandle)
            return i;
    return -1;
}
// On a visited home only the guests are ours to control.
ORIG(SetSelected, void(__cdecl *)(unsigned));
static void __cdecl W_SetSelected(unsigned nHandle)
{
    if (LotVisit::Active())
    {
        ObjectModule *pModule = Settings()->m_pGame->m_pObjectModule;
        if (!pModule)
            return;
        int nGuid = PersonFinder::GetGUID(nHandle);
        cXPerson *pObject = nGuid ? (cXPerson *)pModule->GetObjectByGUID(nGuid) : 0;
        if (!pObject)
        {
            pModule->SetSelectedPerson(0);
            return;
        }
        if (Settings()->m_pNeighborhood->GetZoningType(Settings()->m_pSimulator->m_Globals.CurrentHouse) == 1)
        {
            if (!VisitorControl() && !LotVisit::IsGuest(pObject))
                pObject = 0;
            pModule->SetSelectedPerson(pObject);
            return;
        }
    }
    o_SetSelected(nHandle);
}

// Visited house: only the guests' persistent data is saved (SaveGuestsOnly); the hosts' files stay untouched. Replaces
// the original's lot/neighbourhood saving but keeps its head (watch cursor, flag 3) and tail (arrow cursor, dirty).
ORIG(SaveGame, bool(__thiscall *)(cSimsApp *));
static bool __fastcall W_SaveGame(cSimsApp *app, void *)
{
    if (Global<bool>(A::g_bAutonomySuspended) || !LotVisit::IsVisitedHouse(Settings()->m_pSimulator->m_Globals.CurrentHouse))
        return o_SaveGame(app);
    app->m_pLotView->StartWatchCursor();
    Neighborhood *pNeighborhood = Settings()->m_pNeighborhood;
    pNeighborhood->m_NeighborhoodData[3] = 1;
    LotVisit::SaveGuestsOnly(pNeighborhood);
    cTSWinMgrW95 *pWinMgr = WinMgr();
    pWinMgr->SetGlobalCursor(pWinMgr->m_pCursorMgr->GetStandardCursor(0), true);
    app->m_pRootWin->UpdateCursor();
    app->m_pCPState->SetDirty(-1);
    return true;
}

// OnLoadStart first; after the lot is loaded (the PersonFinder::GetSelected call, reached on every path) the guests
// arrive; a visit disables saving and hides the hosts' funds and friends counters (the two CPState calls).
static int s_nLoadingHouse;
ORIG(LoadGame, bool(__thiscall *)(cSimsApp *, int, int));
static bool __fastcall W_LoadGame(cSimsApp *app, void *, int nHouseId, int nFlags)
{
    LotVisit::OnLoadStart();
    int nSaved = s_nLoadingHouse;
    s_nLoadingHouse = nHouseId;
    bool bOk = o_LoadGame(app, nHouseId, nFlags);
    s_nLoadingHouse = nSaved;
    return bOk;
}
// The original inlines this token sequence at its four arrival sites.
static void RestoreClockFromTokens(SimInventory *pInventory)
{
    short nTokenIndex = -1;
    SimInventoryToken clockToken = {};
    pInventory->SetToNextToken(kToken_SimData, nTokenIndex, kSimData_ArriveHour);
    if (nTokenIndex == -1)
        return;
    pInventory->GetTokenAtIndex(nTokenIndex, clockToken);
    short nSavedHour = clockToken.m_nCount;
    pInventory->RemoveTokenByIndex(nTokenIndex, -1);
    nTokenIndex = -1;
    pInventory->SetToNextToken(kToken_SimData, nTokenIndex, kSimData_ArriveMinute);
    if (nTokenIndex == -1)
        return;
    pInventory->GetTokenAtIndex(nTokenIndex, clockToken);
    short nSavedMinute = clockToken.m_nCount;
    pInventory->RemoveTokenByIndex(nTokenIndex, -1);
    Settings()->m_pSimulator->SetCurrentHour(nSavedHour, nSavedMinute);
}
// SelectFirstGuest runs before the selection fallback here; on a visit it selects a guest, making the fallback a
// no-op with the same result as the reverse order.
static int __cdecl C_LoadGameLotLoaded()
{
    if (cXPerson *pFirstGuest = LotVisit::OnLotLoaded((short)s_nLoadingHouse))
        if (SimInventory *pInventory = Settings()->m_pNeighborhood->GetInventoryByNeighbourID(pFirstGuest->m_nNeighborId))
            RestoreClockFromTokens(pInventory);
    LotVisit::SelectFirstGuest();
    cSimsApp *app = SimsApp();
    app->m_pCPState->SetDirty(CPState::kDirtyBuyBuild);
    if (LotVisit::Active())
        app->RemoveTaxiDialog();                        // no ped marker on a home lot does it
    return PersonFinder::GetSelected();
}
static void __fastcall C_LoadGameDisableSave(CPState *p, void *, bool bDisable)
{
    p->DisableSave(bDisable || LotVisit::Active());    // a visited home is never saved
}
static void __fastcall C_LoadGameHideButtons(CPState *p, void *, bool bHide)
{
    p->HideButtonsForLocation(bHide || LotVisit::Active());    // the hosts' funds / friends (as on a community lot)
}

// Simulate, lot change: decide whether the destination is a visit while still on the old lot.
static void __fastcall C_SimulateUnloadHouse(Neighborhood *n, void *)
{
    cSimsApp *app = SimsApp();
    LotVisit::PrepareTrip(app->m_nInputCode, app->m_nInputParam, app->m_nMoveInFamilyNumber);
    n->UnloadHouse();
}

// Quit during a visit: the visit's own warning (nothing of it can be saved) instead of the save prompts.
static void *s_vaIsSaveDisabled, *s_vaQuitAfterTest, *s_vaQuitTail;
static bool __cdecl Q_Active() { return LotVisit::Active(); }
static bool __cdecl Q_ConfirmLeaving() { return LotVisit::ConfirmLeaving(); }
// ASM: replaces `mov ecx,[esi+0x7c]; call CPState::IsSaveDisabled` (8 bytes at 0x459917), the test that starts
// Quit's chain of dialogs. esi = the cITSApp part of cSimsApp (+0x7c from it is m_pCPState), bl = bDoQuit (0 here),
// bh = bFamilyOnLot. Visiting: bl = ConfirmLeaving() and continue at 0x459a39, the common tail after the dialogs
// (g_bQuitInProgress = 0, the SimLog prompt, then quit if bl); no string was constructed, so the EH state is still
// -1, as the tail expects. Otherwise the two original instructions, then back to 0x45991f (`test al,al`).
__declspec(naked) static void S_QuitSaveDisabled()
{
    __asm
    {
        call Q_Active
        test al, al
        jnz visiting
        mov ecx, [esi + 0x7c]
        call dword ptr [s_vaIsSaveDisabled]
        jmp dword ptr [s_vaQuitAfterTest]
    visiting:
        call Q_ConfirmLeaving
        mov bl, al
        jmp dword ptr [s_vaQuitTail]
    }
}

// -lang:<name or 1..20> overrides the registry and Steam languages (e.g. -lang:polish, -lang:pl, -lang:16). Applied
// at the ReadSimsRegKey("SIMS_SKU") call at 0x458175: after the language switches, reached on every path.
static void ApplyLangOption()
{
    static const char *const s_LangNames[21][3] = {
        { "", "", "" },
        { "english", "en", "usenglish" }, { "british", "en_uk", "ukenglish" }, { "french", "fr", "" },
        { "german", "de", "" }, { "italian", "it", "" }, { "spanish", "es", "" }, { "dutch", "nl", "" },
        { "danish", "da", "" }, { "swedish", "sv", "" }, { "norwegian", "no", "" }, { "finnish", "fi", "" },
        { "hebrew", "he", "" }, { "russian", "ru", "" }, { "portuguese", "pt", "brazilian" },
        { "japanese", "ja", "" }, { "polish", "pl", "" }, { "schinese", "zh_cn", "simplifiedchinese" },
        { "tchinese", "zh_tw", "traditionalchinese" }, { "thai", "th", "" }, { "koreana", "ko", "korean" },
    };
    const char *p = GetCommandLineA();
    char szArg[128];
    while (*p)
    {
        while (*p == ' ' || *p == '\t')
            ++p;
        unsigned n = 0;
        bool bQuoted = false;
        for (; *p && (bQuoted || (*p != ' ' && *p != '\t')); ++p)
            if (*p == '"')
                bQuoted = !bQuoted;
            else if (n + 1 < sizeof szArg)
                szArg[n++] = *p;
        szArg[n] = 0;
        if (_strnicmp(szArg, "-lang:", 6) != 0)
            continue;
        const char *pszValue = szArg + 6;
        int nPick = atoi(pszValue);
        for (int nLang = 1; nLang <= 20 && !(nPick >= 1 && nPick <= 20); ++nLang)
            for (int nName = 0; nName < 3; ++nName)
                if (*s_LangNames[nLang][nName] && !_stricmp(pszValue, s_LangNames[nLang][nName]))
                    nPick = nLang;
        if (nPick >= 1 && nPick <= 20)
            SetLanguage(nPick);
    }
}
static bool __cdecl C_PreFrameWorkInitReadSku(const char *pszKey, void *pOut)
{
    ApplyLangOption();
    return Call<bool>(A::cSimsApp__ReadSimsRegKey, pszKey, pOut);
}

// cWinLotPopup::SetHouseInfo: visit hint for a visitable home, the stock "cannot visit" line for the rest.
static cWinLotPopup *s_pLotPopup;
static int __cdecl C_LotPopupVisitLine(char *pszOut, const char *pszFormat, const char *pszBody, const char *pszEmpty)
{
    if (s_pLotPopup && s_pLotPopup->m_pHouseInfo && LotVisit::CanVisit(s_pLotPopup->m_pHouseInfo->m_nHouseId))
        return wsprintfA(pszOut, "%s\r\n%s", pszBody, LotVisitText::Get(LotVisitText::kVisitHint));
    return wsprintfA(pszOut, pszFormat, pszBody, pszEmpty);
}
// ASM: replaces `lea eax,[ebp-0xc30]; push eax; call esi` (9 bytes at 0x493443: wsprintfA(szVisitLine,
// m_ppszPopupText[20], szBody, "") - the call is through a register, too short for a call patch) with `call stub` +
// 4 NOPs. The stub pushes the same first argument under its return address and tail-jumps to the cdecl
// C_LotPopupVisitLine, which returns to the NOPs; the caller pops the four arguments as before (add esp,0x1c).
// edi = this (the code around reads m_ppszPopupText through it). Clobbers eax/ecx (wsprintfA did too).
__declspec(naked) static void S_LotPopupVisitLine()
{
    __asm
    {
        pop ecx
        lea eax, [ebp - 0xc30]
        push eax
        push ecx
        mov s_pLotPopup, edi
        jmp C_LotPopupVisitLine
    }
}

// cWinNeighborhoodUL: the visit filter reuses the lodging filter slot (slot 6), which can never be enabled in this
// window (lodging lots exist only on Vacation Island, which has its own window), so no other code needs changes.
static void __fastcall C_FilterTipVisit(cTSWin *pBtn, void *, const cTSString *)
{
    cTSString strTip(LotVisitText::Get(LotVisitText::kFilterTip));
    pBtn->SetToolTipsText(strTip);
}
static bool AnyLotVisitable(cWinNeighborhoodUL *w)
{
    bool bAny = false;
    w->m_LotBtnTree.ForEach([&](cWinLotBtn *pBtn) { return bAny = LotVisit::CanVisit(pBtn->m_HouseInfo.m_nHouseId); });
    return bAny;
}
static bool __fastcall C_FilterAnyLot(cWinNeighborhoodUL *w, void *, int nMask)
{
    if (nMask == kLotFilter_Lodging)
        return AnyLotVisitable(w);
    return ThisCall<bool>(A::cWinNeighborhoodUL__AnyLotMatchesFilter, w, nMask);
}
ORIG(DrawFilters, void(__thiscall *)(cWinNeighborhoodUL *));
static void __fastcall W_DrawFilters(cWinNeighborhoodUL *w, void *)
{
    if (w->m_nFilterMask != kLotFilter_Lodging)
        return o_DrawFilters(w);
    w->m_LotBtnTree.ForEach([&](cWinLotBtn *pBtn) {
        if (LotVisit::CanVisit(pBtn->m_HouseInfo.m_nHouseId))
            w->DrawFilterAt(pBtn->m_HouseInfo.m_nLotX - 24, pBtn->m_HouseInfo.m_nLotY - 70);
        return false;
    });
}
// the visit filter is not persisted
static void __fastcall C_SaveFilterNotVisit(cWinNeighborhoodUL *w, void *, int nFilter)
{
    ThisCall<void>(A::cWinNeighborhoodUL__SaveCurrentFilter, w, nFilter == kLotFilter_Lodging ? 0 : nFilter);
}

// "Neighborhood" button (control 0) during a visit: the visit's warning; the original asks nothing on a residential lot.
ORIG(TSOnCommand, bool(__thiscall *)(cWinOptions *, unsigned long, unsigned long));
static bool __fastcall W_TSOnCommand(cWinOptions *w, void *, unsigned long nCommand, unsigned long nParam)
{
    if (nCommand == 1 && nParam && (cTSWin *)nParam == w->m_apCtrls[0] && LotVisit::Active()
        && !Settings()->m_pSimulator->m_Globals.SimlessBuildMode && !LotVisit::ConfirmLeaving())
        return true;
    return o_TSOnCommand(w, nCommand, nParam);
}

// On a visit the owners are always autonomous and the guests follow the free-will option. The free-will gate is the
// only reader of g_bForceAutonomyAlways here, so setting it for the call skips exactly that gate.
ORIG(TryFindBestAction, int(__thiscall *)(cXPerson *, StackElem *));
static int __fastcall W_TryFindBestAction(cXPerson *p, void *, StackElem *pFrame)
{
    if (!LotVisit::Active())
        return o_TryFindBestAction(p, pFrame);
    bool &bFreeWill = Global<bool>(A::g_bForceAutonomyAlways);
    if (LotVisit::IsGuest(p) && !bFreeWill && p->m_nGender <= 1)
        return 0;
    struct Restore { bool &b; bool v; ~Restore() { b = v; } } restore = { bFreeWill, bFreeWill };
    bFreeWill = true;
    return o_TryFindBestAction(p, pFrame);
}

// An ungreeted visiting guest stays in the yard (belongs in StartPortalRoute, which the exe inlined into the caller).
ORIG(FindBestPortal, cXPortal *(__cdecl *)(ObjectModule *, cXObject *, cXObject *));
static cXPortal *__cdecl W_FindBestPortal(ObjectModule *pModule, cXObject *pFrom, cXObject *pTo)
{
    if (!LotVisit::MayEnter(AsPerson(pFrom), pTo))
        return 0;
    return o_FindBestPortal(pModule, pFrom, pTo);
}

// STR# 260 of GameData\UIText.iff ships in GameData\UIText2.iff instead; LoadUIStrings serves the overlay's ids
// (read from its chunk headers) from it first, falling back to the requested file.
static unsigned short s_overlayIds[64];
static unsigned s_nOverlayIds;
static void ScanOverlayIds()
{
    char path[MAX_PATH];
    _snprintf_s(path, sizeof path, _TRUNCATE, "%s\\GameData\\UIText2.iff", s_api->exeDir);
    HANDLE h = CreateFileA(path, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
    if (h == INVALID_HANDLE_VALUE)
    {
        s_api->Log("[visitlots] no %s: UI strings come from UIText.iff only", path);
        return;
    }
    DWORD size = GetFileSize(h, 0), got = 0;
    unsigned char *d = (unsigned char *)VirtualAlloc(0, size + 1, MEM_COMMIT, PAGE_READWRITE);
    if (d && ReadFile(h, d, size, &got, 0) && got == size && size > 64)
        for (unsigned o = 64; o + 76 <= size && s_nOverlayIds < 64;)
        {
            unsigned sz = (d[o + 4] << 24) | (d[o + 5] << 16) | (d[o + 6] << 8) | d[o + 7];
            if (sz < 76 || o + sz > size)
                break;
            if (!memcmp(d + o, "STR#", 4))
                s_overlayIds[s_nOverlayIds++] = (unsigned short)((d[o + 8] << 8) | d[o + 9]);
            o += sz;
        }
    if (d)
        VirtualFree(d, 0, MEM_RELEASE);
    CloseHandle(h);
}
ORIG(LoadUIStrings, bool(__cdecl *)(int, const char *, StringSet *, bool));
static bool __cdecl W_LoadUIStrings(int nId, const char *pszFile, StringSet *pStrings, bool bFlag)
{
    if (pszFile && !_stricmp(pszFile, "UIText"))
        for (unsigned i = 0; i < s_nOverlayIds; ++i)
            if (s_overlayIds[i] == nId)
            {
                if (o_LoadUIStrings(nId, "UIText2", pStrings, bFlag))
                    return true;
                break;
            }
    return o_LoadUIStrings(nId, pszFile, pStrings, bFlag);
}

enum Kind { kWrap, kCall, kJmpStub, kCallStub };
struct Hook
{
    const char *pszName;
    unsigned va;
    Kind kind;
    void *pFn;
    void **ppOrig;          // wrappers: receives the trampoline
};
#define WRAP(site, fn, orig) { #site, A::site, kWrap, (void *)&fn, (void **)&orig }
#define CALL(site, fn) { #site, A::site, kCall, (void *)&fn, 0 }
#define JMP_STUB(site, fn) { #site, A::site, kJmpStub, (void *)&fn, 0 }
#define CALL_STUB(site, fn) { #site, A::site, kCallStub, (void *)&fn, 0 }

static const Hook s_Hooks[] = {
    WRAP(CPState__SetMode,                          W_SetMode, o_SetMode),
    WRAP(CPState__UpdateCPStateFromWorld,           W_UpdateCPStateFromWorld, o_UpdateCPStateFromWorld),
    WRAP(ObjTestSim__AppendInteractionsForAuto,     W_AppendInteractionsForAuto, o_AppendInteractionsForAuto),
    CALL(AIFA_Zoning1,                              C_AifaZoning1),
    CALL_STUB(AIFA_Zoning2,                         S_AifaZoning2),
    WRAP(ObjTestSim__TestInteraction,               W_TestInteraction, o_TestInteraction),
    CALL(TI_Residential,                            C_TiResidential),
    WRAP(ObjectModule__AdvanceSelectedPerson,       W_AdvanceSelectedPerson, o_AdvanceSelectedPerson),
    WRAP(ObjectModule__AddToKillQueue,              W_AddToKillQueue, o_AddToKillQueue),
    WRAP(ObjectModule__PostSim,                     W_PostSim, o_PostSim),
    CALL(PostSim_Advance,                           C_PostSimNoAdvance),
    CALL(PostSim_NbhdZone,                          C_ZoningVisitableAsCommunity),
    WRAP(cXObject__TryGenericSimCall,               W_TryGenericSimCall, o_TryGenericSimCall),
    WRAP(PersonFinder__CountPeopleInHouse,          W_CountPeopleInHouse, o_CountPeopleInHouse),
    WRAP(PersonFinder__GetPersonInHouseIndex,       W_GetPersonInHouseIndex, o_GetPersonInHouseIndex),
    WRAP(PersonFinder__GetIndexFromPersonHandle,    W_GetIndexFromPersonHandle, o_GetIndexFromPersonHandle),
    WRAP(PersonFinder__SetSelected,                 W_SetSelected, o_SetSelected),
    WRAP(cSimsApp__SaveGame,                        W_SaveGame, o_SaveGame),
    WRAP(cSimsApp__LoadGame,                        W_LoadGame, o_LoadGame),
    CALL(LG_GetSelected,                            C_LoadGameLotLoaded),
    CALL(LG_DisableSave,                            C_LoadGameDisableSave),
    CALL(LG_HideButtons,                            C_LoadGameHideButtons),
    CALL(Sim_UnloadHouse,                           C_SimulateUnloadHouse),
    JMP_STUB(Quit_SaveDisabled,                     S_QuitSaveDisabled),
    CALL(PFI_ReadSku,                               C_PreFrameWorkInitReadSku),
    CALL_STUB(LP_VisitLine,                         S_LotPopupVisitLine),
    CALL(BFT_LodgingTip,                            C_FilterTipVisit),
    CALL(BFT_AnyLot,                                C_FilterAnyLot),
    WRAP(cWinNeighborhoodUL__DrawFilters,           W_DrawFilters, o_DrawFilters),
    CALL(SD_SaveFilter,                             C_SaveFilterNotVisit),
    CALL(VMLH_Zoning,                               C_ZoningVisitableAsCommunity),
    WRAP(cWinOptions__TSOnCommand,                  W_TSOnCommand, o_TSOnCommand),
    WRAP(cXPerson__TryFindBestAction,               W_TryFindBestAction, o_TryFindBestAction),
    WRAP(cXPortal__FindBestPortal,                  W_FindBestPortal, o_FindBestPortal),
    WRAP(LoadUIStrings,                             W_LoadUIStrings, o_LoadUIStrings),
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
    s_vaGetZoningType = Va(A::Neighborhood__GetZoningType);
    s_vaAifaAppend = Va(A::AIFA_Append);
    s_vaAifaSkip = Va(A::AIFA_Skip);
    s_vaIsSaveDisabled = Va(A::CPState__IsSaveDisabled);
    s_vaQuitAfterTest = Va(A::Quit_AfterTest);
    s_vaQuitTail = Va(A::Quit_Tail);
    const size_t nHooks = sizeof s_Hooks / sizeof s_Hooks[0];
    // every site must match its signature, or nothing is patched
    int nBad = 0;
    for (size_t i = 0; i < nHooks; ++i)
    {
        const GenSig *pSig = FindSig(s_Hooks[i].va);
        char why[160] = "no signature";
        if (!pSig || !api->SigCheck(&pSig->sig, why, sizeof why))
        {
            api->Log("[visitlots] %s @ 0x%06x: SIGNATURE MISMATCH (%s)", s_Hooks[i].pszName, s_Hooks[i].va, why);
            ++nBad;
        }
    }
    if (nBad)
    {
        api->Log("[visitlots] %d of %u hook sites do not match this Sims.exe: nothing patched, the mod is off", nBad, (unsigned)nHooks);
        return 2;
    }
    ScanOverlayIds();
    int nOk = 0, nFailed = 0;
    for (size_t i = 0; i < nHooks; ++i)
    {
        const Hook &h = s_Hooks[i];
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
                api->Log("[visitlots] %s @ 0x%06x: patch failed", h.pszName, h.va);
        }
        bOk ? ++nOk : ++nFailed;
    }
    api->Log("[visitlots] %d hooks installed, %d failed%s", nOk, nFailed, s_nOverlayIds ? "; UIText2.iff overlay on" : "");
    return nFailed ? 3 : 0;
}

BOOL WINAPI DllMain(HINSTANCE hinst, DWORD reason, LPVOID)
{
    if (reason == DLL_PROCESS_ATTACH)
        DisableThreadLibraryCalls(hinst);
    return TRUE;
}
