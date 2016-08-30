#pragma once
#include <Windows.h>
#include "..\GameContainers\gw_array.h"

namespace GWCA {
  namespace GW {

	using PlayerID = DWORD;

    struct Player {
      DWORD AgentID; //0x0000
      char pad_0x0004[0x14]; //0x0004
      DWORD Primary; //0x0018
      DWORD Secondary; //0x001C
      char pad_0x0020[0x4]; //0x0020
      struct NameMod {
        char pad_0x0000[0x4]; //0x0000
        wchar_t Name[20]; //0x0004
      } *Name1; //Size=0x002C
      wchar_t* Name; //0x0028
      char pad_0x002C[0x4]; //0x002C
      DWORD ActiveTitle; //0x0030
      char pad_0x0034[0x4]; //0x0034
      DWORD PartySize; //0x0038
      char pad_0x003C[0x10]; //0x003C
    };//Size=0x004C

    using PlayerArray = gw_array<Player>;

  }
}
