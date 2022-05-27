#pragma once
#include <GWCA/Constants/Types.h>

#include <GWCA/GameContainers/List.h>
#include <GWCA/GameContainers/Array.h>

namespace GW {
    namespace Constants {
        enum class MapLanguage;
        enum class HeroID;
        enum class Profession;
        enum class MapLanguage;
    }

    struct PlayerPartyMember { // total: 0xC/12
        /* +h0000 */ PlayerID player_id;
        /* +h0004 */ AgentID calledTargetId;
        /* +h0008 */ uint32_t state;
        
        inline bool connected() const { return (state & 1) > 0; }
        inline bool ticked()    const { return (state & 2) > 0; }
    };
    static_assert(sizeof(PlayerPartyMember) == 12, "struct PlayerPartyMember has incorect size");
    
    struct HeroPartyMember { // total: 0x18/24
        /* +h0000 */ AgentID agent_id;
        /* +h0004 */ PlayerID owner_player_id;
        /* +h0008 */ Constants::HeroID hero_id;
        /* +h000C */ uint32_t h000C;
        /* +h0010 */ uint32_t h0010;
        /* +h0014 */ uint32_t level;
    };
    static_assert(sizeof(HeroPartyMember) == 24, "struct HeroPartyMember has incorect size");
    
    struct HenchmanPartyMember { // total: 0x34/52
        /* +h0000 */ AgentID agent_id;
        /* +h0004 */ uint32_t h0004[10];
        /* +h002C */ Constants::Profession profession;
        /* +h0030 */ uint32_t level;
    };
    static_assert(sizeof(HenchmanPartyMember) == 52, "struct HenchmanPartyMember has incorect size");
    
    typedef Array<HeroPartyMember> HeroPartyMemberArray;
    typedef Array<PlayerPartyMember> PlayerPartyMemberArray;
    typedef Array<HenchmanPartyMember> HenchmanPartyMemberArray;
    
    struct PartyInfo { // total: 0x84/132

        size_t GetPartySize() {
            return players.size() + henchmen.size() + heroes.size();
        }

        /* +h0000 */ uint32_t party_id;
        /* +h0004 */ Array<PlayerPartyMember> players;
        /* +h0014 */ Array<HenchmanPartyMember> henchmen;
        /* +h0024 */ Array<HeroPartyMember> heroes;
        /* +h0034 */ Array<AgentID> others; // agent id of allies, minions, pets.
        /* +h0044 */ uint32_t h0044[14];
        /* +h007C */ TLink<PartyInfo> invite_link;
    };
    static_assert(sizeof(PartyInfo) == 132, "struct PartyInfo has incorect size");

    enum PartySearchType {
        PartySearchType_Hunting  = 0,
        PartySearchType_Mission  = 1,
        PartySearchType_Quest    = 2,
        PartySearchType_Trade    = 3,
        PartySearchType_Guild    = 4,
    };

    struct PartySearch {
        /* +h0000 */ uint32_t party_search_id;
        /* +h0004 */ uint32_t party_search_type;
        /* +h0008 */ uint32_t hardmode;
        /* +h000C */ uint32_t district;
        /* +h0010 */ Constants::MapLanguage language;
        /* +h0014 */ uint32_t party_size;
        /* +h0018 */ uint32_t hero_count;
        /* +h001C */ wchar_t message[32];
        /* +h005C */ wchar_t party_leader[20];
        /* +h0084 */ Constants::Profession primary;
        /* +h0088 */ Constants::Profession secondary;
        /* +h008C */ uint32_t level;
        /* +h0090 */ uint32_t timestamp;
    };
}
