Visit Lots mod {VERSION} for {GAME} - a runtime patch mod: nothing in Sims.exe or the game's archives is changed
on disk.
THIS ZIP IS ONLY FOR {GAME_UPPER}. For another version of the game download that version's zip{CRACKED}.

Your Sims can visit other families at home. Take a trip to Old Town and pick a home where another family lives (the
7th filter button shows them). The hosts greet you at the door, you can ask them what's for dinner, and when it gets
late your Sims go home. The visited home is never saved.

Install: extract this zip into the game folder (the one with Sims.exe), keeping the folder structure:
  winmm.dll                          the mod loader (a proxy of the system winmm.dll)
  mods\{DLL}                the mod
  GameData\Global\PersonGlobals.iff  modified globals (a loose file; it overrides the copy inside Global.far)
  GameData\Global\DogGlobals.iff and CatGlobals.iff  modified pet globals (loose; same mechanism)
  GameData\Objects\People\PizzaDude.iff and GameData\Objects\SocialInteractions.iff  the pizza delivery (loose)
  ExpansionShared\HDSI_Ask.iff       the "what's for dinner" conversation (loose)
  GameData\UIText2.iff               one changed UI string table; an overlay the mod reads first (UIText.iff is untouched)

The loader writes a short mod.log next to Sims.exe. If a game update makes Sims.exe differ from the version this
mod was built for, the loader shows a message and the game starts without the mod.

Uninstall only while your Sims are at home, not in the middle of a visit. Delete these files from the game folder:
  winmm.dll                (keep it if another mod in the mods folder uses it)
  mods\{DLL}
  mods\visit_lots.dll  (older versions of the mod)
  GameData\Global\PersonGlobals.iff
  GameData\Global\DogGlobals.iff
  GameData\Global\CatGlobals.iff
  GameData\Objects\People\PizzaDude.iff
  GameData\Objects\SocialInteractions.iff
  ExpansionShared\HDSI_Ask.iff
  GameData\UIText2.iff
  mod.log (if present)
Then delete the mods folder if nothing else is in it.

If you report a problem, attach mod.log from the game folder.
