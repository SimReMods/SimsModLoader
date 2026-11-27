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
set SRCS=seasons.cpp SeasonsCore.cpp SeasonsCareers.cpp SeasonsFolder.cpp SeasonsPanelIcon.cpp
set OBJS=build\seasons.obj build\SeasonsCore.obj build\SeasonsCareers.obj build\SeasonsFolder.obj build\SeasonsPanelIcon.obj
cl /nologo /Brepro /c /MT /GS /EHsc /O2 /Oy- /std:c++17 /W3 /wd4996 /wd4624 /Fobuild\ %SRCS% || exit /b 1
link /nologo /Brepro /DLL /MACHINE:X86 /OPT:REF /OPT:ICF /OUT:build\seasons_steam.dll %OBJS% build\version.res kernel32.lib user32.lib || exit /b 1
echo built build\seasons_steam.dll (version %V%)
if not exist build\ea mkdir build\ea
cl /nologo /Brepro /c /MT /GS /EHsc /O2 /Oy- /std:c++17 /W3 /wd4996 /wd4624 /DGAME_EA=1 /Fobuild\ea\ %SRCS% || exit /b 1
link /nologo /Brepro /DLL /MACHINE:X86 /OPT:REF /OPT:ICF /OUT:build\ea\seasons_ea.dll %OBJS:build\=build\ea\% build\version.res kernel32.lib user32.lib || exit /b 1
echo built build\ea\seasons_ea.dll (version %V%)
if not exist game_addr_cc.h (
    echo skipped build\cc\seasons_cc.dll: game_addr_cc.h not present
    exit /b 0
)
if not exist build\cc mkdir build\cc
set CCOBJS=build\cc\seasons.obj build\cc\SeasonsCore.obj build\cc\SeasonsCareers.obj build\cc\SeasonsFolder.obj build\cc\SeasonsPanelIcon.obj
cl /nologo /Brepro /c /MT /GS /EHsc /O2 /Oy- /std:c++17 /W3 /wd4996 /wd4624 /DGAME_CC=1 /Fobuild\cc\ %SRCS% || exit /b 1
link /nologo /Brepro /DLL /MACHINE:X86 /OPT:REF /OPT:ICF /OUT:build\cc\seasons_cc.dll %CCOBJS% build\version.res kernel32.lib user32.lib || exit /b 1
echo built build\cc\seasons_cc.dll (version %V%)
