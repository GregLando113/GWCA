#pragma once

// Allows enum classes to be compared to uints
// Useful for doing stuff like "if(!!item_id && item_id < array.size())"
// ...but don't take the piss
#define ENUM_CLASS_OPERATORS(varname, null_variable) 
/*#define ENUM_CLASS_OPERATORS(varname, null_variable) \
bool operator!(varname e) { return (uint32_t)e == null_variable; }; \
bool operator>(varname a, uint32_t b) { return (uint32_t)a > b; }; \
bool operator>=(varname a, uint32_t b) { return (uint32_t)a >= b; }; \
bool operator==(varname a, uint32_t b) { return (uint32_t)a == b; }; \
bool operator<=(varname a, uint32_t b) { return (uint32_t)a <= b; }; \
bool operator<(varname a, uint32_t b) { return (uint32_t)a < b; }; \
bool operator!=(varname a, uint32_t b) { return (uint32_t)a != b; }; \
bool operator>(uint32_t a, varname b) { return a > (uint32_t)b; }; \
bool operator>=(uint32_t a, varname b) { return a >= (uint32_t)b; }; \
bool operator==(uint32_t a, varname b) { return a == (uint32_t)b; }; \
bool operator<=(uint32_t a, varname b) { return a <= (uint32_t)b; }; \
bool operator<(uint32_t a, varname b) { return a < (uint32_t)b; }; \
bool operator!=(uint32_t a, varname b) { return a != (uint32_t)b; };*/

// Lightweight identifiers that have strict types
namespace GW {
	typedef uint32_t Color;

	enum class ItemID { None };
	enum class AgentID { None };
	enum class PlayerID { None };
	enum class GuildID { None };
	enum class PartyID { None = -1, PlayerParty = -2 };
	namespace Constants {
		enum class SkillID;
		enum class NpcID;
	}
}