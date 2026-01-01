// detour.h - x86 entry detour: a 5-byte jmp at the target plus a trampoline holding the relocated prologue.
#pragma once
#include "simsmod.h"

// Compares the live image with the signature. Reloc'd dwords are compared after subtracting the ASLR delta.
bool Sig_Check(const OrigSig &sig, char *why, unsigned whyLen);

// Patches the function at sig.rva (checked against sig first) to jump to pHook. *ppTrampoline receives a callable
// copy of the stolen prologue that continues in the original. Logs what it did. Returns false (nothing patched) on
// a signature mismatch or an undecodable prologue.
bool Detour_Install(const char *pszName, const OrigSig &sig, void *pHook, void **ppTrampoline);

// Writes n bytes of code (VirtualProtect, FlushInstructionCache) with every other thread suspended.
bool Detour_WriteCode(void *pDst, const void *pSrc, unsigned n);

// C-ABI wrappers for the SimsLoaderApi table
bool Api_SigCheck(const OrigSig *sig, char *why, unsigned whyLen);
bool Api_Detour(const char *name, const OrigSig *sig, void *pHook, void **ppTrampoline);
