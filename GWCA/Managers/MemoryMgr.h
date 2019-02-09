#pragma once

namespace GW {

    struct MemoryMgr {

        // Agent shit
        static uintptr_t AgentArrayPtr;

        // Base ptr to get context ptr for gameworld
        static uintptr_t BasePointerLocation;

        // Disable/Enable Rendering
        static uintptr_t RenderLoopLocation;

        // For gamethread calls
        static uintptr_t GameLoopLocation;
        static uintptr_t GameLoopReturn;
        static uintptr_t GameLoopRestore;

        // Skill timer for effects.
        static uintptr_t SkillTimerPtr;

        static uintptr_t WinHandlePtr;

        static uintptr_t GStoCBase;

        static uintptr_t AsyncDecodeStringPtr;

        static uintptr_t GetPersonalDirPtr;

        // Basics
        static bool Scan();

        // Memory Reads.
        inline uintptr_t GetContextPtr() {
            return (*(uintptr_t *)BasePointerLocation) + 0x18;
        }

        static uint32_t GetSkillTimer() {
            return GetTickCount() + *(uintptr_t *)SkillTimerPtr;
        }

        static HWND GetGWWindowHandle() {
            return *(HWND *)WinHandlePtr;
        }
    };
}
