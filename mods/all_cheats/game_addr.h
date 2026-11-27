// game_addr.h - every Sims.exe address the all_cheats mod uses (link-time VAs, base 0x400000; Legacy Collection
// 1.10.7.0, Steam). GA_FN(name, va): a function ("A__B" is A::B). gen_sigs.h holds its 16-byte signature.

// the functions the mod wraps (detoured at their entry). The Legacy build has AllowCheat inlined into RegisterCheat (it
// is never called), so the mod wraps RegisterCheat there; AllowCheat itself is still called for the name check in mod.log
GA_FN(cTSCheatCodeManager__AllowCheat,              0x6e6670)    // bool (const char *pszName), thiscall
GA_FN(cTSCheatCodeManager__RegisterCheat,           0x6e6fc0)    // void (name, callback, user data, usage), thiscall
