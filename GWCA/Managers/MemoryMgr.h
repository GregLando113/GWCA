#pragma once

#include <Windows.h>
#include <stdio.h>

namespace GW {

	struct MemoryMgr {

		// Agent shit
		static BYTE* agArrayPtr;

		// Base ptr to get context ptr for gameworld
		static BYTE* BasePointerLocation;

		// Disable/Enable Rendering
		static BYTE* RenderLoopLocation;

		// For gamethread calls
		static BYTE* GameLoopLocation;
		static BYTE* GameLoopReturn;
		static BYTE* GameLoopRestore;

		// Skill timer for effects.
		static BYTE* SkillTimerPtr;

		static BYTE* WinHandlePtr;

		static BYTE* GStoCBase;

        static BYTE* AsyncDecodeStringPtr;

		static BYTE* GetPersonalDirPtr;

		// Basics
		static bool Scan();
		template <typename T> static T ReadPtrChain(DWORD _base, DWORD _amount_of_offsets, ...) {
			va_list vl;

			va_start(vl, _amount_of_offsets);
			while (_amount_of_offsets--) {
				_base = (*(DWORD*)_base);
				if (_base) {
					_base += va_arg(vl, DWORD);
				} else {
					return NULL;
				}
			}
			va_end(vl);

			return (T)_base;
		}

		// Memory Reads.
		inline static DWORD GetContextPtr() { return (*(DWORD*)BasePointerLocation) + 0x18; }
		inline static DWORD GetSkillTimer() { return GetTickCount() + *(DWORD*)SkillTimerPtr; }
		inline static HWND GetGWWindowHandle() { return *(HWND*)WinHandlePtr; }
	};
}
