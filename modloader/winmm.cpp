// winmm.cpp - proxy winmm.dll. Sims.exe imports only timeGetTime, timeBeginPeriod, timeEndPeriod, timeSetEvent and
// timeKillEvent; each reports its return address to the loader, then forwards to the real winmm.dll (system directory).
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <intrin.h>
#include "mod.h"
#include "log.h"

typedef DWORD (WINAPI *PFN_timeGetTime)(void);
typedef UINT (WINAPI *PFN_timePeriod)(UINT);
typedef UINT (WINAPI *PFN_timeSetEvent)(UINT, UINT, void *, DWORD_PTR, UINT);
typedef UINT (WINAPI *PFN_timeKillEvent)(UINT);

static HMODULE s_hReal = 0;
static PFN_timeGetTime s_timeGetTime;
static PFN_timePeriod s_timeBeginPeriod, s_timeEndPeriod;
static PFN_timeSetEvent s_timeSetEvent;
static PFN_timeKillEvent s_timeKillEvent;

// winmm_exports.cpp (generated): naked thunks for every export of the real DLL
extern "C" void *g_winmm_real[];
extern const char *const g_winmm_names[];
extern const unsigned short g_winmm_ordinals[];
extern const unsigned int g_winmm_count;

static void LoadReal()
{
    if (s_hReal) return;
    char path[MAX_PATH];
    UINT n = GetSystemDirectoryA(path, MAX_PATH);
    lstrcpyA(path + n, "\\winmm.dll");
    HMODULE h = LoadLibraryA(path);
    if (!h)
    {
        Log("FATAL: cannot load the real %s (error %lu)", path, GetLastError());
        return;
    }
    unsigned missing = 0;
    for (unsigned i = 0; i < g_winmm_count; ++i)
    {
        g_winmm_real[i] = (void *)GetProcAddress(h, g_winmm_names[i][0] ? g_winmm_names[i] : (LPCSTR)(UINT_PTR)g_winmm_ordinals[i]);
        if (!g_winmm_real[i]) ++missing;
    }
    Log("real winmm: %u exports resolved, %u missing", g_winmm_count - missing, missing);
    s_timeGetTime = (PFN_timeGetTime)GetProcAddress(h, "timeGetTime");
    s_timeBeginPeriod = (PFN_timePeriod)GetProcAddress(h, "timeBeginPeriod");
    s_timeEndPeriod = (PFN_timePeriod)GetProcAddress(h, "timeEndPeriod");
    s_timeSetEvent = (PFN_timeSetEvent)GetProcAddress(h, "timeSetEvent");
    s_timeKillEvent = (PFN_timeKillEvent)GetProcAddress(h, "timeKillEvent");
    Log("real winmm loaded: %s @ %p (timeGetTime %p)", path, h, s_timeGetTime);
    s_hReal = h;
}

extern "C" {

DWORD WINAPI timeGetTime(void)
{
    Loader_OnProxyCall(_ReturnAddress(), "timeGetTime");
    return s_timeGetTime();
}

UINT WINAPI timeBeginPeriod(UINT uPeriod)
{
    Loader_OnProxyCall(_ReturnAddress(), "timeBeginPeriod");
    return s_timeBeginPeriod(uPeriod);
}

UINT WINAPI timeEndPeriod(UINT uPeriod)
{
    Loader_OnProxyCall(_ReturnAddress(), "timeEndPeriod");
    return s_timeEndPeriod(uPeriod);
}

UINT WINAPI timeSetEvent(UINT uDelay, UINT uResolution, void *fptc, DWORD_PTR dwUser, UINT fuEvent)
{
    Loader_OnProxyCall(_ReturnAddress(), "timeSetEvent");
    return s_timeSetEvent(uDelay, uResolution, fptc, dwUser, fuEvent);
}

UINT WINAPI timeKillEvent(UINT uTimerID)
{
    Loader_OnProxyCall(_ReturnAddress(), "timeKillEvent");
    return s_timeKillEvent(uTimerID);
}

} // extern "C"

BOOL WINAPI DllMain(HINSTANCE hinst, DWORD reason, LPVOID)
{
    if (reason == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(hinst);
        Loader_OnAttach(hinst);
        LoadReal();
    }
    else if (reason == DLL_PROCESS_DETACH)
        Loader_OnDetach();
    return TRUE;
}
