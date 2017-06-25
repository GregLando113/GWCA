#ifndef _PARTY_CONTEXT_INC
#define _PARTY_CONTEXT_INC

#include <Windows.h>

#include <GWCA\GameContainers\Array.h>
#include <GWCA\GameEntities\Party.h>

namespace GW {
    struct PartyContext { // total: 0x58/88
        /* +h0000 */ DWORD h0000;
        /* +h0004 */ Array<void*> h0004;
        /* +h0014 */ DWORD flag;
        /* +h0018 */ DWORD h0018[4];
        /* +h0028 */ DWORD InviteCount;
        /* +h002C */ char pad_0x002C[0x28];
        /* +h0054 */ PartyInfo *partyinfo;

        bool InHardMode() { return (flag & 0x10) > 0; }
        bool IsDefeated() { return (flag & 0x20) > 0; }
    };
}

#endif // _PARTY_CONTEXT_INC
