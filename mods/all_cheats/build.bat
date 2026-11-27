@echo off
rem Builds build\all_cheats_steam.dll (Legacy Collection from Steam), build\ea\all_cheats_ea.dll (Legacy Collection from
rem the EA app) and build\cc\all_cheats_cc.dll (Complete Collection). Run from the "x86 Native Tools Command Prompt for VS".
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
cl /nologo /Brepro /c /MT /GS /EHsc /O2 /Oy- /std:c++17 /W3 /wd4996 /Fobuild\ all_cheats.cpp || exit /b 1
link /nologo /Brepro /DLL /MACHINE:X86 /OPT:REF /OPT:ICF /OUT:build\all_cheats_steam.dll build\all_cheats.obj build\version.res kernel32.lib user32.lib || exit /b 1
echo built build\all_cheats_steam.dll (version %V%)
if not exist build\ea mkdir build\ea
cl /nologo /Brepro /c /MT /GS /EHsc /O2 /Oy- /std:c++17 /W3 /wd4996 /DGAME_EA=1 /Fobuild\ea\ all_cheats.cpp || exit /b 1
link /nologo /Brepro /DLL /MACHINE:X86 /OPT:REF /OPT:ICF /OUT:build\ea\all_cheats_ea.dll build\ea\all_cheats.obj build\version.res kernel32.lib user32.lib || exit /b 1
echo built build\ea\all_cheats_ea.dll (version %V%)
if not exist build\cc mkdir build\cc
cl /nologo /Brepro /c /MT /GS /EHsc /O2 /Oy- /std:c++17 /W3 /wd4996 /DGAME_CC=1 /Fobuild\cc\ all_cheats.cpp || exit /b 1
link /nologo /Brepro /DLL /MACHINE:X86 /OPT:REF /OPT:ICF /OUT:build\cc\all_cheats_cc.dll build\cc\all_cheats.obj build\version.res kernel32.lib user32.lib || exit /b 1
echo built build\cc\all_cheats_cc.dll (version %V%)
