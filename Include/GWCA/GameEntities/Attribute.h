#pragma once

#include <GWCA/GameContainers/Array.h>

namespace GW {
    struct Attribute { // total: 0x14/20
        /* +h0000 */ uint32_t id; // ID of attribute
        /* +h0004 */ uint32_t level_base; // Level of attribute without modifiers (runes,pcons,etc)
        /* +h0008 */ uint32_t level; // Level with modifiers
        /* +h000C */ uint32_t decrement_points; // Points that you will recieve back if you decrement level.
        /* +h0010 */ uint32_t increment_points; // Points you will need to increment level.
    };

    struct PartyAttribute {
        uint32_t agent_id;
        Attribute attribute[49];
    };

    typedef Array<PartyAttribute> PartyAttributeArray;
}
