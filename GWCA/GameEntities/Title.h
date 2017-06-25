#ifndef _ENTITIE_TITLE_INC
#define _ENTITIE_TITLE_INC

#include <Windows.h>
#include <GWCA\GameContainers\Array.h>

namespace GW {
    struct Title { // total: 0x28/40
        /* +h0000 */ DWORD h0000;
        /* +h0004 */ DWORD currentpoints;
        /* +h0008 */ DWORD h0008;
        /* +h000C */ DWORD pointsneeded_currentrank;
        /* +h0010 */ DWORD h0010;
        /* +h0014 */ DWORD pointsneeded_nextrank;
        /* +h0018 */ DWORD maxtitlerank;
        /* +h001C */ DWORD h001C;
        /* +h0020 */ void* h0020[2]; // Pretty sure these are ptrs to title hash strings
    };

    using TitleArray = Array<Title>;
}

#endif // _ENTITIE_TITLE_INC
