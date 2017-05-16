#pragma once

#include <Windows.h>

#include <GWCA\GameContainers\Array.h>

namespace GW {
	struct PlayerPartyMember {
		DWORD loginnumber;
		DWORD unk1;
		DWORD state;

		inline bool connected() { return (state & 1) > 0; }
		inline bool ticked() { return (state & 2) > 0; }
	};

	struct HeroPartyMember {
		DWORD agentid;
		DWORD ownerplayerid;
		DWORD heroid;
		DWORD unk1;
		DWORD unk2;
		DWORD level;
	};

	struct HenchmanPartyMember {
		DWORD agentid;
		DWORD unk[10];
		DWORD profession;
		DWORD level;
	};

	using PlayerPartyMemberArray = Array<PlayerPartyMember>;
	using HeroPartyMemberArray = Array<HeroPartyMember>;
	using HenchmanPartyMemberArray = Array<HenchmanPartyMember>;

	struct PartyInfo {
		char pad_0x0000[0x4]; //0x0000
		Array<PlayerPartyMember> players; //0x0004
		Array<HenchmanPartyMember> henchmen; //0x0014
		Array<HeroPartyMember> heroes; //0x0024
		Array<DWORD> others; //0x0034, agent id of allies, minions, pets.
	};

}
