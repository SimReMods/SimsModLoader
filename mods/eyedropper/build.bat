@echo off
rem Builds build\eyedropper.dll. Run from the "x86 Native Tools Command Prompt for VS".
setlocal
cd /d "%~dp0"
if not exist build mkdir build
cl /nologo /Brepro /c /MT /GS /EHsc /O2 /Oy- /std:c++17 /W3 /wd4996 /Fobuild\ eyedropper.cpp || exit /b 1
link /nologo /Brepro /DLL /MACHINE:X86 /OPT:REF /OPT:ICF /OUT:build\eyedropper.dll build\eyedropper.obj kernel32.lib user32.lib || exit /b 1
echo built build\eyedropper.dll
