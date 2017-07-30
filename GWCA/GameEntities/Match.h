#ifndef _ENTITIE_MATCH_INC
#define _ENTITIE_MATCH_INC

#include <Windows.h>

namespace GW {
    using wchar = wchar_t;

    struct ObserverMatch {
        /* +h0000 */ DWORD MatchID;
        /* +h0004 */ DWORD MatchID_dup;
        /* +h0008 */ DWORD MapID;
        /* +h000C */ DWORD Age;
        /* +h0010 */ DWORD h0010[14];
        /* +h0048 */ wchar *TeamNames;
    };
}

#endif // _ENTITIE_MATCH_INC
