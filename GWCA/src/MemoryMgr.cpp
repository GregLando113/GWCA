#include <GWCA\Managers\MemoryMgr.h>

#include <stdio.h>

#include <GWCA\Utilities\Scanner.h>

// Agent Array
BYTE* GW::MemoryMgr::agArrayPtr = NULL;
BYTE* GW::MemoryMgr::PlayerAgentIDPtr = NULL;
BYTE* GW::MemoryMgr::TargetAgentIDPtr = NULL;
BYTE* GW::MemoryMgr::MouseOverAgentIDPtr = NULL;

// Map ID
BYTE* GW::MemoryMgr::MapIDPtr = NULL;

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

BYTE* GW::MemoryMgr::MapInfoPtr = NULL;

BYTE* GW::MemoryMgr::DialogFunc = NULL;

BYTE* GW::MemoryMgr::DecodeStringFunc = NULL;

bool GW::MemoryMgr::Scan() {
	Scanner::Initialize(0x401000, 0x49a000);
	printf("[------------------ API SCAN START ------------------]\n");

	// Agent Array
	agArrayPtr = (BYTE*)Scanner::Find("\x56\x8B\xF1\x3B\xF0\x72\x04", "xxxxxxx", 0xC);
	if (agArrayPtr) {
		printf("agArrayPtr = %X\n", (DWORD)agArrayPtr);
		agArrayPtr = *(BYTE**)agArrayPtr;
		PlayerAgentIDPtr = (BYTE*)(agArrayPtr - 0x54);
		TargetAgentIDPtr = (BYTE*)(agArrayPtr - 0x500);
		MouseOverAgentIDPtr = (BYTE*)(agArrayPtr - 0x4F4);
	} else {
		printf("agArrayPtr = ERR\n");
		return false;
	}

	// Base pointer, used to get context pointer for game world.
	BasePointerLocation = (BYTE*)Scanner::Find("\x8B\x42\x0C\x56\x8B\x35", "xxxxxx", 0);
	if (BasePointerLocation) {
		printf("BasePointerLocation = %X\n", (DWORD)BasePointerLocation);
		BasePointerLocation = (BYTE*)(*(DWORD*)(BasePointerLocation + 6));
	} else {
		printf("BasePointerLocation = ERR\n");
		return false;
	}

	// Used for gamethread calls, as well as disable/enable rendering.
	RenderLoopLocation = (BYTE*)Scanner::Find("\x53\x56\xDF\xE0\xF6\xC4\x41", "xxxxxxx", 0);
	if (RenderLoopLocation) {
		printf("RenderLoopLocation = %X\n", (DWORD)RenderLoopLocation);
		RenderLoopLocation = RenderLoopLocation + 0x65;
		GameLoopLocation = RenderLoopLocation - 0x76;
		RenderLoopLocation = GameLoopLocation + 0x5D;
	} else {
		printf("RenderLoopLocation = ERR\n");
		return false;
	}

	// For Map IDs
	MapIDPtr = (BYTE*)Scanner::Find("\xB0\x7F\x8D\x55", "xxxx", 0);
	if (MapIDPtr) {
		printf("MapIDPtr = %X\n", (DWORD)MapIDPtr);
		MapIDPtr = *(BYTE**)(MapIDPtr + 0x46);
	} else {
		printf("MapIDPtr = ERR\n");
		return false;
	}

	// Skill timer to use for exact effect times.
	SkillTimerPtr = (BYTE*)Scanner::Find("\x85\xC0\x74\x11\x6A\x76\xBA", "xxxxxxx", -4);
	if (SkillTimerPtr) {
		printf("SkillTimerPtr = %X\n", (DWORD)SkillTimerPtr);
		SkillTimerPtr = *(BYTE**)SkillTimerPtr;
	} else {
		printf("SkillTimerPtr = ERR\n");
		return false;
	}

	WinHandlePtr = (BYTE*)Scanner::Find("\x56\x8B\xF1\x85\xC0\x89\x35", "xxxxxxx", 0);
	if (WinHandlePtr) {
		printf("WinHandlePtr = %X\n", (DWORD)WinHandlePtr);
		WinHandlePtr = *(BYTE**)(WinHandlePtr + 7);
	} else {
		printf("WinHandlePtr = ERR\n");
		return false;
	}

	MapInfoPtr = (BYTE*)Scanner::Find("\xC3\x8B\x75\xFC\x8B\x04\xB5", "xxxxxxx", 0);
	if (MapInfoPtr) {
		printf("MapInfoPtr = %X\n", (DWORD)MapInfoPtr);
		MapInfoPtr = *(BYTE**)(MapInfoPtr + 7);
	} else {
		printf("MapInfoPtr = ERR\n");
		return false;
	}


	DialogFunc = (BYTE*)Scanner::Find("\x55\x8B\xEC\x83\xEC\x28\x53\x56\x57\x8B\xF2\x8B\xD9", "xxxxxxxxxxxxx", -0x28);
	if (DialogFunc) {
		printf("DialogFunc = %X\n", (DWORD)DialogFunc);
	} else {
		printf("DialogFunc = ERR\n");
		return false;
	}

    DecodeStringFunc = (BYTE*)Scanner::Find("\x8D\x7C\x46\x02\x8B\xCE\x6A\x01", "xxxxxxxx", -136);
    if (DecodeStringFunc) {
        printf("DecodeStringFunc = %X\n", (DWORD)DecodeStringFunc);
    }
    else {
        printf("DecodeStringFunc = ERR\n");
        return false;
    }

	printf("[--------- API SCAN COMPLETED SUCESSFULLY ---------]\n");
	return true;
}
