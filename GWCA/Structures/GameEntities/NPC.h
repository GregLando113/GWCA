#pragma once
#include <Windows.h>
#include "..\GameContainers\gw_array.h"

namespace GWCA {
	namespace GW {
		struct NPC {
			DWORD modelfileid; //0x0000 
			char pad_0x0004[0xC]; //0x0004
			DWORD npcflags; //0x0010 
			char pad_0x0014[0x1C]; //0x0014
		}; //Size=0x0030
		using NPCArray = gw_array<NPC>;
	}
}

