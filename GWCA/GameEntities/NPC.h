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
        /* +h000C */ DWORD Sex;
        /* +h0010 */ DWORD NpcFlags;
        /* +h0014 */ DWORD Primary;
        /* +h0018 */ DWORD h0018;
		/* +h001C */ BYTE DefaultLevel;
		// +h001D    BYTE padding;
		// +h001E    WORD padding;
        /* +h0020 */ wchar *NameString;
        /* +h0024 */ DWORD *ModelFiles;
        /* +h0028 */ DWORD FilesCount; // length of ModelFile
        /* +h002C */ DWORD FilesCapacity; // capacity of ModelFile
    };
    using NPCArray = Array<NPC>;
}

#endif // _ENTITIE_NPC_INC
