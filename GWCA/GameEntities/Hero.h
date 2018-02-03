#ifndef _ENTITIE_HERO_INC
#define _ENTITIE_HERO_INC

#include <GWCA\GameContainers\Array.h>

namespace GW {

    struct HeroFlag { // total: 0x20/36
        /* +h0000 */ DWORD HeroID;
        /* +h0004 */ AgentID heroid;
        /* +h0008 */ DWORD Level;
        /* +h000C */ DWORD HeroBehavior;
        /* +h0010 */ Vec2f flag;
        /* +h0018 */ DWORD h0018;
        /* +h001C */ AgentID lockedtargetid;
    };

    struct HeroInfo { // total: 0x78/120
        /* +h0000 */ DWORD HeroID;
        /* +h0004 */ DWORD AgentID;
        /* +h0008 */ DWORD Level;
        /* +h000C */ DWORD Primary; // Primary profession 0-10 (None,W,R,Mo,N,Me,E,A,Rt,P,D)
        /* +h0010 */ DWORD Secondary; // Primary profession 0-10 (None,W,R,Mo,N,Me,E,A,Rt,P,D)
        /* +h0014 */ DWORD HeroFileID;
        /* +h0018 */ DWORD ModelFileID;
        /* +h001C */ DWORD h001C[23];
    };

    using HeroFlagArray = Array<HeroFlag>;
    using HeroInfoArray = Array<HeroInfo>;
}

#endif // _ENTITIE_HERO_INC
