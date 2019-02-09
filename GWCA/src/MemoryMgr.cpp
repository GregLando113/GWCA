#include "stdafx.h"

#include <GWCA/Utilities/Export.h>
#include <GWCA/Utilities/Scanner.h>

#include <GWCA/Managers/MemoryMgr.h>

// Agent Array
BYTE* GW::MemoryMgr::agArrayPtr = NULL;

// Base ptr to get context pointer, which houses basically
BYTE* GW::MemoryMgr::BasePointerLocation = NULL;

// Renderloop / Main Gameloop
BYTE* GW::MemoryMgr::RenderLoopLocation = NULL;
BYTE* GW::MemoryMgr::GameLoopLocation = NULL;
BYTE* GW::MemoryMgr::GameLoopReturn = NULL;
BYTE* GW::MemoryMgr::GameLoopRestore = NULL;

// Used to get precise skill recharge times.
BYTE* GW::MemoryMgr::SkillTimerPtr = NULL;

BYTE* GW::MemoryMgr::WinHandlePtr = NULL;

BYTE* GW::MemoryMgr::AsyncDecodeStringPtr = NULL;

BYTE* GW::MemoryMgr::GetPersonalDirPtr = NULL;

bool GW::MemoryMgr::Scan() {
    Scanner::Initialize(0x401000, 0x49a000);

    // Agent Array
    agArrayPtr = (BYTE*)Scanner::Find("\x56\x8B\xF1\x3B\xF0\x72\x04", "xxxxxxx", 0xC);
    if (agArrayPtr) {
        printf("[SCAN] agArrayPtr = %p\n", agArrayPtr);
        agArrayPtr = *(BYTE**)agArrayPtr;
    } else {
        printf("[SCAN] agArrayPtr = ERR\n");
        return false;
    }

    // Base pointer, used to get context pointer for game world.
    BasePointerLocation = (BYTE*)Scanner::Find("\x8B\x42\x0C\x56\x8B\x35", "xxxxxx", 0);
    if (BasePointerLocation) {
        printf("[SCAN] BasePointerLocation = %p\n", BasePointerLocation);
        BasePointerLocation = (BYTE*)(*(DWORD*)(BasePointerLocation + 6));
    } else {
        printf("[SCAN] BasePointerLocation = ERR\n");
        return false;
    }

    // Used for gamethread calls, as well as disable/enable rendering.
    RenderLoopLocation = (BYTE*)Scanner::Find("\x53\x56\xDF\xE0\xF6\xC4\x41", "xxxxxxx", 0);
    if (RenderLoopLocation) {
        printf("[SCAN] RenderLoopLocation = %p\n", RenderLoopLocation);
        RenderLoopLocation = RenderLoopLocation + 0x65;
        GameLoopLocation = RenderLoopLocation - 0x76;
        RenderLoopLocation = GameLoopLocation + 0x5D;
    } else {
        printf("[SCAN] RenderLoopLocation = ERR\n");
        return false;
    }

    // Skill timer to use for exact effect times.
    SkillTimerPtr = (BYTE*)Scanner::Find("\x85\xC0\x74\x11\x6A\x76\xBA", "xxxxxxx", -4);
    if (SkillTimerPtr) {
        printf("[SCAN] SkillTimerPtr = %p\n", SkillTimerPtr);
        SkillTimerPtr = *(BYTE**)SkillTimerPtr;
    } else {
        printf("[SCAN] SkillTimerPtr = ERR\n");
        return false;
    }

    WinHandlePtr = (BYTE*)Scanner::Find("\x56\x8B\xF1\x85\xC0\x89\x35", "xxxxxxx", 0);
    if (WinHandlePtr) {
        printf("[SCAN] WinHandlePtr = %p\n", WinHandlePtr);
        WinHandlePtr = *(BYTE**)(WinHandlePtr + 7);
    } else {
        printf("[SCAN] WinHandlePtr = ERR\n");
        return false;
    }

    AsyncDecodeStringPtr = (BYTE*)Scanner::Find("\x8D\x7C\x46\x02\x8B\xCE\x6A\x01", "xxxxxxxx", -136);
    if (AsyncDecodeStringPtr) {
        printf("[SCAN] AsyncDecodeStringPtr = %p\n", AsyncDecodeStringPtr);
    } else {
        printf("[SCAN] AsyncDecodeStringPtr = ERR\n");
        return false;
    }

    GetPersonalDirPtr = (BYTE*)Scanner::Find("\x81\xFF\x04\x01\x00\x00\x8B\xF2\x73\x23", "xxxxxxxxxx", -8);
    if (GetPersonalDirPtr) {
        printf("[SCAN] GetPersonalDirPtr = %p\n", GetPersonalDirPtr);
    } else {
        printf("[SCAN] GetPersonalDirPtr= ERR\n");
        return false;
    }
    return true;
}
