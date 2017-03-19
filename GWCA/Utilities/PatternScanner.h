#pragma once

#include <cstdint>


namespace GW {

	// class PatternScanner
	// 32 bit pattern scanner for x86 programs.
	// Credits to Zat & Midi12 @ unknowncheats.me for the functionality of this class.
	namespace PatternScanner {

		// Initializer to determine scan range.
		void Init(uintptr_t start, size_t size);
		void Init(char* moduleName = NULL);
		void Init(void* module);

		// Actual pattern finder.
		uintptr_t FindPattern(char* pattern, char* mask, int offset);

	}
}
