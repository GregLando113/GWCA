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

    uintptr_t address;

    // Skill timer to use for exact effect times.
    address = Scanner::Find("\x83\xCA\x01\x89\x15\x00\x00\x00\x00\xFF\xD6\x8B", "xxxxx????xxx", +5);
    if (address)
        SkillTimerPtr = *(DWORD**)address;

    address = Scanner::Find("\x83\xC4\x04\x83\x3D\x00\x00\x00\x00\x00\x75\x31", "xxxxx????xxx", -0xC);
    if (address)
        WinHandlePtr = *(uintptr_t *)address;

    GetPersonalDirPtr = Scanner::Find("\x75\x2E\x6A\x01\x6A\x05\x56\x6A\x00", "xxxxxxxxx", -0x53);

    address = Scanner::Find("\x6A\x00\x68\x00\x00\x01\x00\x89", "xxxxxxxx", 0x42);
    if(address)
        GetGWVersion = (uint32_t(_cdecl*)(void))Scanner::FunctionFromNearCall(address);

    GWCA_INFO("[SCAN] SkillTimerPtr = %08X", SkillTimerPtr);
    GWCA_INFO("[SCAN] WinHandlePtr = %08X", WinHandlePtr);
    GWCA_INFO("[SCAN] GetPersonalDirPtr = %08X", GetPersonalDirPtr);
    GWCA_INFO("[SCAN] GetGWVersion = %08X, %d", GetGWVersion, GetGWVersion());

#if _DEBUG
    GWCA_ASSERT(SkillTimerPtr);
    GWCA_ASSERT(WinHandlePtr);
    GWCA_ASSERT(GetPersonalDirPtr);
    GWCA_ASSERT(GetGWVersion);
#endif

    return SkillTimerPtr && WinHandlePtr && GetPersonalDirPtr && GetGWVersion;
}
