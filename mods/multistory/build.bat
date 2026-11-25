@echo off
rem Builds build\multistory_steam.dll (Legacy Collection from Steam) and build\ea\multistory_ea.dll (Legacy Collection
rem from the EA app). Run from the "x86 Native Tools Command Prompt for VS 2022".
rem Generated (do not edit): ms_game_api.h, msstubs.asm, ms_patches.h; their EA ports ms_game_api_ea.h, msstubs_ea.asm,
rem ms_patches_ea.h, ms_va_ea.h come from tools/port_multistory.py.
setlocal
cd /d "%~dp0"
if not exist build mkdir build
ml /nologo /c /coff /Fobuild\msstubs.obj msstubs.asm || exit /b 1
ml /nologo /c /coff /Fobuild\ms_pick.obj ms_pick.asm || exit /b 1
cl /nologo /c /MT /GS /EHsc /GR /Gy /O2 /Oy- /TP /std:c++17 /W3 /wd4996 /DNDEBUG /D_CRT_SECURE_NO_WARNINGS /Fobuild\ multistory.cpp || exit /b 1
link /nologo /DLL /MACHINE:X86 /OPT:REF /OPT:ICF /OUT:build\multistory_steam.dll build\multistory.obj build\msstubs.obj build\ms_pick.obj kernel32.lib user32.lib || exit /b 1
echo built build\multistory_steam.dll
if not exist build\ea mkdir build\ea
ml /nologo /c /coff /Fobuild\ea\msstubs.obj msstubs_ea.asm || exit /b 1
ml /nologo /c /coff /Fobuild\ea\ms_pick.obj ms_pick.asm || exit /b 1
cl /nologo /c /MT /GS /EHsc /GR /Gy /O2 /Oy- /TP /std:c++17 /W3 /wd4996 /DNDEBUG /D_CRT_SECURE_NO_WARNINGS /DGAME_EA=1 /Fobuild\ea\ multistory.cpp || exit /b 1
link /nologo /DLL /MACHINE:X86 /OPT:REF /OPT:ICF /OUT:build\ea\multistory_ea.dll build\ea\multistory.obj build\ea\msstubs.obj build\ea\ms_pick.obj kernel32.lib user32.lib || exit /b 1
echo built build\ea\multistory_ea.dll
