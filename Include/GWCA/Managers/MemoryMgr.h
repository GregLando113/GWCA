#pragma once

namespace GW {

    struct MemoryMgr {

        // Skill timer for effects.
        static DWORD* SkillTimerPtr;

        static uintptr_t WinHandlePtr;

        static uintptr_t GetPersonalDirPtr;

        static uint32_t GetGWVersion();

        // Basics
        static bool Scan();

        static DWORD GetSkillTimer() {
            return GetTickCount() + *SkillTimerPtr;
        }

        static HWND GetGWWindowHandle() {
            return *reinterpret_cast<HWND*>(WinHandlePtr);
        }
    };
}
