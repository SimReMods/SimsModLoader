# seasons

The engine part of "The Sims: Seasons" (https://github.com/SimReMods/Seasons), as a patch mod: seasons and weather on
normal lots, the season icon and calendar in the control panel, raincoats / umbrellas / winter clothes at the door,
snow locations for Seasons objects (find_location_for modes 7..10), the seasons globals 38..49 writable from trees, the
`ExpansionPack8` objects folder and the extra careers of `ExpansionPack8\SeasonsCareers.iff` (job panel art and at-work
portraits included).

**Do not install it on its own**: the objects, careers, clothes and art it works with are shipped by the Seasons
installer, which downloads this mod's release zip for the game (`seasons_<steam|ea|cc>-DO-NOT-DOWNLOAD.zip`).

## Files

- `seasons.cpp` - the core hooks, the hook installation, the mod's operator new / delete (the exe's: one heap with the
  game, so the game's std containers can be used directly).
- `SeasonsCore.cpp`, `Seasons.h` - the seasons themselves: season clock, leaves and snow on the ground, the weather
  overlay, snow locations, dressing at the door, raincoats and umbrellas.
- `SeasonsCareers.cpp` - the extra careers, their job panel art and at-work portraits.
- `SeasonsFolder.cpp` - the `ExpansionPack8` objects folder.
- `SeasonsPanelIcon.cpp` - the season icon and calendar.
- `game_api.h` - the game classes, fields and functions the mod uses; `game_addr.h` / `game_addr_ea.h` /
  `game_addr_cc.h` - the Steam / EA app / Complete Collection addresses; `gen_sigs*.h` - the signatures of the patched
  places.

On the Complete Collection (2003) the weather is drawn in software into the lot view, the ground frosts evenly (no
per-corner snow or grass-blade fade, which only the Legacy 3D terrain has), and the Seasons content must be unpacked
(loose files, no `.far`) for raincoats, umbrellas and object animations.

## Build

Open "x86 Native Tools Command Prompt for VS 2022" and run `build.bat`. Output: `build\seasons_steam.dll` (Steam) and
`build\ea\seasons_ea.dll` (EA app) and `build\cc\seasons_cc.dll` (The Sims Complete Collection, 2003).
