# multistory - 4 stories (SimsModLoader mod)

Lots get two more stories (3 and 4) on top of the original two. Build, buy and live mode work on every story; the view control's story buttons become "one story down" / "one story up"

## Install

Needs the SimsModLoader `winmm.dll` in the game folder (Steam Legacy Collection 1.10.7.0). Copy `multistory_steam.dll` into the game's `mods` folder and `data\UIGraphics` into the game's `UIGraphics` folder. It can be installed together with visit_lots.

**BACK UP YOUR SAVES.** A lot with anything on stories 3-4 NEEDS THE MOD to load. Otherwise the base game WILL CRASH!!

## How it works

Nothing in the game's data structures changes size. Stories 3-4 live in a side block per object, and every
instruction that indexes a per-story array by a variable story is redirected to that block for stories 3-4.

## Build

From the "x86 Native Tools Command Prompt for VS 2022": `build.bat` -> `build\multistory_steam.dll`.

| file | |
|---|---|
| `multistory.cpp` | the installer, the hooks and replacements |
| `ms_pick.asm` | the two story-pick stubs |
| `ms_game_api.h` | generated: the game offsets, functions, globals and detour signatures the mod uses |
| `msstubs.asm` | generated: the per-story access stubs |
| `ms_patches.h` | generated: the byte patches |
| `simsmod.h` | the loader interface |
| `data/` | the story button art |

## What to report

`mod.log` next to `Sims.exe`: the lines starting with `[multistory]`. A healthy start logs
`167 access stubs, 40 byte patches installed`, `story icons: ids 9500 / 9501` and `ready` (not `ready WITH ERRORS`),
and `world ...: stories 3-4 allocated` when a lot loads.