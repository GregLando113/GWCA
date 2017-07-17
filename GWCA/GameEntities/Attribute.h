#ifndef _ENTITIE_ATTRIBUTE_INC
#define _ENTITIE_ATTRIBUTE_INC

#include <GWCA\GameContainers\Array.h>

namespace GW {
    struct Attribute { // total: 0x14/20
        /* +h0000 */ DWORD ID; // ID of attribute
        /* +h0004 */ DWORD level_base; // Level of attribute without modifiers (runes,pcons,etc)
        /* +h0008 */ DWORD level; // Level with modifiers
        /* +h000C */ DWORD decrement_points; // Points that you will recieve back if you decrement level.
        /* +h0010 */ DWORD increment_points; // Points you will need to increment level.
    };

    struct PartyAttribute {
        DWORD AgentID;
        Attribute Attribute[49];
    };

    using PartyAttributeArray = Array<PartyAttribute>;
}

#endif // _ENTITIE_ATTRIBUTE_INC
