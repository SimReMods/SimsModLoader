// simsmod.h - the interface between the loader (winmm.dll) and a mod DLL (<exe dir>\mods\*.dll).
//
// The loader proxies winmm, waits until the game's code is ready,
// then loads every mods\*.dll and calls its exported `SimsMod_Init(const SimsLoaderApi *)`. Everything a mod needs
// from the loader comes through that table; the mod never touches the proxy.
#pragma once
#include <stddef.h>

struct OrigSig                  // first bytes of a function in Sims.exe
{
    unsigned int rva;           // relative to the link-time base 0x400000
    unsigned char len;          // number of valid bytes (<= 16)
    unsigned char bytes[16];
    unsigned short relocmask;   // bit i set: bytes[i..i+3] is a 32-bit absolute address (a .reloc fixup site)
};

#define SIMSMOD_API_VERSION 1

struct SimsLoaderApi
{
    unsigned int size;          // sizeof(SimsLoaderApi)
    unsigned int version;       // SIMSMOD_API_VERSION
    unsigned int exeBase;       // actual module base of Sims.exe (ASLR)
    unsigned int exeDelta;      // exeBase - 0x400000
    const char *exeDir;         // folder of Sims.exe (no trailing backslash)

    void (*Log)(const char *fmt, ...);                                          // one line in mod.log
    // `why` gets the first difference on a mismatch.
    bool (*SigCheck)(const OrigSig *sig, char *why, unsigned whyLen);
    // Signature-checked 5-byte jmp detour at sig->rva; *ppTrampoline = callable copy of the stolen prologue.
    bool (*Detour)(const char *name, const OrigSig *sig, void *pHook, void **ppTrampoline);
    // Raw code write (VirtualProtect + FlushInstructionCache) with every other thread suspended. For patching
    // code the mod owns; use Detour for the exe.
    bool (*WriteCode)(void *pDst, const void *pSrc, unsigned n);

    // newer loaders only: read it through SimsMod_GetGame
    unsigned int gameBuild;     // SIMSMOD_GAME_* the game is
};

// game builds (bits, also used by SimsMod_Games)
#define SIMSMOD_GAME_LEGACY     0x1u    // The Sims Legacy Collection 1.10.7.0 (Steam)
#define SIMSMOD_GAME_COMPLETE   0x2u    // The Sims Complete Collection (2003)
#define SIMSMOD_GAME_LEGACY_EA  0x4u    // The Sims Legacy Collection 1.10.7 (EA app): different exe layout from Steam

// The game the mod runs in (SIMSMOD_GAME_*). Loaders older than gameBuild only ever ran the Legacy Collection.
inline unsigned SimsMod_GetGame(const SimsLoaderApi *api)
{
    return api->size > offsetof(SimsLoaderApi, gameBuild) ? api->gameBuild : SIMSMOD_GAME_LEGACY;
}

// A mod exports this. Return 0 on success; anything else is logged and the mod is left loaded but inactive.
typedef int (__cdecl *PFN_SimsMod_Init)(const SimsLoaderApi *api);

// Optional export: `extern "C" __declspec(dllexport) const unsigned SimsMod_Games = SIMSMOD_GAME_...;` (bits).
// Without it a mod is for the Legacy Collection only; the loader skips mods that do not list the running game.
