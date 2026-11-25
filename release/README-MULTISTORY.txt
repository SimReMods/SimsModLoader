Multistory mod for The Sims Legacy Collection (Steam only, version 1.10.7.0 only).
WILL NOT WORK WITH COMLETE COLLECTION, WILL NOT WORK WITH EA VERSION, WILL NOT WORK WITH CRACKED VERSIONS.

Lots get two more stories (3 and 4) on top of the original two. Build, buy and live mode work on every story.

BACK UP YOUR SAVES FIRST. With the game closed, copy %USERPROFILE%\Saved Games\Electronic Arts\The Sims 25 somewhere safe.

Install: extract this zip into the game folder (the one with Sims.exe), keeping the folder structure:
  winmm.dll                           the mod loader (a proxy of the system winmm.dll)
  mods\multistory_steam.dll           the mod
  UIGraphics\Res_Multistory.RT        registers the story button art (UIGraphics.far is untouched)
  UIGraphics\Res_Multistory.h         its resource ids
  UIGraphics\Multistory\StoryDown.bmp the "one story down" button
  UIGraphics\Multistory\StoryUp.bmp   the "one story up" button

A LOT WITH ANYTHING ON STORIED 3-4 NEEDS THE MOD TO LOAD. Without it the game WILL CRASH on that lot.
Removing everything above the second floor should let you load back without the mod, BUT THERE IS NO GUARANTEE THERE WON'T BE OTHER BUGS.

Uninstall: delete these files from the game folder:
  winmm.dll                (keep it if another mod in the mods folder uses it)
  mods\multistory_steam.dll
  mods\multistory.dll  (older versions of the mod)
  UIGraphics\Res_Multistory.RT
  UIGraphics\Res_Multistory.h
  UIGraphics\Multistory\StoryDown.bmp
  UIGraphics\Multistory\StoryUp.bmp
  mod.log (if present)
Then delete the UIGraphics\Multistory folder, and the mods folder if nothing else is in it.

If you report a problem, attach mod.log from the game folder.
