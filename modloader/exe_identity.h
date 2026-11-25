// exe_identity.h - the game builds this loader knows: PE identity, entry point bytes, .text range and hash
#pragma once
#include "simsmod.h"

struct GameBuild
{
    unsigned id;                        // SIMSMOD_GAME_*
    const char *pszName;
    unsigned timestamp, checksum, sizeOfImage;
    unsigned textRva, textSize;
    unsigned long long textFnv64;       // FNV-1a 64 of .text once it is runnable, reloc sites zeroed
    OrigSig oep;                        // entry point bytes (Legacy: present only after the Steam stub decrypted .text)
    unsigned relocRva, relocSize;       // reloc table for the hash mask; 0 = the PE header's (EA: the header points at
                                        // the OOA stub's own table, the game's is the .reloc section)
};

static const GameBuild g_GameBuilds[] = {
    { SIMSMOD_GAME_LEGACY, "The Sims Legacy Collection 1.10.7.0 (Steam)",
      0x67e72e1bu, 0x006ce782u, 0x00702000u, 0x001000u, 0x579ac3u, 0x7885f456ea632f04ull,
      { 0x3a8a86u, 16, { 0xe8, 0x40, 0x0b, 0x00, 0x00, 0xe9, 0x35, 0xfe, 0xff, 0xff, 0x8b, 0x44, 0x24, 0x08, 0x8b, 0x4c }, 0x0000 }, 0, 0 },
    { SIMSMOD_GAME_LEGACY_EA, "The Sims Legacy Collection 1.10.7 (EA app)",
      0x67e72d66u, 0x00a6de54u, 0x00aa3000u, 0x001000u, 0x85ce63u, 0x47ef26ca627f3948ull,
      { 0x637fc6u, 16, { 0xe8, 0x90, 0x0d, 0x00, 0x00, 0xe9, 0x35, 0xfe, 0xff, 0xff, 0x8b, 0x44, 0x24, 0x08, 0x8b, 0x4c }, 0x0000 },
      0xa42000u, 0x5f0b8u },
    { SIMSMOD_GAME_COMPLETE, "The Sims Complete Collection (2003)",
      0x3f735f2bu, 0x00000000u, 0x0030d39eu, 0x001000u, 0x263b29u, 0x538a5f9822d4a906ull,
      { 0x238274u, 16, { 0x55, 0x8b, 0xec, 0x6a, 0xff, 0x68, 0x60, 0x93, 0x68, 0x00, 0x68, 0xd0, 0x83, 0x63, 0x00, 0x64 }, 0x0000 }, 0, 0 },
};
