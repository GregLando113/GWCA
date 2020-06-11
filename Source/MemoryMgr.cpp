#include "stdafx.h"

#include <GWCA/Utilities/Debug.h>
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
        GWCA_INFO("[SCAN] SkillTimerPtr = %08X\n", SkillTimerPtr);
        SkillTimerPtr = *(uintptr_t *)SkillTimerPtr;
    } else {
        GWCA_INFO("[SCAN] SkillTimerPtr = ERR\n");
        return false;
    }

    WinHandlePtr = Scanner::Find("\x83\xC4\x04\x83\x3D\x00\x00\x00\x00\x00\x75\x31", "xxxxx????xxx", -0xC);
    if (WinHandlePtr) {
        GWCA_INFO("[SCAN] WinHandlePtr = %08X\n", WinHandlePtr);
        WinHandlePtr = *(uintptr_t *)WinHandlePtr;
    } else {
        GWCA_INFO("[SCAN] WinHandlePtr = ERR\n");
        return false;
    }

    GetPersonalDirPtr = Scanner::Find("\x75\x2E\x6A\x01\x6A\x05\x56\x6A\x00", "xxxxxxxxx", -0x53);
    if (GetPersonalDirPtr) {
        GWCA_INFO("[SCAN] GetPersonalDirPtr = %08X\n", GetPersonalDirPtr);
    } else {
        GWCA_INFO("[SCAN] GetPersonalDirPtr= ERR\n");
        return false;
    }
    return true;
}
