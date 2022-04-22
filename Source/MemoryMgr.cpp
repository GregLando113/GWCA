#include "stdafx.h"

#include <GWCA/Utilities/Debug.h>
#include <GWCA/Utilities/Export.h>
#include <GWCA/Utilities/Scanner.h>

#include <GWCA/Managers/MemoryMgr.h>

// Used to get precise skill recharge times.
DWORD* GW::MemoryMgr::SkillTimerPtr = NULL;

uintptr_t GW::MemoryMgr::WinHandlePtr = NULL;

uintptr_t GW::MemoryMgr::GetPersonalDirPtr = NULL;

uint32_t(_cdecl* GW::MemoryMgr::GetGWVersion)(void)  = NULL;

bool GW::MemoryMgr::Scan() {
    Scanner::Initialize();

    // Skill timer to use for exact effect times.
    SkillTimerPtr = (DWORD*)Scanner::Find(
        "\x83\xCA\x01\x89\x15\x00\x00\x00\x00\xFF\xD6\x8B", "xxxxx????xxx", +5);
    if (SkillTimerPtr) {
        GWCA_INFO("[SCAN] SkillTimerPtr = %08X\n", SkillTimerPtr);
        SkillTimerPtr = *(DWORD**)SkillTimerPtr;
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

    uintptr_t addr = Scanner::Find("\x6A\x00\x68\x00\x00\x01\x00\x89", "xxxxxxxx", 0x42);
    if (addr && (addr = Scanner::FunctionFromNearCall(addr))) {
        GetGWVersion = (uint32_t(_cdecl *)(void))addr;
        GWCA_INFO("[SCAN] GetGWVersion = %08X, %d\n", GetGWVersion, GetGWVersion());
    }
    else {
        GWCA_INFO("[SCAN] GWVersion= ERR\n");
        return false;
    }
    return true;
}
