// SeasonsHooks.h - the hook tables: each part of the mod lists its patch sites (seasons.cpp installs them all, after
// every signature matched).
#pragma once
#include <stddef.h>

enum Kind { kWrap, kCall, kJmpStub, kCallStub };
struct Hook
{
    const char *pszName;
    unsigned va;
    Kind kind;
    void *pFn;
    void **ppOrig;          // wrappers: receives the trampoline
};
struct HookTable { const Hook *p; size_t n; };

template <class T> using Id = T;
#define ORIG(name, ...) static Id<__VA_ARGS__> o_##name    // the trampoline of a wrapped function (set at install)

#define WRAP(site, fn, orig) { #site, A::site, kWrap, (void *)&fn, (void **)&orig }
#define CALL(site, fn) { #site, A::site, kCall, (void *)&fn, 0 }
#define JMP_STUB(site, fn) { #site, A::site, kJmpStub, (void *)&fn, 0 }
#define CALL_STUB(site, fn) { #site, A::site, kCallStub, (void *)&fn, 0 }

extern const Hook g_CareerHooks[];      // SeasonsCareers.cpp: the extra careers, their job panel art and portraits
extern const size_t g_nCareerHooks;
extern const Hook g_FolderHooks[];      // SeasonsFolder.cpp: the ExpansionPack8 objects folder
extern const size_t g_nFolderHooks;
extern const Hook g_PanelHooks[];       // SeasonsPanelIcon.cpp: the season icon in the control panel
extern const size_t g_nPanelHooks;
