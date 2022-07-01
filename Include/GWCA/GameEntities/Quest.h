#pragma once

#include <GWCA/GameContainers/Array.h>

namespace GW {
    namespace Constants {
        enum class QuestID;
        enum class MapID;
    }

    struct Quest { // total: 0x34/52
        /* +h0000 */ GW::Constants::QuestID quest_id;
        /* +h0004 */ uint32_t log_state;
        /* +h0008 */ wchar_t* location; // quest category
        /* +h000C */ wchar_t* name; // quest name
        /* +h0010 */ wchar_t* npc; //
        /* +h0014 */ uint32_t map_from;
        /* +h0018 */ Vec3f    marker;
        /* +h0024 */ uint32_t h0024;
        /* +h0028 */ GW::Constants::MapID map_to;
        /* +h002C */ wchar_t* description; // namestring reward
        /* +h0030 */ wchar_t* objectives; // namestring objective
    };
    static_assert(sizeof(Quest) == 52, "struct Quest has incorect size");

    struct MissionObjective { // total: 0xC/12
        /* +h0000 */ uint32_t objective_id;
        /* +h0004 */ wchar_t *enc_str;
        /* +h0008 */ uint32_t type; // completed, bullet, etc...
    };
    static_assert(sizeof(MissionObjective) == 12, "struct MissionObjective has incorect size");

    typedef Array<Quest> QuestLog;
}
