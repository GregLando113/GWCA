#ifndef _ENTITIE_PARTY_INC
#define _ENTITIE_PARTY_INC

#include <Windows.h>
#include <GWCA\GameContainers\Array.h>

namespace GW {
    struct PlayerPartyMember { // total: 0xC/12
        /* +h0000 */ DWORD loginnumber;
        /* +h0004 */ DWORD unk1;
        /* +h0008 */ DWORD state;

        inline bool connected() { return (state & 1) > 0; }
        inline bool ticked() { return (state & 2) > 0; }
    };

    struct HeroPartyMember { // total: 0x18/24
        /* +h0000 */ DWORD agentid;
        /* +h0004 */ DWORD ownerplayerid;
        /* +h0008 */ DWORD heroid;
        /* +h000C */ DWORD h000C;
        /* +h0010 */ DWORD h0010;
        /* +h0014 */ DWORD level;
    };

    struct HenchmanPartyMember { // total: 0x34/52
        /* +h0000 */ DWORD agentid;
        /* +h0004 */ DWORD h0004[10];
        /* +h002C */ DWORD profession;
        /* +h0030 */ DWORD level;
    };

    using PlayerPartyMemberArray = Array<PlayerPartyMember>;
    using HeroPartyMemberArray = Array<HeroPartyMember>;
    using HenchmanPartyMemberArray = Array<HenchmanPartyMember>;

    struct PartyInfo { // total: 0x44/68
        /* +h0000 */ DWORD h0000;
        /* +h0004 */ Array<PlayerPartyMember> players;
        /* +h0014 */ Array<HenchmanPartyMember> henchmen;
        /* +h0024 */ Array<HeroPartyMember> heroes;
        /* +h0034 */ Array<DWORD> others; // agent id of allies, minions, pets.
    };

}

#endif // _ENTITIE_PARTY_INC
