Visit Lots mod for The Sims Legacy Collection (Steam) - a runtime patch mod: nothing in Sims.exe or the game's
archives is changed on disk.

Install: extract this zip into the game folder (the one with Sims.exe), keeping the folder structure:
  winmm.dll                          the mod loader (a proxy of the system winmm.dll)
  mods\visit_lots.dll                the mod
  GameData\Global\PersonGlobals.iff  modified globals (a loose file; it overrides the copy inside Global.far)
  GameData\Global\DogGlobals.iff and CatGlobals.iff  modified pet globals (loose; same mechanism)
  GameData\Objects\People\PizzaDude.iff and GameData\Objects\SocialInteractions.iff  the pizza delivery (loose)
  ExpansionShared\HDSI_Ask.iff       the "what's for dinner" conversation (loose)
  GameData\UIText2.iff               one changed UI string table; an overlay the mod reads first (UIText.iff is untouched)

The loader writes a short mod.log next to Sims.exe. If a game update makes Sims.exe differ from the version this
mod was built for, the loader shows a message and the game starts without the mod.

Uninstall: delete these files from the game folder:
  winmm.dll
  mods\visit_lots.dll
  GameData\Global\PersonGlobals.iff
  GameData\Global\DogGlobals.iff
  GameData\Global\CatGlobals.iff
  GameData\Objects\People\PizzaDude.iff
  GameData\Objects\SocialInteractions.iff
  ExpansionShared\HDSI_Ask.iff
  GameData\UIText2.iff
  mod.log (if present)
Then delete the mods folder if nothing else is in it.
