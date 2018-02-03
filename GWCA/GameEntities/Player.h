#ifndef _ENTITIE_PLAYER_INC
#define _ENTITIE_PLAYER_INC

#include <GWCA\GameContainers\Array.h>

namespace GW {
    using PlayerID = DWORD;
    using wchar = wchar_t;

    struct Player { // total: 0x4C/76
        /* +h0000 */ DWORD AgentID;
        /* +h0004 */ DWORD h0004[5];
        /* +h0018 */ DWORD Primary;
        /* +h001C */ DWORD Secondary;
        /* +h0020 */ DWORD h0020;
        /* +h0024 */ wchar *NameEnc;
        /* +h0028 */ wchar *Name;
        /* +h002C */ DWORD h002C;
        /* +h0030 */ DWORD ActiveTitle;
        /* +h0034 */ DWORD h0034;
        /* +h0038 */ DWORD PartySize;
        /* +h003C */ DWORD h003C[4];
    };

    using PlayerArray = Array<Player>;
}

#endif // _ENTITIE_PLAYER_INC
