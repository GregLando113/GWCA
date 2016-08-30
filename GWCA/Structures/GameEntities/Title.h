#pragma once
#include <Windows.h>
#include "..\GameContainers\gw_array.h"

namespace GWCA {
  namespace GW {
    struct Title {
			DWORD unk1;
			DWORD currentpoints;
			DWORD unk2;
			DWORD pointsneeded_currentrank;
			DWORD unk3;
			DWORD pointsneeded_nextrank;
			DWORD maxtitlerank;
			DWORD unk4;
			void* unk5[2]; // Pretty sure these are ptrs to title hash strings
		}; // size = 0x28

		using TitleArray = gw_array<Title>;
  }
}
