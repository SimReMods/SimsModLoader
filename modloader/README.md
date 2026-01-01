# modloader - winmm.dll

A mod loader for The Sims Legacy Collection (Steam, 32-bit). It is a drop-in `winmm.dll` placed next to `Sims.exe`:
the game loads it as the system multimedia DLL (every export is forwarded to the real one), it waits until the game
code is decrypted and verified, then loads every `mods\*.dll` and calls its `SimsMod_Init`. Nothing in `Sims.exe` or in
the game data is modified.

Safety: at start it compares the exe's PE identity, and after decryption a hash of the game code, with the build it was
made for. On a mismatch it shows a message box, loads no mod and lets the game run unmodified.

## Build

Open "x86 Native Tools Command Prompt for VS 2022" (any MSVC with the Windows SDK; the target is 32-bit) and run

    build.bat

Output: `build\winmm.dll`.

## Install

Copy `winmm.dll` into the game folder (the one with `Sims.exe`) and put mods into a `mods` subfolder. The loader
writes `mod.log` next to `Sims.exe`. To uninstall, delete `winmm.dll` and the `mods` folder.

## Writing a mod

A mod is a DLL exporting `int __cdecl SimsMod_Init(const SimsLoaderApi *api)` (see `simsmod.h`). The API gives the
exe base, a logger, signature checks, a detour (5-byte jump with trampoline) and a code writer. See `../mods`.
