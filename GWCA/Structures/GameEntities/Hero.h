#pragma once

#include <Windows.h>
#include "..\GameContainers\gw_array.h"
#include "..\..\Constants\Constants.h"

namespace GWCA {
  namespace GW {
    
    struct HeroFlag {
      GwConstants::HeroID hero;
      AgentID heroid;
      DWORD unk1;
      GwConstants::HeroState state;
      GamePos flag;
      DWORD unk2;
      AgentID lockedtargetid;
    };

    using HeroFlagArray = gw_array<HeroFlag>;
  }
}
