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
        /* +h001C */ TList<PartyInfo> requests;
        /* +h0028 */ DWORD requests_count;
        /* +h002C */ TList<PartyInfo> sending;
        /* +h0038 */ DWORD sending_count;
        /* +h003C */ DWORD h003C;
        /* +h0040 */ Array<PartyInfo*> parties; 
        /* +h0050 */ DWORD h0050;
        /* +h0054 */ PartyInfo* playerparty; // Players party
        
        bool InHardMode() { return (flag & 0x10) > 0; }
        bool IsDefeated() { return (flag & 0x20) > 0; }
    };
}

#endif // _PARTY_CONTEXT_INC
