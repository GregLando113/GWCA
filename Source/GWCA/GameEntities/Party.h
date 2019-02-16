#pragma once

#include <GWCA/GameContainers/List.h>
#include <GWCA/GameContainers/Array.h>

namespace GW {
    typedef uint32_t AgentID;
    
    struct PlayerPartyMember { // total: 0xC/12
        /* +h0000 */ uint32_t login_number;
        /* +h0004 */ uint32_t h0004;
        /* +h0008 */ uint32_t state;
        
        inline bool connected() { return (state & 1) > 0; }
        inline bool ticked()    { return (state & 2) > 0; }
    };
    static_assert(sizeof(PlayerPartyMember) == 12, "struct PlayerPartyMember has incorect size");
    
    struct HeroPartyMember { // total: 0x18/24
        /* +h0000 */ uint32_t agent_id;
        /* +h0004 */ uint32_t owner_player_id;
        /* +h0008 */ uint32_t hero_id;
        /* +h000C */ uint32_t h000C;
        /* +h0010 */ uint32_t h0010;
        /* +h0014 */ uint32_t level;
    };
    static_assert(sizeof(HeroPartyMember) == 24, "struct HeroPartyMember has incorect size");
    
    struct HenchmanPartyMember { // total: 0x34/52
        /* +h0000 */ uint32_t agent_id;
        /* +h0004 */ uint32_t h0004[10];
        /* +h002C */ uint32_t profession;
        /* +h0030 */ uint32_t level;
    };
    static_assert(sizeof(HenchmanPartyMember) == 52, "struct HenchmanPartyMember has incorect size");
    
    typedef Array<HeroPartyMember> HeroPartyMemberArray;
    typedef Array<PlayerPartyMember> PlayerPartyMemberArray;
    typedef Array<HenchmanPartyMember> HenchmanPartyMemberArray;
    
    struct PartyInfo { // total: 0x84/132
        /* +h0000 */ uint32_t party_id;
        /* +h0004 */ Array<PlayerPartyMember> players;
        /* +h0014 */ Array<HenchmanPartyMember> henchmen;
        /* +h0024 */ Array<HeroPartyMember> heroes;
        /* +h0034 */ Array<AgentID> others; // agent id of allies, minions, pets.
        /* +h0044 */ uint8_t h0044[56];
        /* +h007C */ TLink<PartyInfo> invite_link;
    };
    static_assert(sizeof(PartyInfo) == 132, "struct PartyInfo has incorect size");
}
