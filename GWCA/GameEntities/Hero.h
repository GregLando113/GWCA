#ifndef _ENTITIE_HERO_INC
#define _ENTITIE_HERO_INC

#include <GWCA\GameContainers\Array.h>

namespace GW {

    struct HeroFlag {
		DWORD HeroID;
        AgentID heroid;
		DWORD unk1;
		DWORD HeroBehavior;
        Vec2f flag;
		DWORD unk2;
        AgentID lockedtargetid;
    };

    using HeroFlagArray = Array<HeroFlag>;
}

#endif // _ENTITIE_HERO_INC
