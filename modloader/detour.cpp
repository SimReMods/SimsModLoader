// detour.cpp - signature check + 5-byte jmp detour with a relocated-prologue trampoline.
#include <windows.h>
#include <tlhelp32.h>
#include <stdio.h>
#include <string.h>
#include "detour.h"
#include "x86len.h"
#include "log.h"
#include "exe.h"

bool Sig_Check(const OrigSig &sig, char *why, unsigned whyLen)
{
    const unsigned char *p = orig::ptr<unsigned char>(sig.rva);
    for (unsigned i = 0; i < sig.len;)
    {
        if (sig.relocmask & (1u << i))
        {
            unsigned live, want;
            memcpy(&live, p + i, 4);
            memcpy(&want, sig.bytes + i, 4);
            if (live - orig::delta != want)
            {
                _snprintf_s(why, whyLen, _TRUNCATE, "reloc dword at +%u: live 0x%08x (-delta 0x%08x) != 0x%08x", i, live, live - orig::delta, want);
                return false;
            }
            i += 4;
        }
        else
        {
            if (p[i] != sig.bytes[i])
            {
                _snprintf_s(why, whyLen, _TRUNCATE, "byte at +%u: live 0x%02x != 0x%02x", i, p[i], sig.bytes[i]);
                return false;
            }
            ++i;
        }
    }
    return true;
}

// Trampoline arena: one RWX page, bump allocated.
static unsigned char *s_pArena = 0;
static unsigned s_nArenaUsed = 0;
enum { kTrampSize = 48 };

static unsigned char *AllocTrampoline()
{
    if (!s_pArena)
    {
        s_pArena = (unsigned char *)VirtualAlloc(0, 4096, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
        if (!s_pArena) return 0;
        memset(s_pArena, 0xCC, 4096);
    }
    if (s_nArenaUsed + kTrampSize > 4096) return 0;
    unsigned char *p = s_pArena + s_nArenaUsed;
    s_nArenaUsed += kTrampSize;
    return p;
}

// Suspends all other threads while code is written. The snapshot (allocates) is taken first and the handles kept,
// because nothing that may take the heap lock can be called once another thread is frozen.
struct Frozen { HANDLE h[256]; unsigned n; };

static void FreezeOthers(Frozen &f)
{
    f.n = 0;
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
    if (hSnap == INVALID_HANDLE_VALUE) return;
    THREADENTRY32 te; te.dwSize = sizeof te;
    DWORD pid = GetCurrentProcessId(), tid = GetCurrentThreadId();
    for (BOOL ok = Thread32First(hSnap, &te); ok && f.n < 256; ok = Thread32Next(hSnap, &te))
    {
        if (te.th32OwnerProcessID != pid || te.th32ThreadID == tid) continue;
        HANDLE h = OpenThread(THREAD_SUSPEND_RESUME, FALSE, te.th32ThreadID);
        if (h) f.h[f.n++] = h;
    }
    CloseHandle(hSnap);
    for (unsigned i = 0; i < f.n; ++i) SuspendThread(f.h[i]);
}

static void ThawOthers(Frozen &f)
{
    for (unsigned i = 0; i < f.n; ++i) { ResumeThread(f.h[i]); CloseHandle(f.h[i]); }
    f.n = 0;
}

static bool WriteCode(void *pDst, const void *pSrc, unsigned n)
{
    DWORD old;
    if (!VirtualProtect(pDst, n, PAGE_EXECUTE_READWRITE, &old)) return false;
    memcpy(pDst, pSrc, n);
    DWORD tmp;
    VirtualProtect(pDst, n, old, &tmp);
    FlushInstructionCache(GetCurrentProcess(), pDst, n);
    return true;
}

bool Detour_WriteCode(void *pDst, const void *pSrc, unsigned n)
{
    Frozen f; FreezeOthers(f);
    bool ok = WriteCode(pDst, pSrc, n);
    ThawOthers(f);
    return ok;
}

bool Api_SigCheck(const OrigSig *sig, char *why, unsigned whyLen) { return Sig_Check(*sig, why, whyLen); }
bool Api_Detour(const char *name, const OrigSig *sig, void *pHook, void **ppTrampoline) { return Detour_Install(name, *sig, pHook, ppTrampoline); }

bool Detour_Install(const char *pszName, const OrigSig &sig, void *pHook, void **ppTrampoline)
{
    char why[160];
    unsigned char *pTarget = orig::ptr<unsigned char>(sig.rva);
    if (ppTrampoline) *ppTrampoline = 0;
    if (!Sig_Check(sig, why, sizeof why))
    {
        Log("hook %s @ %p (rva 0x%06x): SIGNATURE MISMATCH, not patched: %s", pszName, pTarget, sig.rva, why);
        return false;
    }
    unsigned stolen = 0;
    X86Insn insns[8]; int nInsn = 0;
    while (stolen < 5 && nInsn < 8)
    {
        X86Insn &in = insns[nInsn];
        if (!X86_Decode(pTarget + stolen, in))
        {
            Log("hook %s @ %p: cannot decode instruction at +%u (opcode %02x %02x), not patched", pszName, pTarget, stolen, pTarget[stolen], pTarget[stolen + 1]);
            return false;
        }
        if (in.isLoop)
        {
            Log("hook %s @ %p: loop/jecxz in prologue at +%u, not patched", pszName, pTarget, stolen);
            return false;
        }
        stolen += in.len; ++nInsn;
        if (in.isRet || in.isJmp) break;
    }
    if (stolen < 5)
    {
        Log("hook %s @ %p: prologue ends (ret/jmp) after %u bytes, not patched", pszName, pTarget, stolen);
        return false;
    }
    if (stolen > 16)
    {
        Log("hook %s @ %p: stolen prologue too long (%u), not patched", pszName, pTarget, stolen);
        return false;
    }
    unsigned char *pTramp = AllocTrampoline();
    if (!pTramp) { Log("hook %s: trampoline arena full", pszName); return false; }

    // rel8 jmp/jcc are widened to rel32; rel32 branches are re-targeted
    unsigned src = 0, dst = 0;
    char dis[200]; dis[0] = 0;
    for (int k = 0; k < nInsn; ++k)
    {
        const X86Insn &in = insns[k];
        const unsigned char *pi = pTarget + src;
        if (in.relSize == 4)
        {
            memcpy(pTramp + dst, pi, in.len);
            int rel; memcpy(&rel, pi + in.relPos, 4);
            unsigned char *dest = (unsigned char *)(pi + in.len + rel);
            int nrel = (int)(dest - (pTramp + dst + in.len));
            memcpy(pTramp + dst + in.relPos, &nrel, 4);
            dst += in.len;
        }
        else if (in.relSize == 1)
        {
            signed char rel = (signed char)pi[in.relPos];
            unsigned char *dest = (unsigned char *)(pi + in.len + rel);
            unsigned char op = pi[in.relPos - 1];
            if (op == 0xEB) { pTramp[dst] = 0xE9; int nrel = (int)(dest - (pTramp + dst + 5)); memcpy(pTramp + dst + 1, &nrel, 4); dst += 5; }
            else if (op >= 0x70 && op <= 0x7F) { pTramp[dst] = 0x0F; pTramp[dst + 1] = (unsigned char)(0x80 | (op & 0x0F)); int nrel = (int)(dest - (pTramp + dst + 6)); memcpy(pTramp + dst + 2, &nrel, 4); dst += 6; }
            else { Log("hook %s: unexpected rel8 opcode %02x, not patched", pszName, op); return false; }
        }
        else
        {
            memcpy(pTramp + dst, pi, in.len);
            dst += in.len;
        }
        char one[48]; int o = 0;
        for (unsigned b = 0; b < in.len && o < 40; ++b) o += _snprintf_s(one + o, sizeof one - o, _TRUNCATE, "%02x", pi[b]);
        _snprintf_s(dis + strlen(dis), sizeof dis - strlen(dis), _TRUNCATE, "%s%s", k ? " | " : "", one);
        src += in.len;
    }
    if (!(insns[nInsn - 1].isRet || insns[nInsn - 1].isJmp))
    {
        pTramp[dst] = 0xE9;
        int nrel = (int)((pTarget + stolen) - (pTramp + dst + 5));
        memcpy(pTramp + dst + 1, &nrel, 4);
        dst += 5;
    }

    unsigned char patch[16];
    memset(patch, 0x90, sizeof patch);
    patch[0] = 0xE9;
    int rel = (int)((unsigned char *)pHook - (pTarget + 5));
    memcpy(patch + 1, &rel, 4);
    Frozen f; FreezeOthers(f);
    bool ok = WriteCode(pTarget, patch, stolen);
    ThawOthers(f);
    if (!ok)
    {
        Log("hook %s @ %p: VirtualProtect failed (%lu), not patched", pszName, pTarget, GetLastError());
        return false;
    }
    if (ppTrampoline) *ppTrampoline = pTramp;
    LogV("hook %s @ %p (rva 0x%06x) -> %p, trampoline %p, stolen %u bytes [%s]", pszName, pTarget, sig.rva, pHook, pTramp, stolen, dis);
    return true;
}
