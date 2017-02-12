#include "..\..\Utilities\PatternScanner.h"
#include <Psapi.h>

DWORD GW::PatternScanner::FindPattern(char* pattern, char* mask, DWORD offset) {
	BYTE first = pattern[0];
	int patternLength = strlen(mask);
	bool found = false;

	//For each byte from start to end
	for (DWORD i = base_; i < base_ + size_ - patternLength; i++) {
		if (*(BYTE*)i != first) {
			continue;
		}
		found = true;
		//For each byte in the pattern
		for (int idx = 0; idx < patternLength; idx++) {

			if (mask[idx] == 'x' && pattern[idx] != *(char*)(i + idx)) {
				found = false;
				break;
			}
		}
		if (found) {
			return i + offset;
		}
	}
	return NULL;
}

GW::PatternScanner::PatternScanner(HMODULE _module) {
	MODULEINFO info;
	if (!GetModuleInformation(GetCurrentProcess(), _module, &info, sizeof(MODULEINFO)))
		throw 1;

	base_ = (DWORD)info.lpBaseOfDll;
	size_ = (DWORD)info.SizeOfImage;
}

GW::PatternScanner::PatternScanner(char* moduleName /*= NULL*/) {
	HMODULE mod = GetModuleHandleA(moduleName);
	LPVOID textSection = (LPVOID)((DWORD)mod + 0x1000);

	MEMORY_BASIC_INFORMATION info = { 0 };

	if (VirtualQuery(textSection, &info, sizeof(MEMORY_BASIC_INFORMATION))) {
		base_ = (DWORD)textSection;
		size_ = (DWORD)info.RegionSize;
	} else {
		throw 1;
	}
}

GW::PatternScanner::PatternScanner(DWORD _start, DWORD _size) : base_(_start), size_(_size) {}
