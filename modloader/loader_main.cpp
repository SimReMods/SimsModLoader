// loader_main.cpp - module base, decrypt detection, and loading of the mod DLLs.
//
// .text is still encrypted by the Steam stub when DllMain runs, so nothing is patched at attach. The first proxied
// export called from a return address inside .text comes from decrypted game code; the OEP bytes are then verified
// against the reference build before any mod loads.
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include "mod.h"
#include "log.h"
#include "detour.h"
#include "exe.h"
#include "simsmod.h"
#include "exe_identity.h"

namespace orig { unsigned int base = 0x400000; unsigned int delta = 0; }

static unsigned char *s_pText = 0;
static unsigned s_nText = 0;
static volatile LONG s_nState = 0;          // 0 = waiting, 1 = initialising, 2 = done, 3 = given up
static volatile LONG s_nCallsBefore = 0;    // proxied calls seen before init (from the stub or other DLLs)
static int s_nFailedChecks = 0;
static char s_szExeDir[MAX_PATH];
static SimsLoaderApi s_api;

static void FindTextSection()
{
    HMODULE hExe = GetModuleHandleA(0);
    orig::base = (unsigned)(UINT_PTR)hExe;
    orig::delta = orig::base - 0x400000;
    const IMAGE_DOS_HEADER *dos = (const IMAGE_DOS_HEADER *)hExe;
    const IMAGE_NT_HEADERS *nt = (const IMAGE_NT_HEADERS *)((const char *)hExe + dos->e_lfanew);
    const IMAGE_SECTION_HEADER *sec = IMAGE_FIRST_SECTION(nt);
    LogV("exe %p: link-time base 0x400000 (in-memory header ImageBase 0x%08x), actual 0x%08x, ASLR delta 0x%08x, DllCharacteristics 0x%04x, entry rva 0x%06x, %u sections",
        hExe, nt->OptionalHeader.ImageBase, orig::base, orig::delta, nt->OptionalHeader.DllCharacteristics,
        nt->OptionalHeader.AddressOfEntryPoint, nt->FileHeader.NumberOfSections);
    for (unsigned i = 0; i < nt->FileHeader.NumberOfSections; ++i)
    {
        char name[9]; memcpy(name, sec[i].Name, 8); name[8] = 0;
        LogV("  section %-8s rva 0x%06x size 0x%06x chars 0x%08x", name, sec[i].VirtualAddress, sec[i].Misc.VirtualSize, sec[i].Characteristics);
        if (!strcmp(name, ".text"))
        {
            s_pText = orig::ptr<unsigned char>(sec[i].VirtualAddress);
            s_nText = sec[i].Misc.VirtualSize;
        }
    }
    if (!s_pText)
    {
        s_pText = orig::ptr<unsigned char>(ORIG_TEXT_RVA); s_nText = ORIG_TEXT_SIZE;
        Log("  no .text section header: using the decrypted exe's range");
    }
}

static const char *ModuleNameOf(const void *p, char *buf, unsigned len)
{
    HMODULE h = 0;
    if (GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCSTR)p, &h) && h)
    {
        char path[MAX_PATH];
        GetModuleFileNameA(h, path, MAX_PATH);
        const char *s = strrchr(path, '\\');
        _snprintf_s(buf, len, _TRUNCATE, "%s+0x%x", s ? s + 1 : path, (unsigned)((const char *)p - (const char *)h));
    }
    else
        _snprintf_s(buf, len, _TRUNCATE, "<no module>");
    return buf;
}

// Exe identity checks: PE header fields at attach, then an FNV-1a 64 hash of the decrypted .text (reloc'd dwords
// zeroed, so ASLR does not matter). Either mismatch: message box, no mods, the game runs unmodified.
static bool s_bWrongExe = false;

static void WrongExe(const char *pszWhat)
{
    s_bWrongExe = true;
    InterlockedExchange(&s_nState, 3);
    Log("EXE CHECK FAILED: %s -> mods will NOT be loaded; the game runs unmodified", pszWhat);
    char msg[700];
    _snprintf_s(msg, sizeof msg, _TRUNCATE,
                "This Sims.exe is not the build the installed mod was made for.\n\n%s\n\n"
                "The mod has been disabled for this run; the game starts unmodified.\n"
                "Remove winmm.dll and the mods folder from the game directory, or install a mod build that matches this game version.",
                pszWhat);
    MessageBoxA(0, msg, "Sims mod loader", MB_OK | MB_ICONWARNING | MB_TOPMOST | MB_SETFOREGROUND);
}

static bool CheckExeIdentity()
{
    const IMAGE_DOS_HEADER *dos = (const IMAGE_DOS_HEADER *)(UINT_PTR)orig::base;
    const IMAGE_NT_HEADERS *nt = (const IMAGE_NT_HEADERS *)((const char *)dos + dos->e_lfanew);
    unsigned ts = nt->FileHeader.TimeDateStamp, cs = nt->OptionalHeader.CheckSum, si = nt->OptionalHeader.SizeOfImage;
    Log("exe identity: TimeDateStamp 0x%08x CheckSum 0x%08x SizeOfImage 0x%08x (expected 0x%08x / 0x%08x / 0x%08x)",
        ts, cs, si, ORIG_PACKED_TIMESTAMP, ORIG_PACKED_CHECKSUM, ORIG_PACKED_SIZEOFIMAGE);
    if (ts == ORIG_PACKED_TIMESTAMP && cs == ORIG_PACKED_CHECKSUM && si == ORIG_PACKED_SIZEOFIMAGE && !GetEnvironmentVariableA("SIMSMOD_FAKE_UPDATE", 0, 0))
        return true;
    char why[300];
    _snprintf_s(why, sizeof why, _TRUNCATE, "PE header: TimeDateStamp 0x%08x, CheckSum 0x%08x, SizeOfImage 0x%08x\nexpected:  TimeDateStamp 0x%08x, CheckSum 0x%08x, SizeOfImage 0x%08x%s",
                ts, cs, si, ORIG_PACKED_TIMESTAMP, ORIG_PACKED_CHECKSUM, ORIG_PACKED_SIZEOFIMAGE,
                GetEnvironmentVariableA("SIMSMOD_FAKE_UPDATE", 0, 0) ? "\n(SIMSMOD_FAKE_UPDATE is set: simulated mismatch)" : "");
    WrongExe(why);
    return false;
}

static unsigned long long TextHash()
{
    // mask: .text bytes that belong to a relocated dword (from the image's .reloc section)
    const IMAGE_DOS_HEADER *dos = (const IMAGE_DOS_HEADER *)(UINT_PTR)orig::base;
    const IMAGE_NT_HEADERS *nt = (const IMAGE_NT_HEADERS *)((const char *)dos + dos->e_lfanew);
    const IMAGE_DATA_DIRECTORY &rd = nt->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC];
    unsigned textRva = (unsigned)(s_pText - (unsigned char *)(UINT_PTR)orig::base);
    unsigned char *mask = (unsigned char *)VirtualAlloc(0, s_nText, MEM_COMMIT, PAGE_READWRITE);
    if (!mask) return 0;
    const unsigned char *rel = orig::ptr<unsigned char>(rd.VirtualAddress), *end = rel + rd.Size;
    while (rel + 8 <= end)
    {
        unsigned page = *(const unsigned *)rel, bsz = *(const unsigned *)(rel + 4);
        if (bsz < 8) break;
        for (unsigned k = 8; k + 2 <= bsz; k += 2)
        {
            unsigned short e = *(const unsigned short *)(rel + k);
            if ((e >> 12) != IMAGE_REL_BASED_HIGHLOW) continue;
            unsigned r = page + (e & 0xfff);
            if (r >= textRva && r + 4 <= textRva + s_nText)
                for (unsigned j = 0; j < 4; ++j) mask[r - textRva + j] = 1;
        }
        rel += bsz;
    }
    unsigned long long h = 0xcbf29ce484222325ull;
    for (unsigned i = 0; i < s_nText; ++i)
    {
        unsigned char x = mask[i] ? 0 : s_pText[i];
        h = (h ^ x) * 0x100000001b3ull;
    }
    VirtualFree(mask, 0, MEM_RELEASE);
    return h;
}

static bool CheckTextHash()
{
    DWORD t0 = GetTickCount();
    unsigned long long h = TextHash();
    Log("decrypted .text FNV-1a 64 (reloc sites zeroed): 0x%016llx, expected 0x%016llx (%lu ms)", h, (unsigned long long)ORIG_TEXT_FNV64, GetTickCount() - t0);
    if (h == ORIG_TEXT_FNV64) return true;
    char why[200];
    _snprintf_s(why, sizeof why, _TRUNCATE, "code hash of the decrypted game: 0x%016llx\nexpected:                        0x%016llx", h, (unsigned long long)ORIG_TEXT_FNV64);
    WrongExe(why);
    return false;
}

void Loader_OnAttach(void *hThisDll)
{
    GetModuleFileNameA(0, s_szExeDir, MAX_PATH);
    char *slash = strrchr(s_szExeDir, '\\');
    // Other programs started from the game folder (e.g. crashpad_handler.exe) load this winmm.dll too: plain proxy.
    if (_stricmp(slash ? slash + 1 : s_szExeDir, "Sims.exe") != 0)
    {
        InterlockedExchange(&s_nState, 3);
        return;
    }
    if (slash) *slash = 0;
    Log_Init(s_szExeDir);
    char self[MAX_PATH];
    GetModuleFileNameA((HMODULE)hThisDll, self, MAX_PATH);
    Log("==== loader attached: %s (pid %lu)", self, GetCurrentProcessId());
    FindTextSection();
    char why[160];
    bool oepOk = Sig_Check(ORIG_SIG_7a8a86, why, sizeof why);
    LogV("at attach: .text %p..%p, OEP bytes %s (%s)", s_pText, s_pText + s_nText, oepOk ? "ALREADY DECRYPTED" : "still packed", oepOk ? "-" : why);
    CheckExeIdentity();
}

static void LoadMods()
{
    s_api.size = sizeof s_api; s_api.version = SIMSMOD_API_VERSION;
    s_api.exeBase = orig::base; s_api.exeDelta = orig::delta; s_api.exeDir = s_szExeDir;
    s_api.Log = Log; s_api.SigCheck = Api_SigCheck; s_api.Detour = Api_Detour; s_api.WriteCode = Detour_WriteCode;

    char pattern[MAX_PATH];
    _snprintf_s(pattern, sizeof pattern, _TRUNCATE, "%s\\mods\\*.dll", s_szExeDir);
    WIN32_FIND_DATAA fd;
    HANDLE hFind = FindFirstFileA(pattern, &fd);
    if (hFind == INVALID_HANDLE_VALUE) { Log("no mods: %s not found", pattern); return; }
    // sorted by name for a deterministic load order
    char names[32][MAX_PATH]; int n = 0;
    do { if (n < 32) lstrcpynA(names[n++], fd.cFileName, MAX_PATH); } while (FindNextFileA(hFind, &fd));
    FindClose(hFind);
    for (int i = 1; i < n; ++i)
        for (int j = i; j > 0 && _stricmp(names[j - 1], names[j]) > 0; --j)
        { char t[MAX_PATH]; lstrcpynA(t, names[j], MAX_PATH); lstrcpynA(names[j], names[j - 1], MAX_PATH); lstrcpynA(names[j - 1], t, MAX_PATH); }
    for (int i = 0; i < n; ++i)
    {
        char path[MAX_PATH];
        _snprintf_s(path, sizeof path, _TRUNCATE, "%s\\mods\\%s", s_szExeDir, names[i]);
        HMODULE h = LoadLibraryA(path);
        if (!h) { Log("mod %s: LoadLibrary failed (%lu)", names[i], GetLastError()); continue; }
        PFN_SimsMod_Init init = (PFN_SimsMod_Init)GetProcAddress(h, "SimsMod_Init");
        if (!init) { Log("mod %s @ %p: no SimsMod_Init export, ignored", names[i], h); continue; }
        LogV("mod %s @ %p: SimsMod_Init ...", names[i], h);
        int r = init(&s_api);
        Log("mod %s: SimsMod_Init returned %d%s", names[i], r, r ? " (INACTIVE)" : "");
    }
}

static void Initialise(const void *pRetAddr, const char *pszExport)
{
    char who[80];
    LogV("first call into %s from game .text (return address %p = %s) after %ld proxied calls from elsewhere",
        pszExport, pRetAddr, ModuleNameOf(pRetAddr, who, sizeof who), s_nCallsBefore);
    char why[160];
    if (!Sig_Check(ORIG_SIG_7a8a86, why, sizeof why))
    {
        Log("decrypt check: OEP bytes do not match the reference build (%s): NOT loading mods", why);
        if (++s_nFailedChecks >= 50) { InterlockedExchange(&s_nState, 3); Log("giving up after %d failed checks", s_nFailedChecks); }
        else InterlockedExchange(&s_nState, 0);
        return;
    }
    LogV("decrypt check: OEP bytes match the decrypted exe -> .text is decrypted; method = return-address-in-.text + byte verification");
    if (!CheckTextHash()) return;              // state is 3 now: never retried
    LoadMods();
    InterlockedExchange(&s_nState, 2);
    LogV("loader initialisation done");
}

void Loader_OnProxyCall(const void *pRetAddr, const char *pszExport)
{
    if (s_nState >= 2) return;
    const unsigned char *ra = (const unsigned char *)pRetAddr;
    if (ra < s_pText || ra >= s_pText + s_nText)
    {
        LONG n = InterlockedIncrement(&s_nCallsBefore);
        if (n <= 5)
        {
            char who[80];
            LogV("proxied %s called from outside game .text: %p = %s (ignored)", pszExport, pRetAddr, ModuleNameOf(pRetAddr, who, sizeof who));
        }
        return;
    }
    if (InterlockedCompareExchange(&s_nState, 1, 0) != 0) return;
    Initialise(pRetAddr, pszExport);
}

void Loader_OnDetach()
{
    Log("==== loader detached (process exit)");
}
