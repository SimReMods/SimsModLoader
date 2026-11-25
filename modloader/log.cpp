// log.cpp - mod.log next to the exe (or in Saved Games\Electronic Arts\The Sims 25 when that folder is not writable).
// Opened once (append), every line flushed: the process may die at any point.
#include <windows.h>
#include <shlobj.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "log.h"

static HANDLE s_hLog = INVALID_HANDLE_VALUE;
static CRITICAL_SECTION s_cs;
static DWORD s_t0;

static HANDLE OpenLog(const char *pszDir, char *path, unsigned len)
{
    _snprintf_s(path, len, _TRUNCATE, "%s\\mod.log", pszDir);
    return CreateFileA(path, FILE_APPEND_DATA, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, 0, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
}

void Log_Init(const char *pszDir)
{
    InitializeCriticalSection(&s_cs);
    s_t0 = GetTickCount();
    char path[MAX_PATH];
    s_hLog = OpenLog(pszDir, path, sizeof path);
    if (s_hLog != INVALID_HANDLE_VALUE)
        return;
    // The game folder is read-only for the game when it lives in Program Files (EA app install, not run as admin):
    // fall back to the game's save folder, then to %TEMP%.
    DWORD err = GetLastError();
    char dir[MAX_PATH];
    PWSTR pszSaved = 0;
    if (SUCCEEDED(SHGetKnownFolderPath(FOLDERID_SavedGames, KF_FLAG_CREATE, 0, &pszSaved)))
    {
        _snprintf_s(dir, sizeof dir, _TRUNCATE, "%ls\\Electronic Arts", pszSaved);
        CreateDirectoryA(dir, 0);
        _snprintf_s(dir, sizeof dir, _TRUNCATE, "%ls\\Electronic Arts\\The Sims 25", pszSaved);
        CreateDirectoryA(dir, 0);
        s_hLog = OpenLog(dir, path, sizeof path);
    }
    CoTaskMemFree(pszSaved);
    if (s_hLog == INVALID_HANDLE_VALUE && GetTempPathA(sizeof dir, dir))
    {
        size_t n = strlen(dir);
        if (n && dir[n - 1] == '\\') dir[n - 1] = 0;
        s_hLog = OpenLog(dir, path, sizeof path);
    }
    if (s_hLog != INVALID_HANDLE_VALUE)
        Log("(game folder %s is not writable, error %lu: logging to %s)", pszDir, err, path);
}

static void LogVa(const char *fmt, va_list ap)
{
    if (s_hLog == INVALID_HANDLE_VALUE)
        return;
    char buf[1024];
    int n = _snprintf_s(buf, sizeof buf, _TRUNCATE, "[%7.3f tid %5lu] ", (GetTickCount() - s_t0) / 1000.0, GetCurrentThreadId());
    int m = _vsnprintf_s(buf + n, sizeof buf - n, _TRUNCATE, fmt, ap);
    if (m < 0) m = (int)sizeof buf - n - 1;
    n += m;
    if (n < (int)sizeof buf - 2) { buf[n++] = '\r'; buf[n++] = '\n'; }
    DWORD written;
    EnterCriticalSection(&s_cs);
    WriteFile(s_hLog, buf, n, &written, 0);
    FlushFileBuffers(s_hLog);
    LeaveCriticalSection(&s_cs);
}

void Log(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    LogVa(fmt, ap);
    va_end(ap);
}

void LogV(const char *fmt, ...)
{
#if SIMSMOD_DEV
    va_list ap;
    va_start(ap, fmt);
    LogVa(fmt, ap);
    va_end(ap);
#else
    (void)fmt;
#endif
}
