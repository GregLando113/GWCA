#include "stdafx.h"

#include <GWCA/Utilities/Export.h>
#include <GWCA/Utilities/Scanner.h>

#include <GWCA/Managers/MemoryMgr.h>

// Agent Array
uintptr_t GW::MemoryMgr::AgentArrayPtr = NULL;

// Base ptr to get context pointer, which houses basically
uintptr_t GW::MemoryMgr::BasePointerLocation = NULL;

// Renderloop / Main Gameloop
uintptr_t GW::MemoryMgr::RenderLoopLocation = NULL;
uintptr_t GW::MemoryMgr::GameLoopLocation = NULL;
uintptr_t GW::MemoryMgr::GameLoopReturn = NULL;
uintptr_t GW::MemoryMgr::GameLoopRestore = NULL;

// Used to get precise skill recharge times.
uintptr_t GW::MemoryMgr::SkillTimerPtr = NULL;

uintptr_t GW::MemoryMgr::WinHandlePtr = NULL;

uintptr_t GW::MemoryMgr::AsyncDecodeStringPtr = NULL;

uintptr_t GW::MemoryMgr::GetPersonalDirPtr = NULL;

bool GW::MemoryMgr::Scan() {
    Scanner::Initialize(0x401000, 0x49a000);

    // Agent Array
    AgentArrayPtr = Scanner::Find("\x56\x8B\xF1\x3B\xF0\x72\x04", "xxxxxxx", 0xC);
    if (AgentArrayPtr) {
        printf("[SCAN] AgentArrayPtr = %08lX\n", AgentArrayPtr);
        AgentArrayPtr = *(uintptr_t *)AgentArrayPtr;
    } else {
        printf("[SCAN] AgentArrayPtr = ERR\n");
        return false;
    }

    // Base pointer, used to get context pointer for game world.
    BasePointerLocation = Scanner::Find("\x8B\x42\x0C\x56\x8B\x35", "xxxxxx", 0);
    if (BasePointerLocation) {
        printf("[SCAN] BasePointerLocation = %08lX\n", BasePointerLocation);
        BasePointerLocation = *(uintptr_t *)(BasePointerLocation + 6);
    } else {
        printf("[SCAN] BasePointerLocation = ERR\n");
        return false;
    }

    // Used for gamethread calls, as well as disable/enable rendering.
    RenderLoopLocation = Scanner::Find("\x53\x56\xDF\xE0\xF6\xC4\x41", "xxxxxxx", 0);
    if (RenderLoopLocation) {
        printf("[SCAN] RenderLoopLocation = %08lX\n", RenderLoopLocation);
        RenderLoopLocation = RenderLoopLocation + 0x65;
        GameLoopLocation = RenderLoopLocation - 0x76;
        RenderLoopLocation = GameLoopLocation + 0x5D;
    } else {
        printf("[SCAN] RenderLoopLocation = ERR\n");
        return false;
    }

    // Skill timer to use for exact effect times.
    SkillTimerPtr = Scanner::Find("\x85\xC0\x74\x11\x6A\x76\xBA", "xxxxxxx", -4);
    if (SkillTimerPtr) {
        printf("[SCAN] SkillTimerPtr = %08lX\n", SkillTimerPtr);
        SkillTimerPtr = *(uintptr_t *)SkillTimerPtr;
    } else {
        printf("[SCAN] SkillTimerPtr = ERR\n");
        return false;
    }

    WinHandlePtr = Scanner::Find("\x56\x8B\xF1\x85\xC0\x89\x35", "xxxxxxx", 0);
    if (WinHandlePtr) {
        printf("[SCAN] WinHandlePtr = %08lX\n", WinHandlePtr);
        WinHandlePtr = *(uintptr_t *)(WinHandlePtr + 7);
    } else {
        printf("[SCAN] WinHandlePtr = ERR\n");
        return false;
    }

    AsyncDecodeStringPtr = Scanner::Find("\x8D\x7C\x46\x02\x8B\xCE\x6A\x01", "xxxxxxxx", -136);
    if (AsyncDecodeStringPtr) {
        printf("[SCAN] AsyncDecodeStringPtr = %08lX\n", AsyncDecodeStringPtr);
    } else {
        printf("[SCAN] AsyncDecodeStringPtr = ERR\n");
        return false;
    }

    GetPersonalDirPtr = Scanner::Find("\x81\xFF\x04\x01\x00\x00\x8B\xF2\x73\x23", "xxxxxxxxxx", -8);
    if (GetPersonalDirPtr) {
        printf("[SCAN] GetPersonalDirPtr = %08lX\n", GetPersonalDirPtr);
    } else {
        printf("[SCAN] GetPersonalDirPtr= ERR\n");
        return false;
    }
    return true;
}
