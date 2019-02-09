#pragma once

#include <GWCA/GameContainers/Array.h>

namespace GW {
    using PlayerID = uint32_t;
    using wchar = wchar_t;

    struct Player { // total: 0x4C/76
        /* +h0000 */ uint32_t agent_id;
        /* +h0004 */ uint32_t h0004[5];
        /* +h0018 */ uint32_t primary;
        /* +h001C */ uint32_t secondary;
        /* +h0020 */ uint32_t h0020;
        /* +h0024 */ wchar_t *name_enc;
        /* +h0028 */ wchar_t *name;
        /* +h002C */ uint32_t h002C;
        /* +h0030 */ uint32_t active_title;
        /* +h0034 */ uint32_t h0034;
        /* +h0038 */ uint32_t party_size;
        /* +h003C */ uint32_t h003C[4];
    };
    static_assert(sizeof(Player) == 76, "struct Player has incorect size");

    using PlayerArray = Array<Player>;
}
