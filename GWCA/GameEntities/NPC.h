#ifndef _ENTITIE_NPC_INC
#define _ENTITIE_NPC_INC

#include <Windows.h>
#include <GWCA\GameContainers\Array.h>

namespace GW {
    using wchar = wchar_t;

    struct NPC { // total: 0x30/48
        /* +h0000 */ DWORD ModelFileID;
        /* +h0004 */ DWORD h0004;
        /* +h0008 */ DWORD scale; // I think, 2 highest order bytes are percent of size, so 0x64000000 is 100%
        /* +h000C */ DWORD h000C;
        /* +h0010 */ DWORD NpcFlags;
        /* +h0014 */ DWORD Profession;
        /* +h0018 */ DWORD h0018[2];
        /* +h0020 */ wchar *NameString;
        /* +h0024 */ DWORD *ModelFiles;
        /* +h0028 */ DWORD h0028; // probably length of ModelFile
        /* +h002C */ DWORD h002C; // probably capacity of ModelFile
    };
    using NPCArray = Array<NPC>;
}

#endif // _ENTITIE_NPC_INC
