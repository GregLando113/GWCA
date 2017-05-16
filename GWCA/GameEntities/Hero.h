#pragma once

#include <Windows.h>

#include <GWCA\Constants\Constants.h>
#include <GWCA\GameContainers\Array.h>

namespace GW {

	struct HeroFlag {
		GW::Constants::HeroID hero;
		AgentID heroid;
		DWORD unk1;
		GW::Constants::HeroBehavior state;
		GamePos flag;
		DWORD unk2;
		AgentID lockedtargetid;
	};

	using HeroFlagArray = Array<HeroFlag>;
}
