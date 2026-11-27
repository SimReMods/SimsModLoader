// generated from game_addr_cc.h and the Complete Collection Sims.exe (TimeDateStamp 0x3f735f2b): one 16-byte signature per
// entry (relocmask 0: the 2003 exe has no relocations), the number of bytes the mod overwrites, and the original
// instructions there
#pragma once
struct GenSig { unsigned va; unsigned char patchLen; OrigSig sig; };
static const GenSig g_GenSigs[] = {
    { 0x5e6990, 0, { 0x1e6990, 16, { 0x80, 0x79, 0x48, 0x00, 0x56, 0x75, 0x04, 0xb0, 0x01, 0xeb, 0x23, 0xbe, 0x2c, 0x75, 0x6d, 0x00 }, 0x0000 } },   // cTSCheatCodeManager__AllowCheat: cmp byte ptr [ecx + 0x48], 0 | push esi | jne 0x5e699b | mov al, 1 | jmp 0x5e69be | mov esi, 0x6d752c
};
