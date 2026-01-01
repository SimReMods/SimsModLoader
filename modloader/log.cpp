// log.cpp - mod.log next to the exe. Opened once (append), every line flushed: the process may die at any point.
#include <windows.h>
#include <stdio.h>
#include <stdarg.h>
#include "log.h"

static HANDLE s_hLog = INVALID_HANDLE_VALUE;
static CRITICAL_SECTION s_cs;
static DWORD s_t0;

void Log_Init(const char *pszDir)
{
    char path[MAX_PATH];
    _snprintf_s(path, sizeof path, _TRUNCATE, "%s\\mod.log", pszDir);
    InitializeCriticalSection(&s_cs);
    s_hLog = CreateFileA(path, FILE_APPEND_DATA, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, 0, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
    s_t0 = GetTickCount();
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
