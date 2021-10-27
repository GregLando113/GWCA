#pragma once

#include <GWCA/Utilities/Export.h>

namespace GW {

    struct MemoryMgr {

        // Skill timer for effects.
        static DWORD* SkillTimerPtr;

        static uintptr_t WinHandlePtr;

        static uintptr_t GetPersonalDirPtr;

        static uint32_t(_cdecl* GetGWVersion)(void);

        // Basics
        static bool Scan();

        static DWORD GetSkillTimer() {
            return GetTickCount() + *(DWORD*)SkillTimerPtr;
        }

        static HWND GetGWWindowHandle() {
            return *(HWND *)WinHandlePtr;
        }
    };
}
