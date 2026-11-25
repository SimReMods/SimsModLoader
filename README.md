# SimsModLoader

A mod loader for The Sims 1 Legacy Collection: a drop-in `winmm.dll` that patches the game in memory while it runs
and loads mods from a `mods` folder, without changing any of the game's files on disk. This repository contains the
loader and three mods: Visit Lots, 4 Stories and Eyedropper.

## Visit Lots mod

Your Sims can visit other families at home. Take a trip to Old Town and pick a home where another family lives (the
7th filter button shows them). The hosts greet you at the door, you can ask them what's for dinner, and when it gets
late your Sims go home. The visited home is never saved.

> **Only works with The Sims 1 Legacy Collection from Steam, version 1.10.7.0.** On any other version the mod shows
> a message and the game starts without it.

> **Back up your saves first.** This mod changes parts of the original game engine while the game is running. With
> the game closed, copy `%USERPROFILE%\Saved Games\Electronic Arts\The Sims 25` somewhere safe.

> **No guarantee.** After you uninstall the mod, your game should play as if nothing had ever been installed.
> Relationships and skills gained on visits are kept, like after a trip Downtown. **Uninstall only while your Sims are
> at home, not in the middle of a visit.** There is no guarantee about side effects on your save files.

### Install

1. Download [`sims-visitlots.zip`](https://github.com/SimReMods/SimsModLoader/releases/latest/download/sims-visitlots.zip).
2. In Steam, right-click **The Sims Legacy Collection** → **Manage** → **Browse local files**.
3. Copy everything from the zip into that folder, merging folders. Replace files only when upgrading the mod; on a
   fresh install nothing should need replacing.

### Uninstall

With the game closed, delete these files from the game folder (keep `winmm.dll` if another mod is still installed):

```
winmm.dll
README-MOD.txt
mod.log
mods\visit_lots_steam.dll
mods\visit_lots.dll (older versions)
GameData\UIText2.iff
GameData\Global\PersonGlobals.iff
GameData\Global\DogGlobals.iff
GameData\Global\CatGlobals.iff
GameData\Objects\SocialInteractions.iff
GameData\Objects\People\PizzaDude.iff
ExpansionShared\HDSI_Ask.iff
```

If you report a problem, attach `mod.log` from the game folder.

## 4 Stories mod

Lots get two more stories (3 and 4) on top of the original two. Build, buy and live mode work on every story. The
story buttons in the view control become "one story down" / "one story up", PgUp / PgDn move one story, and Tab cycles
up through the stories. A room built on a story above leaves no roof under it, with or without a floor.

> **Only works with The Sims 1 Legacy Collection from Steam, version 1.10.7.0.** It will not work with the Complete
> Collection, the EA app version or cracked versions.

> **Back up your saves first.** With the game closed, copy `%USERPROFILE%\Saved Games\Electronic Arts\The Sims 25`
> somewhere safe.

> **A lot with anything on stories 3-4 needs the mod to load.** Without it, the game crashes on that lot. Removing
> everything above the second story should let you load it without the mod, but there is no guarantee there won't be
> other bugs.

### Install

1. Download [`sims-multistory.zip`](https://github.com/SimReMods/SimsModLoader/releases/latest/download/sims-multistory.zip).
2. In Steam, right-click **The Sims Legacy Collection** → **Manage** → **Browse local files**.
3. Copy everything from the zip into that folder, merging folders. It can be installed together with Visit Lots (both
   zips contain the same `winmm.dll`).

### Uninstall

With the game closed, delete these files from the game folder (keep `winmm.dll` if another mod is still installed):

```
winmm.dll
README-MULTISTORY.txt
mod.log
mods\multistory_steam.dll
mods\multistory.dll (older versions)
UIGraphics\Res_Multistory.RT
UIGraphics\Res_Multistory.h
UIGraphics\Multistory\StoryDown.bmp
UIGraphics\Multistory\StoryUp.bmp
```

If you report a problem, attach `mod.log` from the game folder.

## Eyedropper mod

In build and buy mode, click the eyedropper button (below undo / redo in the build panel) or press `E`, then click an
object, a floor or a wall on the lot: its catalog entry gets selected (the object on the cursor, the floor tool with
that floor, the wallpaper tool with that wallpaper; Shift-click a wall for its wall type). `Esc` puts the previous tool
back. The mod changes no files and no save data.

> **Only works with The Sims 1 Legacy Collection from Steam, version 1.10.7.0.**

### Install

1. Download [`sims-eyedropper.zip`](https://github.com/SimReMods/SimsModLoader/releases/latest/download/sims-eyedropper.zip).
2. In Steam, right-click **The Sims Legacy Collection** → **Manage** → **Browse local files**.
3. Copy everything from the zip into that folder, merging folders. It can be installed together with the other mods.

### Uninstall

With the game closed, delete these files from the game folder (keep `winmm.dll` if another mod is still installed):

```
winmm.dll
README-EYEDROPPER.txt
mod.log
mods\eyedropper_steam.dll
mods\eyedropper.dll (older versions)
UIGraphics\Res_Eyedropper.RT
UIGraphics\Res_Eyedropper.h
UIGraphics\Eyedropper\EyedropperBtn.bmp
UIGraphics\Eyedropper\Eyedropper.cur
```

If you report a problem, attach `mod.log` from the game folder.

## For developers

The `winmm.dll` loader (`modloader/`) patches the game in memory and loads `mods\*.dll`; nothing on disk is changed.
`mods/visit_lots/`, `mods/multistory/` and `mods/eyedropper/` are the mods, `mods/example_mod/` a minimal example. Build each with its
`build.bat` from the "x86 Native Tools Command Prompt for VS 2022", then `python package.py` makes one release zip
per mod.

> **This project has been coded using AI coding tools.**
