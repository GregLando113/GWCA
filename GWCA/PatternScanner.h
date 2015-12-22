#pragma once

#include <Windows.h>

namespace GWAPI {

	class PatternScanner {
	public:

		// Initializer to determine scan range.
		PatternScanner(DWORD _start, DWORD _size);
		PatternScanner(char* moduleName = NULL);
		PatternScanner(HMODULE _module);

		// Actual pattern finder.
		const void* FindPattern(const char* pattern,const char* mask, DWORD offset);

	private:
		const BYTE* base_;
		DWORD size_;
	};

}