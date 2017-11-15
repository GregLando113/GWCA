#ifndef _CHAR_CONTEXT_INC
#define _CHAR_CONTEXT_INC

#include <Windows.h>

#include <GWCA\GameContainers\Array.h>
#include <GWCA\GameEntities\Match.h>

namespace GW {
    typedef wchar_t wchar;
    struct CharContext { // total: 0x03A8
        /* +h0000 */ Array<void*> h0000;
        /* +h0010 */ char h0010[4];
        /* +h0014 */ Array<void*> h0014;
        /* +h0024 */ char h0024[0x10];
        /* +h0034 */ Array<void*> h0034;
        /* +h0044 */ Array<void*> h0044;
        /* +h0054 */ char h0054[0x20];
        /* +h0074 */ wchar playername[20];
        /* +h009C */ char h009C[0x50];
        /* +h00EC */ Array<void*> h00EC;
        /* +h00FC */ char h00FC[0x28];
        /* +h0124 */ DWORD h0124;
        /* +h0128 */ DWORD token1; // world id
        /* +h012C */ DWORD mapidagain;
        /* +h0130 */ DWORD is_explorable;
        /* +h0134 */ BYTE host[24];
        /* +h014C */ DWORD token2; // player id
        /* +h0150 */ char h0130[0x40];
        /* +h0190 */ DWORD currentmapid_again;
        /* +h0194 */ char h0194[0x28];
        /* +h01BC */ DWORD currentmapid;
        /* +h01C0 */ DWORD currentmapid_dupe;
        /* +h01C4 */ char h01C4[0x8];
        /* +h01CC */ Array<ObserverMatch*> ObserverMatchs;
        /* +h01DC */ char h001DC[0x14C];
        /* +h0328 */ wchar playeremail[64];
    };
}

#endif // _CHAR_CONTEXT_INC
