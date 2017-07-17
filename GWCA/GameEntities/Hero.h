#ifndef _ENTITIE_HERO_INC
#define _ENTITIE_HERO_INC

#include <GWCA\GameContainers\Array.h>

namespace GW {

    struct HeroFlag { // total: 0x20/36
		/* +h0000 */ DWORD HeroID;
        /* +h0004 */ AgentID heroid;
		/* +h0008 */ DWORD h0008;
		/* +h000C */ DWORD HeroBehavior;
        /* +h0010 */ Vec2f flag;
		/* +h0018 */ DWORD h0018;
        /* +h001C */ AgentID lockedtargetid;
    };

    using HeroFlagArray = Array<HeroFlag>;
}

#endif // _ENTITIE_HERO_INC
