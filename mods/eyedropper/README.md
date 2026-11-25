# eyedropper - pick an object, floor or wall off the lot (SimsModLoader mod)

In build and buy mode, the eyedropper (the button below undo / redo in the build panel, or the `E` key) picks what is
under the cursor and selects its catalog entry:

- an object -> its build tool (doors, windows, stairs, fireplaces, plants, pools) or its buy catalog page, with the
  object on the cursor ready to place;
- a floor -> the floor tool with that floor;
- a wall -> the wallpaper tool with that wallpaper; fences, unpainted walls and Shift-click -> the wall tool with that
  wall type.

`Esc` (or the button / `E` again) puts the mode's previous tool back. The tool tip shows the name of what would be picked.

## Install

Needs the SimsModLoader `winmm.dll` in the game folder (Steam Legacy Collection 1.10.7.0). Copy `eyedropper.dll` into
the game's `mods` folder and `data\UIGraphics` into the game's `UIGraphics` folder. Without the `UIGraphics` files there
is no button (the `E` key still works) and the eyedropper uses the pointing-hand cursor. It can be installed together
with visit_lots and multistory (on multistory's stories 3-4 objects and walls are picked, floors are not, and a
cut-away wall counts as full height: the eyedropper reads the game's story-1/2 floor and wall tables directly).

Nothing is saved: the mod changes no file and no save data.

## How it works

The game's code runs unchanged; two functions are wrapped at their entry:

- `cWinArch::UpdateViewFromCPState` (0x46d470) creates the button;
- `cDDDSimsView::TSOnCharacter` (0x40dfd0) takes the `E` key in build / buy mode.

The tool (a `cTool`) and the button (a `cTSWinBtn`) are game objects: the game's constructor builds them, then their
vtable pointer is switched to a copy of the game's vtable (RTTI locator included) with a few slots replaced.

At start-up every game function the mod calls is checked against its signature (`gen_sigs.h`), as are every vtable
slot it copies or calls, the RTTI names and the dynamic_cast helper; nothing is patched unless all of them match. The
one exception is `CPState::SetMode`: visit_lots wraps it, so its first bytes depend on the load order (`s_NoSigCheck`
in `eyedropper.cpp`; the loader has already matched the exe build, so its address holds).

## Build

From the "x86 Native Tools Command Prompt for VS": `build.bat` -> `build\eyedropper.dll`.

| file | |
|---|---|
| `eyedropper.cpp` | the feature, the hooks, the installer |
| `game_api.h` | the game classes, fields and functions the mod uses (explicit offsets, address thunks) |
| `game_addr.h` | every game address the mod uses (functions, globals, RTTI, vtable slots) |
| `gen_sigs.h` | generated from `game_addr.h` and the game build: the function signatures |
| `simsmod.h` | the loader interface |
| `data/` | the button art (`EyedropperBtn.bmp`, id 9520 via `Res_Eyedropper.RT`) and the cursor (`Eyedropper.cur`) |

## What to report

`mod.log` next to `Sims.exe`: the lines starting with `[eyedropper]`. A healthy start logs `2 hooks installed, 0 failed`.
