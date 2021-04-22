#pragma once

namespace GW {

    // class PatternScanner
    // 32 bit pattern scanner for x86 programs.
    // Credits to Zat & Midi12 @ unknowncheats.me for the functionality of this class.
    namespace Scanner {

        // Initializer to determine scan range.
        void Initialize(uintptr_t start, size_t size);
        void Initialize(const char* moduleName = NULL);

        enum Section : uint8_t {
            TEXT,
            RDATA
        };

        // Find reference in GW memory to a specific assertion message
        uintptr_t FindAssertion(char* assertion_file, char* assertion_msg, int offset = 0);

        // Pattern find between a start and end address. If end is less than start, will scan backward.
        uintptr_t FindInRange(const char* pattern, const char* mask, int offset, DWORD start, DWORD end);

        // Actual pattern finder.
        uintptr_t Find(const char* pattern, const char* mask, int offset = 0, Section section = Section::TEXT);
    }
}