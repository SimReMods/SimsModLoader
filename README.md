# SimsModLoader

A mod loader for The Sims 1 Legacy Collection: a drop-in `winmm.dll` that patches the game in memory while it runs
and loads mods from a `mods` folder, without changing any of the game's files on disk. This repository contains the
loader and its first mod, Visit Lots.

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

1. Download `sims-visitlots-<version>.zip` from **Releases** (right-hand side of this page).
2. In Steam, right-click **The Sims Legacy Collection** → **Manage** → **Browse local files**.
3. Copy everything from the zip into that folder, merging folders. Replace files only when upgrading the mod; on a
   fresh install nothing should need replacing.

### Uninstall

With the game closed, delete these files from the game folder:

```
winmm.dll
README-MOD.txt
mod.log
mods\visit_lots.dll
GameData\UIText2.iff
GameData\Global\PersonGlobals.iff
GameData\Global\DogGlobals.iff
GameData\Global\CatGlobals.iff
GameData\Objects\SocialInteractions.iff
GameData\Objects\People\PizzaDude.iff
ExpansionShared\HDSI_Ask.iff
```

If you report a problem, attach `mod.log` from the game folder.

## For developers

The `winmm.dll` loader (`modloader/`) patches the game in memory and loads `mods\*.dll`; nothing on disk is changed.
`mods/visit_lots/` is the mod, `mods/example_mod/` a minimal example. Build each with its `build.bat` from the
"x86 Native Tools Command Prompt for VS 2022", then `python package.py <version>` makes the release zip.

> **This project has been coded using AI coding tools.**
