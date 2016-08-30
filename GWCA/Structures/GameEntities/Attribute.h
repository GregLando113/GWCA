#pragma once
#include <Windows.h>

namespace GWCA {
  namespace GW {
    struct Attribute {
			DWORD ID; // ID of attribute
			DWORD level_base; // Level of attribute without modifiers (runes,pcons,etc)
			DWORD level; // Level with modifiers
			DWORD decrement_points; // Points that you will recieve back if you decrement level.
			DWORD increment_points; // Points you will need to increment level.
		};

		struct PartyAttribute {
			DWORD AgentID;
			Attribute Attribute[49];
		};

		using PartyAttributeArray = gw_array<PartyAttribute>;
  }
}
