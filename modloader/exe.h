// exe.h - the loader's view of Sims.exe in memory
#pragma once

namespace orig
{
    extern unsigned int base;                                   // actual module base of Sims.exe (ASLR applies)
    extern unsigned int delta;                                  // base - 0x400000 (link-time base)
    inline unsigned int rva(unsigned int r) { return base + r; }
    template <class T> inline T *ptr(unsigned int r) { return reinterpret_cast<T *>(base + r); }
}
