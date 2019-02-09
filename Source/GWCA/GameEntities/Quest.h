#pragma once

#include <GWCA/GameContainers/Array.h>

namespace GW {
    struct Quest { // total: 0x34/52
        /* +h0000 */ uint32_t quest_id;
        /* +h0004 */ uint32_t log_state;
        /* +h0008 */ uint32_t h0008; // quest category
        /* +h000C */ uint32_t h000C; // quest name
        /* +h0010 */ uint32_t h0010; //
        /* +h0014 */ uint32_t map_from;
        /* +h0018 */ Vec3f    marker;
        /* +h0024 */ uint32_t h0024;
        /* +h0028 */ uint32_t map_to;
        /* +h002C */ uint32_t h002C; // namestring reward
        /* +h0030 */ uint32_t h0030; // namestring objective
    };
    static_assert(sizeof(Quest) == 52, "struct Quest has incorect size");

    struct MissionObjective { // total: 0xC/12
        /* +h0000 */ uint32_t objective_id;
        /* +h0004 */ wchar_t *enc_str;
        /* +h0008 */ uint32_t type; // completed, bullet, etc...
    };
    static_assert(sizeof(MissionObjective) == 12, "struct MissionObjective has incorect size");

    using QuestLog = Array<Quest>;
}
