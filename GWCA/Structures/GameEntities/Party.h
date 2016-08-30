#pragma once

#include <Windows.h>
#include "gw_array.h"

namespace GWCA {
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
			DWORD unk[11];
			DWORD profession;
			DWORD level;
		};

    using PlayerPartyMemberArray = gw_array<PlayerPartyMember>;
		using HeroPartyMemberArray = gw_array<HeroPartyMember>;
		using HenchmanPartyMemberArray = gw_array<HenchmanPartyMember>;

    struct PartyInfo
		{
			char pad_0x0000[0x4]; //0x0000
			gw_array<PlayerPartyMember> players; //0x0004
			gw_array<HenchmanPartyMember> henchmen; //0x0014
			gw_array<HeroPartyMember> heroes; //0x0024
			gw_array<void*> unk1_arr; //0x0034
		};

  }
}
