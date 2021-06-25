#include "stdafx.h"

#include <Psapi.h>

#include <GWCA/Utilities/Scanner.h>
#include <DbgHelp.h>

#pragma comment( lib, "dbghelp.lib" )

namespace {
    struct SectionOffset {
        uintptr_t start = 0;
        uintptr_t end = 0;
    };
    SectionOffset sections[2] = { 0 };
}
uintptr_t GW::Scanner::FindAssertion(char* assertion_file, char* assertion_msg, int offset) {
#pragma warning( push )
#pragma warning( disable : 4838 )
#pragma warning( disable : 4242 )
#pragma warning( disable : 4244 )
#pragma warning( disable : 4365 )
    char mask[64];
    int i;
    char assertion_bytes[] = "\xBA????\xB9????";
    char assertion_mask[] = "x????x????";
    if (assertion_msg && assertion_msg[0]) {
        for (i = 0; assertion_msg[i]; i++) {
            mask[i] = 'x';
        }
        mask[i++] = 'x'; // Include terminating char
        mask[i] = 0;
        uint32_t rdata_addr = Find(assertion_msg, mask, 0, Section::RDATA);
        if (!rdata_addr)
            return 0;

        assertion_bytes[6] = rdata_addr;
        assertion_bytes[7] = rdata_addr >> 8;
        assertion_bytes[8] = rdata_addr >> 16;
        assertion_bytes[9] = rdata_addr >> 24;

        memcpy(&assertion_mask[6], "xxxx",4);
    }
    if (assertion_file) {
        for (i = 0; assertion_file[i]; i++) {
            mask[i] = 'x';
        }
        mask[i++] = 'x'; // Include terminating char
        mask[i] = 0;
        uint32_t rdata_addr = Find(assertion_file, mask, 0, Section::RDATA);
        if (!rdata_addr)
            return 0;
        assertion_bytes[1] = rdata_addr;
        assertion_bytes[2] = rdata_addr >> 8;
        assertion_bytes[3] = rdata_addr >> 16;
        assertion_bytes[4] = rdata_addr >> 24;
        memcpy(&assertion_mask[1], "xxxx",4);
    }
#pragma warning(pop)
    return Find(assertion_bytes, assertion_mask, offset);
}
uintptr_t GW::Scanner::FindInRange(const char* pattern, const char* mask, int offset, DWORD start, DWORD end) {
    char first = pattern[0];
    size_t patternLength = strlen(mask);
    bool found = false;
    end -= patternLength;

    if (start > end) {
        // Scan backward
        for (DWORD i = start; i >= end ; i--) {
            if (*(char*)i != first)
                continue;
            found = true;
            //For each byte in the pattern
            for (size_t idx = 0; idx < patternLength; idx++) {
                if (mask[idx] == 'x' && pattern[idx] != *(char*)(i + idx)) {
                    found = false;
                    break;
                }
            }
            if (found)
                return i + offset;
        }
    }
    else {
        // Scan forward
        for (DWORD i = start; i < end; i++) {
            if (*(char*)i != first)
                continue;
            found = true;
            //For each byte in the pattern
            for (size_t idx = 0; idx < patternLength; idx++) {
                if (mask[idx] == 'x' && pattern[idx] != *(char*)(i + idx)) {
                    found = false;
                    break;
                }
            }
            if (found)
                return i + offset;
        }
    }
    return NULL;
}
uintptr_t GW::Scanner::Find(const char* pattern, const char* mask, int offset, Section section) {
    return FindInRange(pattern, mask, offset, sections[section].start, sections[section].end);
}

void GW::Scanner::Initialize(const char* moduleName) {
	HMODULE hModule = GetModuleHandleA(moduleName);
	uint32_t dllImageBase = (uint32_t)hModule;
	IMAGE_NT_HEADERS* pNtHdr = ImageNtHeader(hModule);
	IMAGE_SECTION_HEADER* pSectionHdr = (IMAGE_SECTION_HEADER*)(pNtHdr + 1);
	//iterate through the list of all sections, and check the section name in the if conditon. etc
	for (int i = 0; i < pNtHdr->FileHeader.NumberOfSections; i++)
	{
		char* name = (char*)pSectionHdr->Name;
        uint8_t section = 0x8;
        if (memcmp(name, ".text", 5) == 0)
            section = Section::TEXT;
        else if (memcmp(name, ".rdata", 6) == 0)
            section = Section::RDATA;
        if (section != 0x8) {
            sections[section].start = dllImageBase + pSectionHdr->VirtualAddress;
            sections[section].end = sections[section].start + pSectionHdr->Misc.VirtualSize;
        }
        pSectionHdr++;
	}
    if (!(sections[Section::TEXT].start && sections[Section::TEXT].end))
        throw 1;
}

void GW::Scanner::Initialize(uintptr_t start, size_t size) {
    sections[Section::TEXT].start = start;
    sections[Section::TEXT].end = start + size;
}
