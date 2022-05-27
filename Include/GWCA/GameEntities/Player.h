#pragma once
#include <GWCA/Constants/Constants.h>
#include <GWCA/GameContainers/Array.h>
#include <GWCA/Constants/Types.h>

namespace GW {
    struct Player { // total: 0x4C/76
        /* +h0000 */ AgentID agent_id;
        /* +h0004 */ uint32_t h0004[3];
        /* +h0010 */ uint32_t appearance_bitmap;
        /* +h0014 */ uint32_t flags; // Bitwise field
        /* +h0018 */ Constants::Profession primary;
        /* +h001C */ Constants::Profession secondary;
        /* +h0020 */ uint32_t h0020;
        /* +h0024 */ wchar_t *name_enc;
        /* +h0028 */ wchar_t *name;
        /* +h002C */ PlayerID party_leader_player_id;
        /* +h0030 */ uint32_t active_title_tier;
        /* +h0034 */ PlayerID player_id;
        /* +h0038 */ uint32_t party_size;
        /* +h003C */ Array<void*> h003C;

        inline bool IsPvP() {
            return (flags & 0x800) != 0;
        }

    };
    static_assert(sizeof(Player) == 0x4c, "struct Player has incorect size");

    typedef Array<Player> PlayerArray;
}
