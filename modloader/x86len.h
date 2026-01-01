// x86len.h - minimal 32-bit x86 length disassembler for relocating function prologues into a trampoline.
#pragma once

struct X86Insn
{
    unsigned char len;          // total length, 0 = unknown / unsupported opcode
    unsigned char relPos;       // offset of the branch displacement inside the instruction (0 = none)
    unsigned char relSize;      // 1 or 4 when the instruction is an eip-relative branch (call/jmp/jcc/loop)
    unsigned char opcode;       // first opcode byte after the prefixes (0x0F for two-byte opcodes: see opcode2)
    unsigned char opcode2;
    bool isRet;                 // ret / retn imm16 / iret
    bool isCall;                // E8 (rel32 call)
    bool isJmp;                 // unconditional jmp (E9 / EB) - control does not continue
    bool isLoop;                // E0..E3 (rel8 only; cannot be widened)
};

// Decodes one instruction at p (at most 15 bytes are read). Returns insn.len, 0 if unsupported.
int X86_Decode(const unsigned char *p, X86Insn &insn);
