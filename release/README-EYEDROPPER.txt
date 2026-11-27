Eyedropper mod {VERSION} for {GAME}.
THIS ZIP IS ONLY FOR {GAME_UPPER}. For another version of the game download that version's zip{CRACKED}.

In build and buy mode, click the eyedropper button (below undo / redo in the build panel) or press E, then click
something on the lot: an object, a floor or a wall. Its catalog entry gets selected - the object's catalog page with the
object on the cursor, the floor tool with that floor, the wallpaper tool with that wallpaper (Shift-click a wall for the
wall tool with that wall type). Esc puts the previous tool back. The mod changes no files and no save data.

Install: extract this zip into the game folder (the one with Sims.exe), keeping the folder structure:
  winmm.dll                                 the mod loader (a proxy of the system winmm.dll)
  mods\{DLL}                       the mod
  UIGraphics\Res_Eyedropper.RT              registers the button art (UIGraphics.far is untouched)
  UIGraphics\Res_Eyedropper.h               its resource id
  UIGraphics\Eyedropper\EyedropperBtn.bmp   the build panel button
  UIGraphics\Eyedropper\Eyedropper.cur      the eyedropper cursor

Uninstall: delete these files from the game folder:
  winmm.dll                (keep it if another mod in the mods folder uses it)
  mods\{DLL}
  mods\eyedropper.dll  (older versions of the mod)
  UIGraphics\Res_Eyedropper.RT
  UIGraphics\Res_Eyedropper.h
  UIGraphics\Eyedropper\EyedropperBtn.bmp
  UIGraphics\Eyedropper\Eyedropper.cur
  mod.log (if present)
Then delete the UIGraphics\Eyedropper folder, and the mods folder if nothing else is in it.

If you report a problem, attach mod.log from the game folder.
