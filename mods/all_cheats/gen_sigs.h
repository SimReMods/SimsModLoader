// generated from game_addr.h and the Steam Legacy Collection Sims.exe: one 16-byte signature per
// entry (relocmask: bytes that are relocated dwords), the number of bytes the mod overwrites, and the original
// instructions there
#pragma once
struct GenSig { unsigned va; unsigned char patchLen; OrigSig sig; };
static const GenSig g_GenSigs[] = {
    { 0x6e6670, 0, { 0x2e6670, 16, { 0x55, 0x8b, 0xec, 0x80, 0x79, 0x4c, 0x00, 0x56, 0x57, 0x75, 0x46, 0x8b, 0x7d, 0x08, 0x33, 0xf6 }, 0x0000 } },   // cTSCheatCodeManager__AllowCheat: push ebp | mov ebp, esp | cmp byte ptr [ecx + 0x4c], 0 | push esi | push edi | jne 0x6e66c1 | mov edi, dword ptr [ebp + 8] | xor esi, esi
    { 0x6e6fc0, 0, { 0x2e6fc0, 16, { 0x55, 0x8b, 0xec, 0x6a, 0xff, 0x68, 0xd5, 0xf6, 0x96, 0x00, 0x64, 0xa1, 0x00, 0x00, 0x00, 0x00 }, 0x0040 } },   // cTSCheatCodeManager__RegisterCheat: push ebp | mov ebp, esp | push -1 | push 0x96f6d5 | mov eax, dword ptr fs:[0]
};
