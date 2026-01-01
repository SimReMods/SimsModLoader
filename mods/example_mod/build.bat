@echo off
setlocal
cd /d "%~dp0"
if not exist build mkdir build
cl /nologo /Brepro /c /MT /EHsc /O2 /std:c++17 /W3 /Fobuild\ example_mod.cpp || exit /b 1
link /nologo /Brepro /DLL /MACHINE:X86 /OUT:build\example_mod.dll build\example_mod.obj kernel32.lib user32.lib || exit /b 1
echo built build\example_mod.dll
