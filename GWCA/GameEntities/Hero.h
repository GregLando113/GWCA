#pragma once

#include <Windows.h>

#include <GWCA\Constants\Constants.h>
#include <GWCA\GameContainers\gw_array.h>

namespace GW {

	struct HeroFlag {
		GW::Constants::HeroID hero;
		AgentID heroid;
		DWORD unk1;
		GW::Constants::HeroState state;
		GamePos flag;
		DWORD unk2;
		AgentID lockedtargetid;
	};

	using HeroFlagArray = gw_array<HeroFlag>;
}
