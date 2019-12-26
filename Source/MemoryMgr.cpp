#include "stdafx.h"

#include <GWCA/Utilities/Export.h>
#include <GWCA/Utilities/Scanner.h>

#include <GWCA/Managers/MemoryMgr.h>

// Used to get precise skill recharge times.
uintptr_t GW::MemoryMgr::SkillTimerPtr = NULL;

uintptr_t GW::MemoryMgr::WinHandlePtr = NULL;

uintptr_t GW::MemoryMgr::GetPersonalDirPtr = NULL;

bool GW::MemoryMgr::Scan() {
    Scanner::Initialize();

    // Skill timer to use for exact effect times.
    SkillTimerPtr = Scanner::Find(
        "\x83\xCA\x01\x89\x15\x00\x00\x00\x00\xFF\xD6\x8B", "xxxxx????xxx", +5);
    if (SkillTimerPtr) {
        printf("[SCAN] SkillTimerPtr = %08lX\n", SkillTimerPtr);
        SkillTimerPtr = *(uintptr_t *)SkillTimerPtr;
    } else {
        printf("[SCAN] SkillTimerPtr = ERR\n");
        return false;
    }

    WinHandlePtr = Scanner::Find("\x83\xC4\x04\x83\x3D\x00\x00\x00\x00\x00\x75\x31", "xxxxx????xxx", -0xC);
    if (WinHandlePtr) {
        printf("[SCAN] WinHandlePtr = %08lX\n", WinHandlePtr);
        WinHandlePtr = *(uintptr_t *)WinHandlePtr;
    } else {
        printf("[SCAN] WinHandlePtr = ERR\n");
        return false;
    }

    GetPersonalDirPtr = Scanner::Find("\x75\x2E\x6A\x01\x6A\x05\x56\x6A\x00", "xxxxxxxxx", -0x53);
    if (GetPersonalDirPtr) {
        printf("[SCAN] GetPersonalDirPtr = %08lX\n", GetPersonalDirPtr);
    } else {
        printf("[SCAN] GetPersonalDirPtr= ERR\n");
        return false;
    }
    return true;
}
