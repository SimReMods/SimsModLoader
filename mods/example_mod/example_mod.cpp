// example_mod.cpp - the smallest possible mod: a message box when the loader starts it.
#include <windows.h>
#include <stdio.h>
#include "simsmod.h"

extern "C" __declspec(dllexport) int __cdecl SimsMod_Init(const SimsLoaderApi *api)
{
    if (api->version != SIMSMOD_API_VERSION)
        return 1;
    api->Log("[example_mod] loaded; game base 0x%08x, game folder %s", api->exeBase, api->exeDir);
    char msg[300];
    _snprintf_s(msg, sizeof msg, _TRUNCATE, "example_mod is running inside the game.\n\nexe base 0x%08x\nfolder %s", api->exeBase, api->exeDir);
    MessageBoxA(0, msg, "example_mod", MB_OK | MB_ICONINFORMATION | MB_TOPMOST);
    return 0;
}

BOOL WINAPI DllMain(HINSTANCE hinst, DWORD reason, LPVOID)
{
    if (reason == DLL_PROCESS_ATTACH)
        DisableThreadLibraryCalls(hinst);
    return TRUE;
}
