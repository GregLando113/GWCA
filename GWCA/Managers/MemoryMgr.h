#pragma once

namespace GW {

    struct MemoryMgr {

        // Agent shit
        static uint8_t* agArrayPtr;

        // Base ptr to get context ptr for gameworld
        static uint8_t* BasePointerLocation;

        // Disable/Enable Rendering
        static uint8_t* RenderLoopLocation;

        // For gamethread calls
        static uint8_t* GameLoopLocation;
        static uint8_t* GameLoopReturn;
        static uint8_t* GameLoopRestore;

        // Skill timer for effects.
        static uint8_t* SkillTimerPtr;

        static uint8_t* WinHandlePtr;

        static uint8_t* GStoCBase;

        static uint8_t* AsyncDecodeStringPtr;

        static uint8_t* GetPersonalDirPtr;

        // Basics
        static bool Scan();
        template <typename T> static T ReadPtrChain(uint32_t _base, uint32_t _amount_of_offsets, ...) {
            va_list vl;

            va_start(vl, _amount_of_offsets);
            while (_amount_of_offsets--) {
                _base = (*(uint32_t*)_base);
                if (_base) {
                    _base += va_arg(vl, uint32_t);
                } else {
                    return NULL;
                }
            }
            va_end(vl);

            return (T)_base;
        }

        // Memory Reads.
        inline static uint32_t GetContextPtr() { return (*(uint32_t*)BasePointerLocation) + 0x18; }
        inline static uint32_t GetSkillTimer() { return GetTickCount() + *(uint32_t*)SkillTimerPtr; }
        inline static HWND GetGWWindowHandle() { return *(HWND*)WinHandlePtr; }
    };
}
