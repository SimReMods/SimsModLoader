// x86len.cpp - table-driven length disassembler for the instruction forms that occur in MSVC function prologues
// (and most of the rest of the 32-bit integer / x87 / SSE instruction set). Unknown opcodes return 0.
#include <string.h>
#include "x86len.h"

enum
{
    F_NONE = 0,
    F_MODRM = 1,        // ModRM (+SIB, +disp) follows
    F_IMM8 = 2,         // 1-byte immediate
    F_IMMZ = 4,         // 4-byte immediate (2 bytes with a 66h operand-size prefix)
    F_IMM16 = 8,        // 2-byte immediate (ret imm16, enter)
    F_REL8 = 16,        // eip-relative 8-bit displacement
    F_REL32 = 32,       // eip-relative 32-bit displacement
    F_MOFFS = 64,       // 4-byte absolute address (A0..A3)
    F_PREFIX = 128,     // legacy prefix byte
    F_BAD = 256,        // unsupported (far call/jmp, etc.)
    F_GRP3 = 512,       // F6/F7: imm follows when reg field is 0 or 1
};

static const unsigned short s_tab1[256] = {
    /*00*/ F_MODRM, F_MODRM, F_MODRM, F_MODRM, F_IMM8, F_IMMZ, F_NONE, F_NONE,  F_MODRM, F_MODRM, F_MODRM, F_MODRM, F_IMM8, F_IMMZ, F_NONE, F_NONE,
    /*10*/ F_MODRM, F_MODRM, F_MODRM, F_MODRM, F_IMM8, F_IMMZ, F_NONE, F_NONE,  F_MODRM, F_MODRM, F_MODRM, F_MODRM, F_IMM8, F_IMMZ, F_NONE, F_NONE,
    /*20*/ F_MODRM, F_MODRM, F_MODRM, F_MODRM, F_IMM8, F_IMMZ, F_PREFIX, F_NONE, F_MODRM, F_MODRM, F_MODRM, F_MODRM, F_IMM8, F_IMMZ, F_PREFIX, F_NONE,
    /*30*/ F_MODRM, F_MODRM, F_MODRM, F_MODRM, F_IMM8, F_IMMZ, F_PREFIX, F_NONE, F_MODRM, F_MODRM, F_MODRM, F_MODRM, F_IMM8, F_IMMZ, F_PREFIX, F_NONE,
    /*40*/ F_NONE, F_NONE, F_NONE, F_NONE, F_NONE, F_NONE, F_NONE, F_NONE,  F_NONE, F_NONE, F_NONE, F_NONE, F_NONE, F_NONE, F_NONE, F_NONE,
    /*50*/ F_NONE, F_NONE, F_NONE, F_NONE, F_NONE, F_NONE, F_NONE, F_NONE,  F_NONE, F_NONE, F_NONE, F_NONE, F_NONE, F_NONE, F_NONE, F_NONE,
    /*60*/ F_NONE, F_NONE, F_MODRM, F_MODRM, F_PREFIX, F_PREFIX, F_PREFIX, F_PREFIX,  F_IMMZ, F_MODRM | F_IMMZ, F_IMM8, F_MODRM | F_IMM8, F_NONE, F_NONE, F_NONE, F_NONE,
    /*70*/ F_REL8, F_REL8, F_REL8, F_REL8, F_REL8, F_REL8, F_REL8, F_REL8,  F_REL8, F_REL8, F_REL8, F_REL8, F_REL8, F_REL8, F_REL8, F_REL8,
    /*80*/ F_MODRM | F_IMM8, F_MODRM | F_IMMZ, F_MODRM | F_IMM8, F_MODRM | F_IMM8, F_MODRM, F_MODRM, F_MODRM, F_MODRM,  F_MODRM, F_MODRM, F_MODRM, F_MODRM, F_MODRM, F_MODRM, F_MODRM, F_MODRM,
    /*90*/ F_NONE, F_NONE, F_NONE, F_NONE, F_NONE, F_NONE, F_NONE, F_NONE,  F_NONE, F_NONE, F_BAD, F_NONE, F_NONE, F_NONE, F_NONE, F_NONE,
    /*A0*/ F_MOFFS, F_MOFFS, F_MOFFS, F_MOFFS, F_NONE, F_NONE, F_NONE, F_NONE,  F_IMM8, F_IMMZ, F_NONE, F_NONE, F_NONE, F_NONE, F_NONE, F_NONE,
    /*B0*/ F_IMM8, F_IMM8, F_IMM8, F_IMM8, F_IMM8, F_IMM8, F_IMM8, F_IMM8,  F_IMMZ, F_IMMZ, F_IMMZ, F_IMMZ, F_IMMZ, F_IMMZ, F_IMMZ, F_IMMZ,
    /*C0*/ F_MODRM | F_IMM8, F_MODRM | F_IMM8, F_IMM16, F_NONE, F_MODRM, F_MODRM, F_MODRM | F_IMM8, F_MODRM | F_IMMZ,  F_IMM16 | F_IMM8, F_NONE, F_IMM16, F_NONE, F_NONE, F_IMM8, F_NONE, F_NONE,
    /*D0*/ F_MODRM, F_MODRM, F_MODRM, F_MODRM, F_IMM8, F_IMM8, F_NONE, F_NONE,  F_MODRM, F_MODRM, F_MODRM, F_MODRM, F_MODRM, F_MODRM, F_MODRM, F_MODRM,
    /*E0*/ F_REL8, F_REL8, F_REL8, F_REL8, F_IMM8, F_IMM8, F_IMM8, F_IMM8,  F_REL32, F_REL32, F_BAD, F_REL8, F_NONE, F_NONE, F_NONE, F_NONE,
    /*F0*/ F_PREFIX, F_NONE, F_PREFIX, F_PREFIX, F_NONE, F_NONE, F_MODRM | F_GRP3, F_MODRM | F_GRP3,  F_NONE, F_NONE, F_NONE, F_NONE, F_NONE, F_NONE, F_MODRM, F_MODRM,
};

static unsigned short tab2(unsigned char op)
{
    if (op >= 0x80 && op <= 0x8F) return F_REL32;                       // jcc rel32
    if (op >= 0x90 && op <= 0x9F) return F_MODRM;                       // setcc
    if (op >= 0x40 && op <= 0x4F) return F_MODRM;                       // cmovcc
    if (op >= 0x10 && op <= 0x17) return F_MODRM;                       // movups/movss/movhps...
    if (op >= 0x18 && op <= 0x1F) return F_MODRM;                       // prefetch / multi-byte nop
    if (op >= 0x28 && op <= 0x2F) return F_MODRM;                       // movaps, cvt*, ucomiss, comiss
    if (op >= 0x50 && op <= 0x6F) return F_MODRM;                       // SSE arithmetic, punpck*
    if (op >= 0x70 && op <= 0x73) return F_MODRM | F_IMM8;              // pshuf*, psrl/psra/psll imm
    if (op >= 0x74 && op <= 0x7F) return F_MODRM;                       // pcmpeq*, emms(77: modrm-less but harmless), movd/movq
    if (op >= 0xC8 && op <= 0xCF) return F_NONE;                        // bswap
    if (op >= 0xD0 && op <= 0xFF) return F_MODRM;                       // MMX / SSE2 integer
    switch (op)
    {
    case 0x00: case 0x01: case 0x02: case 0x03: case 0x0D: return F_MODRM;
    case 0x05: case 0x06: case 0x07: case 0x08: case 0x09: case 0x0B: case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35:
    case 0xA0: case 0xA1: case 0xA2: case 0xA8: case 0xA9: return F_NONE;
    case 0xA3: case 0xA5: case 0xAB: case 0xAD: case 0xAE: case 0xAF: case 0xB0: case 0xB1: case 0xB3: case 0xB6: case 0xB7:
    case 0xB8: case 0xBB: case 0xBC: case 0xBD: case 0xBE: case 0xBF: case 0xC0: case 0xC1: case 0xC3: case 0xC7: return F_MODRM;
    case 0xA4: case 0xAC: case 0xBA: case 0xC2: case 0xC4: case 0xC5: case 0xC6: return F_MODRM | F_IMM8;
    case 0x38: return F_MODRM | 1024;                                   // 3-byte opcode 0F 38 xx (modrm)
    case 0x3A: return F_MODRM | F_IMM8 | 1024;                          // 3-byte opcode 0F 3A xx ib
    }
    return F_BAD;
}

int X86_Decode(const unsigned char *p, X86Insn &insn)
{
    memset(&insn, 0, sizeof insn);
    int i = 0;
    bool opsize = false;
    for (; i < 4; ++i)
    {
        unsigned char b = p[i];
        if (b == 0x66) { opsize = true; continue; }
        if (b == 0x67 || b == 0xF0 || b == 0xF2 || b == 0xF3 || b == 0x26 || b == 0x2E || b == 0x36 || b == 0x3E || b == 0x64 || b == 0x65) continue;
        break;
    }
    unsigned char op = p[i++];
    unsigned short f;
    insn.opcode = op;
    if (op == 0x0F)
    {
        insn.opcode2 = p[i++];
        f = tab2(insn.opcode2);
        if (f & 1024) ++i;                                              // third opcode byte
    }
    else
    {
        f = s_tab1[op];
        if (f & F_PREFIX) return 0;                                      // too many prefixes
        if (op == 0xC3 || op == 0xC2 || op == 0xCF) insn.isRet = true;
        if (op == 0xE8) insn.isCall = true;
        if (op == 0xE9 || op == 0xEB) insn.isJmp = true;
        if (op >= 0xE0 && op <= 0xE3) insn.isLoop = true;
    }
    if (f & F_BAD) return 0;
    if (f & F_MODRM)
    {
        unsigned char modrm = p[i++];
        unsigned char mod = modrm >> 6, rm = modrm & 7, reg = (modrm >> 3) & 7;
        if (mod != 3 && rm == 4)
        {
            unsigned char sib = p[i++];
            if (mod == 0 && (sib & 7) == 5) i += 4;
        }
        if (mod == 1) i += 1;
        else if (mod == 2) i += 4;
        else if (mod == 0 && rm == 5) i += 4;
        if ((f & F_GRP3) && reg <= 1) f |= (op == 0xF6) ? F_IMM8 : F_IMMZ;
    }
    if (f & F_MOFFS) i += 4;
    if (f & F_IMM16) i += 2;
    if (f & F_IMM8) i += 1;
    if (f & F_IMMZ) i += opsize ? 2 : 4;
    if (f & F_REL8) { insn.relPos = (unsigned char)i; insn.relSize = 1; i += 1; }
    if (f & F_REL32) { insn.relPos = (unsigned char)i; insn.relSize = 4; i += 4; }
    if (i > 15) return 0;
    insn.len = (unsigned char)i;
    return i;
}
