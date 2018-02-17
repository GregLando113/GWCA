#ifndef _PARTY_CONTEXT_INC
#define _PARTY_CONTEXT_INC

#include <Windows.h>

#include <GWCA\GameEntities\Party.h>

namespace GW {
    struct PartyContext { // total: 0x58/88
        /* +h0000 */ DWORD h0000;
        /* +h0004 */ Array<void*> h0004;
        /* +h0014 */ DWORD flag;
        /* +h0018 */ DWORD h0018;
        struct { // Invite requests, recieving and sending to your party
            TList<PartyInfo> requests;
            DWORD count;
        } recieving, sending;
        /* +h003C */ DWORD h003C;
        /* +h0040 */ Array<PartyInfo*> parties; 
        /* +h0054 */ PartyInfo* playerparty; // Players party
        
        bool InHardMode() { return (flag & 0x10) > 0; }
        bool IsDefeated() { return (flag & 0x20) > 0; }
    };
}

#endif // _PARTY_CONTEXT_INC
