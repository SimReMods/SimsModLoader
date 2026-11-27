// game_addr_ea.h - every EA app Sims.exe address the all_cheats mod uses (link-time VAs, base 0x400000): the Steam
// code relinked, AllowCheat sits in the same block as cTSWin / cTSWinBtn (moved by 0x28e430, as there); its bytes were
// checked against the EA app build.

GA_FN(cTSCheatCodeManager__AllowCheat,              0x974aa0)    // bool (const char *pszName), thiscall
GA_FN(cTSCheatCodeManager__RegisterCheat,           0x9753f0)    // void (name, callback, user data, usage), thiscall
