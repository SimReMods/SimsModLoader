@echo off
rem Builds build\eyedropper.dll (Legacy Collection), build\ea\eyedropper_ea.dll (Legacy Collection from the EA app)
rem and, when game_addr_cc.h is present, build\cc\eyedropper_cc.dll (Complete Collection).
rem Run from the "x86 Native Tools Command Prompt for VS".
setlocal
cd /d "%~dp0"
if not exist build mkdir build
cl /nologo /Brepro /c /MT /GS /EHsc /O2 /Oy- /std:c++17 /W3 /wd4996 /Fobuild\ eyedropper.cpp || exit /b 1
link /nologo /Brepro /DLL /MACHINE:X86 /OPT:REF /OPT:ICF /OUT:build\eyedropper.dll build\eyedropper.obj kernel32.lib user32.lib || exit /b 1
echo built build\eyedropper.dll
if not exist build\ea mkdir build\ea
cl /nologo /Brepro /c /MT /GS /EHsc /O2 /Oy- /std:c++17 /W3 /wd4996 /DGAME_EA=1 /Fobuild\ea\ eyedropper.cpp || exit /b 1
link /nologo /Brepro /DLL /MACHINE:X86 /OPT:REF /OPT:ICF /OUT:build\ea\eyedropper_ea.dll build\ea\eyedropper.obj kernel32.lib user32.lib || exit /b 1
echo built build\ea\eyedropper_ea.dll
if not exist game_addr_cc.h (
    echo skipped build\cc\eyedropper_cc.dll: game_addr_cc.h not present
    exit /b 0
)
if not exist build\cc mkdir build\cc
cl /nologo /Brepro /c /MT /GS /EHsc /O2 /Oy- /std:c++17 /W3 /wd4996 /DGAME_CC=1 /Fobuild\cc\ eyedropper.cpp || exit /b 1
link /nologo /Brepro /DLL /MACHINE:X86 /OPT:REF /OPT:ICF /OUT:build\cc\eyedropper_cc.dll build\cc\eyedropper.obj kernel32.lib user32.lib || exit /b 1
echo built build\cc\eyedropper_cc.dll
