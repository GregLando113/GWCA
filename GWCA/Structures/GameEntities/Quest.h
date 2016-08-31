#pragma once

#include <Windows.h>
#include "..\GameContainers\gw_array.h"

namespace GWCA {
	namespace GW {
		struct Quest {
			DWORD questid;
			int logstate;
			void* unk1[3];
			DWORD mapfrom;
			GamePos marker;
			DWORD unk2;
			DWORD mapto;
			void* unk3[2];
		};

		using QuestLog = gw_array<Quest>;
	}
}
