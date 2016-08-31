#pragma once

#include <Windows.h>

namespace GW {

	// class PatternScanner
	// 32 bit pattern scanner for x86 programs.
	// Credits to Zat & Midi12 @ unknowncheats.me for the functionality of this class.
	class PatternScanner {
	public:

		// Initializer to determine scan range.
		PatternScanner(DWORD _start, DWORD _size);
		PatternScanner(char* moduleName = NULL);
		PatternScanner(HMODULE _module);

		// Actual pattern finder.
		DWORD FindPattern(char* pattern, char* mask, DWORD offset);

	private:
		DWORD base_;
		DWORD size_;
	};
}
