# SimsModLoader

A mod loader for The Sims 1 (Legacy Collection from Steam or the EA app, and the Complete Collection): a drop-in
`winmm.dll` that patches the game in memory while it runs and loads mods from a `mods` folder, without changing any of
the game's files on disk. This repository contains the loader and the mods: Visit Lots, 4 Stories, Eyedropper and the
engine part of Seasons.

## Which zip, and where it goes

Each mod comes as one zip per version of the game: `<mod>_steam.zip` for The Sims Legacy Collection from Steam
(1.10.7.0), `<mod>_ea.zip` for the Legacy Collection from the EA app (1.10.7) and `<mod>_cc.zip` for The Sims Complete
Collection (2003). Download the one for your game; on any other version (or a cracked one) the mod shows a message and
the game starts without it. The README in each zip starts with the mod's version.

Copy everything from the zip into the game folder (the one with `Sims.exe`), merging folders:
- Steam: right-click **The Sims Legacy Collection** → **Manage** → **Browse local files**.
- EA app: the game's install folder, by default `C:\Program Files\EA Games\The Sims Legacy`.
- Complete Collection: the folder you installed it to (the one with `Sims.exe`).

Several mods can be installed together: their zips contain the same `winmm.dll`.

## Visit Lots mod

Your Sims can visit other families at home. Take a trip to Old Town and pick a home where another family lives (the
7th filter button shows them). The hosts greet you at the door, you can ask them what's for dinner, and when it gets
late your Sims go home. The visited home is never saved.

> **Back up your saves first.** This mod changes parts of the original game engine while the game is running. With
> the game closed, copy your saves somewhere safe (Legacy Collection: `%USERPROFILE%\Saved Games\Electronic Arts\The
> Sims 25`; Complete Collection: the `UserData` folders in the game folder).

> **No guarantee.** After you uninstall the mod, your game should play as if nothing had ever been installed.
> Relationships and skills gained on visits are kept, like after a trip Downtown. **Uninstall only while your Sims are
> at home, not in the middle of a visit.** There is no guarantee about side effects on your save files.

### Install

1. Download the zip for your game:
   - Legacy Collection (Steam): [`visit_lots_steam.zip`](https://github.com/SimReMods/SimsModLoader/releases/latest/download/visit_lots_steam.zip)
   - Legacy Collection (EA app): [`visit_lots_ea.zip`](https://github.com/SimReMods/SimsModLoader/releases/latest/download/visit_lots_ea.zip)
   - Complete Collection: [`visit_lots_cc.zip`](https://github.com/SimReMods/SimsModLoader/releases/latest/download/visit_lots_cc.zip)
2. Copy everything from the zip into the game folder (see above), merging folders. Replace files only when upgrading
   the mod; on a fresh install nothing should need replacing.

### Uninstall

With the game closed, delete these files from the game folder (keep `winmm.dll` if another mod is still installed):

```
winmm.dll
README-MOD.txt
mod.log
mods\visit_lots_steam.dll, visit_lots_ea.dll or visit_lots_cc.dll
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

> **For the Legacy Collection only** (Steam or the EA app). There is no version for the Complete Collection.

> **Back up your saves first.** With the game closed, copy `%USERPROFILE%\Saved Games\Electronic Arts\The Sims 25`
> somewhere safe.

> **A lot with anything on stories 3-4 needs the mod to load.** Without it, the game crashes on that lot. Removing
> everything above the second story should let you load it without the mod, but there is no guarantee there won't be
> other bugs.

### Install

1. Download the zip for your game:
   - Legacy Collection (Steam): [`multistory_steam.zip`](https://github.com/SimReMods/SimsModLoader/releases/latest/download/multistory_steam.zip)
   - Legacy Collection (EA app): [`multistory_ea.zip`](https://github.com/SimReMods/SimsModLoader/releases/latest/download/multistory_ea.zip)
2. Copy everything from the zip into the game folder (see above), merging folders.

### Uninstall

With the game closed, delete these files from the game folder (keep `winmm.dll` if another mod is still installed):

```
winmm.dll
README-MULTISTORY.txt
mod.log
mods\multistory_steam.dll or multistory_ea.dll
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

### Install

1. Download the zip for your game:
   - Legacy Collection (Steam): [`eyedropper_steam.zip`](https://github.com/SimReMods/SimsModLoader/releases/latest/download/eyedropper_steam.zip)
   - Legacy Collection (EA app): [`eyedropper_ea.zip`](https://github.com/SimReMods/SimsModLoader/releases/latest/download/eyedropper_ea.zip)
   - Complete Collection: [`eyedropper_cc.zip`](https://github.com/SimReMods/SimsModLoader/releases/latest/download/eyedropper_cc.zip)
2. Copy everything from the zip into the game folder (see above), merging folders.

### Uninstall

With the game closed, delete these files from the game folder (keep `winmm.dll` if another mod is still installed):

```
winmm.dll
README-EYEDROPPER.txt
mod.log
mods\eyedropper_steam.dll, eyedropper_ea.dll or eyedropper_cc.dll
mods\eyedropper.dll (older versions)
UIGraphics\Res_Eyedropper.RT
UIGraphics\Res_Eyedropper.h
UIGraphics\Eyedropper\EyedropperBtn.bmp
UIGraphics\Eyedropper\Eyedropper.cur
```

If you report a problem, attach `mod.log` from the game folder.

## Seasons (engine part)

`mods/seasons/` is the engine part of "The Sims: Seasons": seasons and weather on normal lots, the season icon and
calendar, raincoats, umbrellas and winter clothes, the ExpansionPack8 objects folder and the Seasons careers.

> **DO NOT INSTALL IT ON YOUR OWN. The required files are shipped in https://github.com/SimReMods/Seasons** - install
> Seasons from there. Its installer downloads this repository's `seasons_<steam|ea|cc>-DO-NOT-DOWNLOAD.zip` itself.

## For developers

The `winmm.dll` loader (`modloader/`) patches the game in memory and loads `mods\*.dll`; nothing on disk is changed.
`mods/visit_lots/`, `mods/multistory/`, `mods/eyedropper/` and `mods/seasons/` are the mods, `mods/example_mod/` a minimal example. Build each with its
`build.bat` from the "x86 Native Tools Command Prompt for VS 2022", then `python package.py [--version 1.2.3]` makes
one release zip per mod per game (`dist\<mod>_<steam|ea|cc>.zip`); CI passes the tag as the version.

> **This project has been coded using AI coding tools.**
