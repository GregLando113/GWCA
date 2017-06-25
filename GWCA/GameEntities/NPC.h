#ifndef _ENTITIE_NPC_INC
#define _ENTITIE_NPC_INC

#include <Windows.h>
#include <GWCA\GameContainers\Array.h>

namespace GW {
	using wchar = wchar_t;

    struct NPC { // total: 0x30/48
        /* +h0000 */ DWORD ModelFileID;
        /* +h0004 */ DWORD h0004[3];
        /* +h0010 */ DWORD NpcFlags;
        /* +h0014 */ DWORD h00014[3];
        /* +h0020 */ wchar *NameString;
        /* +h0024 */ DWORD h0024[3];
    };
    using NPCArray = Array<NPC>;
}

#endif // _ENTITIE_NPC_INC
