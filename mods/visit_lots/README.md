# visit_lots

Visiting another family's home. From the Old Town lot picker (the phone's / taxi's "Go to Old Town" trip), residential
lots where another family lives can be picked too: the owners stay the lot's family (autonomous, never selectable),
the travelling Sims arrive as guests (visitors, the only selectable Sims), Build/Buy and saving are off, the lot file
is never written; only neighbourhood data (relationships, skills, inventories) persists, as on community lots. The
seventh filter button of the picker (the lodging one, which can never light up in that window) becomes "homes you
can drop in on". The hosts greet the guests at the door; a guest can ask a host what's for dinner. The visit ends by
itself when the hosts are asleep or exhausted, all at work or school, after a full day, or when the guests are
starving or falling asleep (after a hint, never in the first hour); the guests can also go home from the phone.

## How it works

The game's code runs unchanged; the mod patches it in memory at a few places (35 in all), each checked against a
16-byte signature of the game build it was made for before anything is written:

- 19 functions are wrapped at their entry (the wrapper applies the visit's rule and calls the original, or returns);
- 13 calls inside game functions are redirected to the mod (same calling convention as the original callee);
- 3 short code sequences jump to small assembler stubs (see the `// ASM:` comments in `visitlots.cpp`).

## Files

- `visitlots.cpp` - the hooks, their installation, the UIText2 overlay.
- `LotVisit.cpp`, `LotVisit.h` - the visit's rules (arrival, greeting, hints, the end of the visit, saving).
- `LotVisitText.cpp`, `LotVisitText.h` - the popup and button texts in the game's 20 languages.
- `game_api.h` - the game classes, fields and functions the mod uses (explicit offsets, address thunks).
- `game_addr.h` - every game address the mod uses; `gen_sigs.h` - the signatures of the patched places.
- `simsmod.h` - the loader interface (same as `modloader/simsmod.h`).
- `data/` - seven loose data files the game reads in preference to the copies inside its archives:
  `GameData\Objects\People\PizzaDude.iff`, `GameData\Objects\SocialInteractions.iff`,
  `GameData\Global\PersonGlobals.iff`, `GameData\Global\DogGlobals.iff`, `GameData\Global\CatGlobals.iff`,
  `ExpansionShared\HDSI_Ask.iff` (the "what's for dinner" conversation) and the overlay `GameData\UIText2.iff` (the
  mod reads it first for the one changed string table; the original `UIText.iff` is untouched).

## Build

Open "x86 Native Tools Command Prompt for VS 2022" and run

    build.bat

Output: `build\visit_lots.dll`.

## Install

1. Install the loader (`modloader\build\winmm.dll`) into the game folder.
2. Copy `build\visit_lots.dll` to `<game folder>\mods\`.
3. Copy the contents of `data\` into the game folder (so that e.g. `<game folder>\GameData\Global\PersonGlobals.iff`
   exists next to `Global.far` and `<game folder>\ExpansionShared\HDSI_Ask.iff` next to `ExpansionShared.far`).

Uninstall: delete `mods\visit_lots.dll` and the seven data files. Nothing original is modified.
