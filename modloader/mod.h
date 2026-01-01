// mod.h - the loader's life cycle, driven by the proxy exports.
#pragma once

void Loader_OnAttach(void *hThisDll);                // DllMain(DLL_PROCESS_ATTACH): log, module base, .text range
void Loader_OnProxyCall(const void *pRetAddr, const char *pszExport);   // every proxied export calls this first
void Loader_OnDetach();
