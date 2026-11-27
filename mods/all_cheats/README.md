# all_cheats - every cheat the game has, in the cheat window (SimsModLoader mod)

The game registers over 100 cheats but lets only 28 of them through (rosebud, move_objects, set_hour, map_edit,
sim_speed, ...). With this mod all of them work in the cheat window (Ctrl+Shift+C); `help` lists them. Some examples:
`motives`, `lot_size`, `age_me`, `kid_me`, `cat_me`, `dog_me`, `transform_me`, `edit_char`, `fam_test`,
`magic_tokens`, `set_level`, `visitor_control`, `water_tool`, `edit_grass`, `swap_houses`, `reload_people`.

Many of them are the developers' own tools: they do what they do without asking. Save before trying an unknown one.
Five stay off because they only crash the game or overwrite its files: `crash`, `core_dump`, `assert`,
`save_careers`, `save_careers_txt`.

Builds: `all_cheats_steam.dll` (Legacy Collection from Steam), `ea\all_cheats_ea.dll` (Legacy Collection from the EA
app), `cc\all_cheats_cc.dll` (Complete Collection, 2003).

## How it works

When a cheat is registered, `cTSCheatCodeManager::RegisterCheat` asks `cTSCheatCodeManager::AllowCheat(name)` and drops
the cheat on a no; the game's answer is yes only for its short list. On the Complete Collection the mod wraps
`AllowCheat` (0x5e6990) and answers yes for every other name too (the five above get the game's own answer). The
Legacy builds have that test compiled into `RegisterCheat` itself, so there the mod wraps `RegisterCheat` (Steam
0x6e6fc0, EA 0x9753f0), drops the five and lets the rest through. mod.log lists every cheat the mod unlocked. Nothing is saved; the mod changes no file.

## Build

`build.bat` from the "x86 Native Tools Command Prompt for VS".
