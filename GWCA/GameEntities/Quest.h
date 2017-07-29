#ifndef _ENTITIE_QUEST_INC
#define _ENTITIE_QUEST_INC

#include <Windows.h>
#include <GWCA\GameContainers\Array.h>

namespace GW {
    struct Quest { // total: 0x34/52
        /* +h0000 */ DWORD questid;
        /* +h0004 */ DWORD logstate;
        /* +h0008 */ DWORD h0008; // quest category
        /* +h000C */ DWORD h000C; // quest name
        /* +h0010 */ DWORD h0010; //
        /* +h0014 */ DWORD mapfrom;
        /* +h0018 */ Vec3f marker;
        /* +h0024 */ DWORD h0024;
        /* +h0028 */ DWORD mapto;
        /* +h002C */ DWORD h002C; // namestring reward
        /* +h0030 */ DWORD h0030; // namestring objective
    };

    using QuestLog = Array<Quest>;
}

#endif // _ENTITIE_QUEST_INC
