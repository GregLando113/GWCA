#pragma once

#include <Windows.h>

#include <GWCA\GameContainers\Array.h>

namespace GW {
	struct NPC {
		DWORD ModelFileID; //0x0000
		char pad_0x0004[12]; //0x0004
		DWORD NpcFlags; //0x0010
		char pad_0x0014[12]; //0x0014
		wchar_t *NameString;
		char pad_0x0024[12];
	}; //Size=0x0030
	using NPCArray = Array<NPC>;
}
