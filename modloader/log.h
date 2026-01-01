// log.h - mod.log next to the exe
#pragma once

void Log_Init(const char *pszDirOfExe);
void Log(const char *fmt, ...);
// verbose lines (per hook, per section, timing): only the dev flavour (SIMSMOD_DEV) writes them
void LogV(const char *fmt, ...);
