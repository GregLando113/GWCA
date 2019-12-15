#pragma once

#include <GWCA/Utilities/Export.h>

namespace GW {

    struct MemoryMgr {

        // Skill timer for effects.
        static uintptr_t SkillTimerPtr;

        static uintptr_t WinHandlePtr;

        static uintptr_t GetPersonalDirPtr;

        // Basics
        static bool Scan();

        static uint32_t GetSkillTimer() {
            return GetTickCount() + *(uintptr_t *)SkillTimerPtr;
        }

        static HWND GetGWWindowHandle() {
            return *(HWND *)WinHandlePtr;
        }
    };
}
