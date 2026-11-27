// game_addr_cc.h - every Complete Collection (2003, VC6) Sims.exe address the all_cheats mod uses (link-time VAs, base
// 0x400000, no relocations). Same entries as game_addr.h.

// the function the mod wraps (detoured at its entry). 2003: the manager's flag (+0x48) switches the short list ON, and
// the app always sets it, so the same 28 cheats are available as in the Legacy Collection
GA_FN(cTSCheatCodeManager__AllowCheat,              0x5e6990)    // bool (const char *pszName), thiscall
