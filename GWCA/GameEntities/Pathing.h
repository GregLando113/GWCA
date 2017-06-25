#ifndef _ENTITIE_PATHING_INC
#define _ENTITIE_PATHING_INC

#include <Windows.h>
#include <GWCA\GameContainers\Array.h>

namespace GW {
    struct PathingTrapezoid { // total: 0x30/48
        /* +h0000 */ DWORD ID;
        /* +h0004 */ PathingTrapezoid* Adjacent[4];
        /* +h0014 */ DWORD idekwhyisthisherebutok;
        /* +h0018 */ float XTL;
        /* +h001C */ float XTR;
        /* +h0020 */ float YT;
        /* +h0024 */ float XBL;
        /* +h0028 */ float XBR;
        /* +h002C */ float YB;
    };

    struct PathingMap { // total: 0x54/84
        /* +h0000 */ DWORD zplane; // ground plane = UINT_MAX, rest 0 based index
        /* +h0004 */ DWORD h0004;
        /* +h0008 */ DWORD h0008;
        /* +h000C */ DWORD h000C;
        /* +h0010 */ DWORD h0010;
        /* +h0014 */ DWORD h0014;
        /* +h0018 */ PathingTrapezoid* trapezoids;
        /* +h001C */ DWORD trapezoidcount;
        /* +h0020 */ DWORD h0020[13];
    };

    using PathingMapArray = Array<PathingMap>;

}

#endif // _ENTITIE_PATHING_INC
