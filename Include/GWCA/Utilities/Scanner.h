#pragma once

namespace GW {

    // class PatternScanner
    // 32 bit pattern scanner for x86 programs.
    // Credits to Zat & Midi12 @ unknowncheats.me for the functionality of this class.
    namespace Scanner {

        // Initializer to determine scan range.
        void Initialize(uintptr_t start, size_t size);
        void Initialize(const char* moduleName = NULL);
        void Initialize(void* module);

        // Actual pattern finder.
        uintptr_t Find(const char* pattern, const char* mask, int offset);
    }
}
