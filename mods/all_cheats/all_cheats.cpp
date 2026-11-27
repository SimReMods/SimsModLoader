// all_cheats.cpp - mods\all_cheats_*.dll: every cheat the game registers is available in the cheat window (Ctrl+Shift+C),
// not only the short list the game allows (rosebud, move_objects, set_hour, map_edit, ...).
//
// When a cheat is registered (cTSCheatCodeManager::RegisterCheat), the manager asks AllowCheat(name) and drops the cheat
// when the answer is no; by default only 28 names pass. The mod lets every name pass (Complete Collection: wraps
// AllowCheat; Legacy: AllowCheat is inlined into RegisterCheat, so it wraps RegisterCheat), except the few developer
// commands in s_apszKeepBlocked, which only crash the game or rewrite its career files; those stay unavailable. Nothing else changes: the cheats themselves are the game's code.
#include <windows.h>
#include <string.h>
#include "simsmod.h"

#ifndef GAME_CC
#define GAME_CC 0
#endif
#ifndef GAME_EA
#define GAME_EA 0
#endif

namespace A
{
    enum : unsigned
    {
#define GA_FN(n, va) n = va,
#if GAME_CC
#include "game_addr_cc.h"
#elif GAME_EA
#include "game_addr_ea.h"
#else
#include "game_addr.h"
#endif
#undef GA_FN
    };
}

#if GAME_CC
#include "gen_sigs_cc.h"
#elif GAME_EA
#include "gen_sigs_ea.h"
#else
#include "gen_sigs.h"
#endif

extern "C" __declspec(dllexport) const unsigned SimsMod_Games =
    GAME_CC ? SIMSMOD_GAME_COMPLETE : GAME_EA ? SIMSMOD_GAME_LEGACY_EA : SIMSMOD_GAME_LEGACY;

static const SimsLoaderApi *s_api;

// developer commands that stay as the game has them: they crash the game on purpose or overwrite the career files
static const char *const s_apszKeepBlocked[] = { "crash", "core_dump", "assert", "save_careers", "save_careers_txt" };

static bool KeepBlocked(const char *pszName)
{
    for (const char *psz : s_apszKeepBlocked)
        if (pszName && !_stricmp(pszName, psz))
            return true;
    return false;
}

static bool(__thiscall *o_AllowCheat)(void *, const char *);

#if GAME_CC
// cTSCheatCodeManager::AllowCheat(const char *): yes for every name but the few above
static bool __fastcall W_AllowCheat(void *pMgr, void *, const char *pszName)
{
    bool bGame = o_AllowCheat(pMgr, pszName);
    if (bGame || KeepBlocked(pszName))
        return bGame;
    if (s_api)
        s_api->Log("[all_cheats] unlocked %s", pszName);
    return true;
}
#else
// Legacy: RegisterCheat has the AllowCheat test inlined (the manager's allow-all flag at +0x4C, then the short list), so
// the mod wraps RegisterCheat and sets the flag for the call; the few names above are dropped as the game drops them
typedef void *TSMsgParam;
static void(__thiscall *o_RegisterCheat)(void *, const char *, void *, TSMsgParam, const char *);
static void __fastcall W_RegisterCheat(void *pMgr, void *, const char *pszName, void *pfn, TSMsgParam nUser,
                                       const char *pszUsage)
{
    if (KeepBlocked(pszName))
        return;
    bool &bAllowAll = *(bool *)((char *)pMgr + 0x4C);
    if (!bAllowAll && pszName && !o_AllowCheat(pMgr, pszName) && s_api)
        s_api->Log("[all_cheats] unlocked %s", pszName);
    bool bWas = bAllowAll;
    bAllowAll = true;
    o_RegisterCheat(pMgr, pszName, pfn, nUser, pszUsage);
    bAllowAll = bWas;
}
#endif

static const GenSig *FindSig(unsigned va)
{
    for (const GenSig &s : g_GenSigs)
        if (s.va == va)
            return &s;
    return 0;
}

extern "C" __declspec(dllexport) int __cdecl SimsMod_Init(const SimsLoaderApi *api)
{
    if (api->version != SIMSMOD_API_VERSION)
        return 1;
    s_api = api;
#if GAME_CC
    const unsigned va = A::cTSCheatCodeManager__AllowCheat;
    const char *pszFn = "cTSCheatCodeManager__AllowCheat";
#else
    const unsigned va = A::cTSCheatCodeManager__RegisterCheat;
    const char *pszFn = "cTSCheatCodeManager__RegisterCheat";
#endif
    const GenSig *pSig = FindSig(va);
    char why[160] = "no signature";
    if (!pSig || !api->SigCheck(&pSig->sig, why, sizeof why))
    {
        api->Log("[all_cheats] %s @ 0x%06x: SIGNATURE MISMATCH (%s): nothing patched", pszFn, va, why);
        return 2;
    }
#if GAME_CC
    bool bOk = api->Detour(pszFn, &pSig->sig, (void *)&W_AllowCheat, (void **)&o_AllowCheat) && o_AllowCheat;
#else
    o_AllowCheat = (bool(__thiscall *)(void *, const char *))(uintptr_t)(A::cTSCheatCodeManager__AllowCheat - 0x400000 +
                                                                         (uintptr_t)GetModuleHandleA(0));
    bool bOk = api->Detour(pszFn, &pSig->sig, (void *)&W_RegisterCheat, (void **)&o_RegisterCheat) && o_RegisterCheat;
#endif
    if (!bOk)
    {
        api->Log("[all_cheats] hook failed");
        return 3;
    }
    api->Log("[all_cheats] all cheats unlocked (except crash, core_dump, assert, save_careers, save_careers_txt)");
    return 0;
}

BOOL WINAPI DllMain(HINSTANCE hinst, DWORD reason, LPVOID)
{
    if (reason == DLL_PROCESS_ATTACH)
        DisableThreadLibraryCalls(hinst);
    return TRUE;
}
