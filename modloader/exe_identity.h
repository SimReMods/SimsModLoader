// exe_identity.h - the game build this loader is made for: entry point bytes, .text range and hash, PE identity
#pragma once
#include "simsmod.h"

#define ORIG_OEP_RVA 0x3a8a86u
#define ORIG_TEXT_RVA 0x001000u
#define ORIG_TEXT_SIZE 0x579ac3u
#define ORIG_TEXT_FNV64 0x7885f456ea632f04ull     // FNV-1a 64 of the decrypted .text, reloc sites zeroed
#define ORIG_PACKED_TIMESTAMP 0x67e72e1bu
#define ORIG_PACKED_CHECKSUM 0x006ce782u
#define ORIG_PACKED_SIZEOFIMAGE 0x00702000u
#define ORIG_PACKED_FILESIZE 7125288u
static const OrigSig ORIG_SIG_7a8a86 = { 0x3a8a86u, 16, { 0xe8, 0x40, 0x0b, 0x00, 0x00, 0xe9, 0x35, 0xfe, 0xff, 0xff, 0x8b, 0x44, 0x24, 0x08, 0x8b, 0x4c }, 0x0000 };
