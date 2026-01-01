@echo off
setlocal
cd /d "%~dp0"
if not exist build mkdir build
rem Version: MOD_VERSION (e.g. 1.0.0 or v1.0.0; CI sets it from the git tag), 0.0.0 when unset.
if not defined MOD_VERSION set MOD_VERSION=0.0.0
set V=%MOD_VERSION%
if /i "%V:~0,1%"=="v" set V=%V:~1%
for /f "tokens=1-3 delims=.-+" %%a in ("%V%") do (set VMAJ=%%a& set VMIN=%%b& set VPAT=%%c)
set /a VMAJ=VMAJ+0, VMIN=VMIN+0, VPAT=VPAT+0
> build\version.h echo #define VER_MAJOR %VMAJ%
>> build\version.h echo #define VER_MINOR %VMIN%
>> build\version.h echo #define VER_PATCH %VPAT%
>> build\version.h echo #define VER_STRING "%V%"
rc /nologo /i build /fo build\version.res version.rc || exit /b 1
cl /nologo /Brepro /c /MT /GS /EHsc /GR /O2 /Oy- /std:c++17 /W3 /wd4996 /Fobuild\ winmm.cpp winmm_exports.cpp loader_main.cpp detour.cpp x86len.cpp log.cpp || exit /b 1
link /nologo /Brepro /DLL /MACHINE:X86 /OPT:REF /OPT:ICF /DEF:winmm.def /OUT:build\winmm.dll build\winmm.obj build\winmm_exports.obj build\loader_main.obj build\detour.obj build\x86len.obj build\log.obj build\version.res kernel32.lib user32.lib || exit /b 1
echo built build\winmm.dll (version %V%)
