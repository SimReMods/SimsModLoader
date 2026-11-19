// simsmod.h - the interface between the loader (winmm.dll) and a mod DLL (<exe dir>\mods\*.dll).
//
// The loader proxies winmm, waits until the Steam stub has decrypted .text (verified against the decrypted exe),
// then loads every mods\*.dll and calls its exported `SimsMod_Init(const SimsLoaderApi *)`. Everything a mod needs
// from the loader comes through that table; the mod never touches the proxy or the decrypt logic.
#pragma once

struct OrigSig                  // first bytes of a function in Sims.exe.decrypted.exe
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
};

// A mod exports this. Return 0 on success; anything else is logged and the mod is left loaded but inactive.
typedef int (__cdecl *PFN_SimsMod_Init)(const SimsLoaderApi *api);
