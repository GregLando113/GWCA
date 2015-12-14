#include "MemoryMgr.h"
#include <stdio.h>
#include "PatternScanner.h"

// Agent Array
BYTE* GWAPI::MemoryMgr::agArrayPtr = NULL;
BYTE* GWAPI::MemoryMgr::PlayerAgentIDPtr = NULL;
BYTE* GWAPI::MemoryMgr::TargetAgentIDPtr = NULL;
BYTE* GWAPI::MemoryMgr::MouseOverAgentIDPtr = NULL;

// Map ID
BYTE* GWAPI::MemoryMgr::MapIDPtr = NULL;

// Gameserver PacketSend Addresses
BYTE* GWAPI::MemoryMgr::GSObjectPtr = NULL;
BYTE* GWAPI::MemoryMgr::CtoGSSendFunction = NULL;

// Base ptr to get context pointer, which houses basically
BYTE* GWAPI::MemoryMgr::BasePointerLocation = NULL;

// Renderloop / Main Gameloop
BYTE* GWAPI::MemoryMgr::RenderLoopLocation = NULL;
BYTE* GWAPI::MemoryMgr::GameLoopLocation = NULL;
BYTE* GWAPI::MemoryMgr::GameLoopReturn = NULL;
BYTE* GWAPI::MemoryMgr::GameLoopRestore = NULL;

// Chat function for simple debug/notifications
BYTE* GWAPI::MemoryMgr::WriteChatFunction = NULL;

// Used to get precise skill recharge times.
BYTE* GWAPI::MemoryMgr::SkillTimerPtr = NULL;

BYTE* GWAPI::MemoryMgr::PostProcessEffectFunction = NULL;

// Used to get skill information.
BYTE* GWAPI::MemoryMgr::SkillArray = NULL;
BYTE* GWAPI::MemoryMgr::UseSkillFunction = NULL;

BYTE* GWAPI::MemoryMgr::ChangeTargetFunction = NULL;

BYTE* GWAPI::MemoryMgr::XunlaiSession = NULL;
BYTE* GWAPI::MemoryMgr::OpenXunlaiFunction = NULL;

BYTE* GWAPI::MemoryMgr::MoveFunction = NULL;

BYTE* GWAPI::MemoryMgr::WinHandlePtr = NULL;

BYTE* GWAPI::MemoryMgr::MapInfoPtr = NULL;

BYTE* GWAPI::MemoryMgr::DialogFunc = NULL;

bool GWAPI::MemoryMgr::Scan()
{
	PatternScanner scan(0x401000, 0x4FF000);
	printf("[------------------ API SCAN START ------------------]\n");

		// Agent Array
		agArrayPtr = (BYTE*)scan.FindPattern("\x56\x8B\xF1\x3B\xF0\x72\x04", "xxxxxxx", 0xC);
		if (agArrayPtr)
		{
			printf("agArrayPtr = %X\n", agArrayPtr);
			agArrayPtr = *(BYTE**)agArrayPtr;
			PlayerAgentIDPtr = (BYTE*)(agArrayPtr - 0x54);
			TargetAgentIDPtr = (BYTE*)(agArrayPtr - 0x500);
			MouseOverAgentIDPtr = (BYTE*)(agArrayPtr - 0x4F4);
		}
		else{
			printf("agArrayPtr = ERR\n");
			return false;
		}

		// Packet Sender Stuff
		GSObjectPtr = (BYTE*)scan.FindPattern("\x56\x33\xF6\x3B\xCE\x74\x0E\x56\x33\xD2", "xxxxxxxxxx", -4);
		if (GSObjectPtr){
			printf("CtoGSObjectPtr = %X\n", GSObjectPtr);
		}
		else{
			printf("CtoGSObjectPtr = ERR\n");
			return false;
		}
		CtoGSSendFunction = (BYTE*)scan.FindPattern("\x55\x8B\xEC\x83\xEC\x2C\x53\x56\x57\x8B\xF9\x85", "xxxxxxxxxxxx", 0);
		if (CtoGSSendFunction){
			printf("CtoGSSendFunction = %X\n", CtoGSSendFunction);
		}
		else{
			printf("CtoGSSendFunction = ERR\n");
			return false;
		}

		// Base pointer, used to get context pointer for game world.
		BasePointerLocation = (BYTE*)scan.FindPattern("\x8B\x42\x0C\x56\x8B\x35", "xxxxxx", 0);
		if (BasePointerLocation){
			printf("BasePointerLocation = %X\n", BasePointerLocation);
			BasePointerLocation = (BYTE*)(*(DWORD*)(BasePointerLocation + 6));
		}
		else{
			printf("BasePointerLocation = ERR\n");
			return false;
		}

		// Used for gamethread calls, as well as disable/enable rendering.
		RenderLoopLocation = (BYTE*)scan.FindPattern("\x53\x56\xDF\xE0\xF6\xC4\x41", "xxxxxxx", 0);
		if (RenderLoopLocation){
			printf("RenderLoopLocation = %X\n", RenderLoopLocation);
			RenderLoopLocation = RenderLoopLocation + 0x65;
			GameLoopLocation = RenderLoopLocation - 0x76;
			RenderLoopLocation = GameLoopLocation + 0x5D;
		}
		else{
			printf("RenderLoopLocation = ERR\n");
			return false;
		}

		// For Map IDs
		MapIDPtr = (BYTE*)scan.FindPattern("\xB0\x7F\x8D\x55", "xxxx", 0);
		if (MapIDPtr){
			printf("MapIDPtr = %X\n", MapIDPtr);
			MapIDPtr = *(BYTE**)(MapIDPtr + 0x46);
		}
		else{
			printf("MapIDPtr = ERR\n");
			return false;
		}

		// To write info / Debug as a PM in chat
		WriteChatFunction = (BYTE*)scan.FindPattern("\x55\x8B\xEC\x51\x53\x89\x4D\xFC\x8B\x4D\x08\x56\x57\x8B", "xxxxxxxxxxxxxx", 0);
		if (WriteChatFunction){
			printf("WriteChatFunction = %X\n", WriteChatFunction);
		}
		else{
			printf("WriteChatFunction = ERR\n");
			return false;
		}

		// Skill timer to use for exact effect times.
		SkillTimerPtr = (BYTE*)scan.FindPattern("\x56\x85\xC0\x8B\xF1\x75\x05", "xxxxxxx", -4);
		if (SkillTimerPtr){
			printf("SkillTimerPtr = %X\n", SkillTimerPtr);
			SkillTimerPtr = *(BYTE**)SkillTimerPtr;
		}
		else{
			printf("SkillTimerPtr = ERR\n");
			return false;
		}

		// Skill array.
		SkillArray = (BYTE*)scan.FindPattern("\x8D\x04\xB6\x5E\xC1\xE0\x05\x05", "xxxxxxxx", 0);
		if (SkillArray){
			printf("SkillArray = %X\n", SkillArray);
			SkillArray = *(BYTE**)(SkillArray + 8);
		}
		else{
			printf("SkillArray = ERR\n");
			return false;
		}

		// Use Skill Function.
		UseSkillFunction = (BYTE*)scan.FindPattern("\x55\x8B\xEC\x83\xEC\x10\x53\x56\x8B\xD9\x57\x8B\xF2\x89\x5D\xF0", "xxxxxxxxxxxxxxxx", 0);
		if (UseSkillFunction){
			printf("UseSkillFunction = %X\n", UseSkillFunction);
		}
		else{
			printf("UseSkillFunction = ERR\n");
			return false;
		}


		PostProcessEffectFunction = (BYTE*)scan.FindPattern("\x55\x8B\xEC\x83\xEC\x10\x89\x4D\xF8\xC7\x45\xFC", "xxxxxxxxxxxx", 0);
		if (PostProcessEffectFunction){
			printf("PostProcessEffectFunction = %X\n", PostProcessEffectFunction);
		}
		else{
			printf("PostProcessEffectFunction = ERR\n");
			return false;
		}

		ChangeTargetFunction = (BYTE*)scan.FindPattern("\x33\xC0\x3B\xDA\x0F\x95\xC0\x33", "xxxxxxxx", -0x78);
		if (ChangeTargetFunction){
			printf("ChangeTargetFunction = %X\n", ChangeTargetFunction);
		}
		else{
			printf("ChangeTargetFunction = ERR\n");
			return false;
		}

		OpenXunlaiFunction = (BYTE*)scan.FindPattern("\x8B\xF1\x6A\x00\xBA\x12", "xxxxxx", -6);
		if (OpenXunlaiFunction){
			printf("OpenXunlaiFunction = %X\n", OpenXunlaiFunction);
		}
		else{
			printf("OpenXunlaiFunction = ERR\n");
			return false;
		}
		XunlaiSession = (BYTE*)scan.FindPattern("\x8D\x14\x76\x8D\x14\x90\x8B\x42\x08\xA8\x01\x75\x41", "xxxxxxxxxxxxx", 0);
		if (XunlaiSession){
			printf("XunlaiSession = %X\n", XunlaiSession);
			XunlaiSession = *(BYTE**)(XunlaiSession - 4);
		}
		else{
			printf("XunlaiSession = ERR\n");
			return false;
		}

		MoveFunction = (BYTE*)scan.FindPattern("\xD9\x07\xD8\x5D\xF0\xDF\xE0\xF6\xC4\x01", "xxxxxxxxxx", -0x12);
		if (MoveFunction){
			printf("MoveFunction = %X\n", MoveFunction);
		}
		else{
			printf("MoveFunction = ERR\n");
			return false;
		}

		WinHandlePtr = (BYTE*)scan.FindPattern("\x56\x8B\xF1\x85\xC0\x89\x35", "xxxxxxx", 0);
		if (WinHandlePtr){
			printf("WinHandlePtr = %X\n", WinHandlePtr);
			WinHandlePtr = *(BYTE**)(WinHandlePtr + 7);
		}
		else{
			printf("WinHandlePtr = ERR\n");
			return false;
		}

		MapInfoPtr = (BYTE*)scan.FindPattern("\xC3\x8B\x75\xFC\x8B\x04\xB5", "xxxxxxx", 0);
		if (MapInfoPtr){
			printf("MapInfoPtr = %X\n", MapInfoPtr);
			MapInfoPtr = *(BYTE**)(MapInfoPtr + 7);
		}
		else{
			printf("MapInfoPtr = ERR\n");
			return false;
		}


		DialogFunc = (BYTE*)scan.FindPattern("\x55\x8B\xEC\x83\xEC\x28\x53\x56\x57\x8B\xF2\x8B\xD9", "xxxxxxxxxxxxx", -0x28);
		if (DialogFunc){
			printf("DialogFunc = %X\n", DialogFunc);
		}
		else{
			printf("DialogFunc = ERR\n");
			return false;
		}

		printf("[--------- API SCAN COMPLETED SUCESSFULLY ---------]\n");
		return true;
}





