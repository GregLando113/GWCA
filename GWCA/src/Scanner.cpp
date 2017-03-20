#include "..\Utilities\Scanner.h"

#include <Windows.h>
#include <Psapi.h>
#include <cstring>

namespace {
	uintptr_t g_base = 0;
	size_t g_size    = 0;
}

uintptr_t GW::Scanner::Find(char* pattern, char* mask, int offset) {
	BYTE first = pattern[0];
	int patternLength = strlen(mask);
	bool found = false;

	//For each byte from start to end
	for (DWORD i = g_base; i < g_base + g_size - patternLength; i++) {
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

void GW::Scanner::Initialize(void* module) {
	MODULEINFO info;
	if (!GetModuleInformation(GetCurrentProcess(), (HMODULE)module, &info, sizeof(MODULEINFO)))
		throw 1;

	g_base = (DWORD)info.lpBaseOfDll;
	g_size = (DWORD)info.SizeOfImage;
}

void GW::Scanner::Initialize(char* moduleName) {
	HMODULE mod = GetModuleHandleA(moduleName);
	LPVOID textSection = (LPVOID)((DWORD)mod + 0x1000);

	MEMORY_BASIC_INFORMATION info = { 0 };

	if (VirtualQuery(textSection, &info, sizeof(MEMORY_BASIC_INFORMATION))) {
		g_base = (uintptr_t)textSection;
		g_size = (DWORD)info.RegionSize;
	} else {
		throw 1;
	}
}

void GW::Scanner::Initialize(uintptr_t start, size_t size) {
	g_base = start;
	g_size = size;
}
