#pragma once

namespace GW {

    // class PatternScanner
    // 32 bit pattern scanner for x86 programs.
    // Credits to Zat & Midi12 @ unknowncheats.me for the functionality of this class.
    namespace Scanner {

        // Initializer to determine scan range.
        void Initialize(uintptr_t start, size_t size);
        void Initialize(char* moduleName = NULL);
        void Initialize(void* module);

        // Actual pattern finder.
        uintptr_t Find(char* pattern, char* mask, int offset);

        inline uintptr_t DecodeE8(uintptr_t addr) {
            assert(addr);
            uint8_t op = *(uint8_t *)addr;
            assert(op == 0xE8);
            uint32_t rel = *(uint32_t *)(addr + 1);
            uint32_t real = rel + addr + 5;
            return real;
        }
    }
}
