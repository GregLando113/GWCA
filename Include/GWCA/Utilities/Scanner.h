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
            RDATA,
            DATA
        };

        // Find reference in GW memory to a specific assertion message
        uintptr_t FindAssertion(const char* assertion_file, const char* assertion_msg, int offset = 0);

        // Pattern find between a start and end address. If end is less than start, will scan backward.
        uintptr_t FindInRange(const char* pattern, const char* mask, int offset, DWORD start, DWORD end);

        // Actual pattern finder.
        uintptr_t Find(const char* pattern, const char* mask = 0, int offset = 0, Section section = Section::TEXT);

        // Check if current address is a valid pointer (usually to a data variable in DATA)
        bool IsValidPtr(uintptr_t address, Section section = Section::DATA);

        // Returns actual address of a function call given via CALL <near call> instruction e.g. *call_instruction_address = 0xE8 ?? ?? ?? 0xFF
        uintptr_t FunctionFromNearCall(uintptr_t call_instruction_address);

        struct MemoryPageFilter {
            uint32_t state;
            uint32_t type;
            uint32_t protection;
        };

        // Searches a specific memory page and finds the address to the first occurrence of the specified bytes
        uint8_t* FindOnMemoryPage(uint8_t* page_ptr, size_t page_size, uint8_t* value_ptr, size_t value_size);

        // Searches all memory pages and finds the address to the first occurrence of the specified bytes
        uint8_t* FindOnMemoryPages(MemoryPageFilter filter, uint8_t* value_ptr, size_t value_size);

    }

}
