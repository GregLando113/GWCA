#pragma once

namespace GW {
    struct PartyInfo;

    struct PartyContext { // total: 0x58/88
        /* +h0000 */ uint32_t h0000;
        /* +h0004 */ Array<void *> h0004;
        /* +h0014 */ uint32_t flag;
        /* +h0018 */ uint32_t h0018;
        /* +h001C */ TList<PartyInfo> requests;
        /* +h0028 */ uint32_t requests_count;
        /* +h002C */ TList<PartyInfo> sending;
        /* +h0038 */ uint32_t sending_count;
        /* +h003C */ uint32_t h003C;
        /* +h0040 */ Array<PartyInfo *> parties; 
        /* +h0050 */ uint32_t h0050;
        /* +h0054 */ PartyInfo *player_party; // Players party
        
        bool InHardMode() { return (flag & 0x10) > 0; }
        bool IsDefeated() { return (flag & 0x20) > 0; }
    };
}
