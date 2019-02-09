#pragma once

#include <GWCA/GameContainers/Array.h>

namespace GW {
    struct PathingTrapezoid { // total: 0x30/48
        /* +h0000 */ uint32_t id;
        /* +h0004 */ PathingTrapezoid* adjacent[4];
        /* +h0014 */ uint32_t h0014;
        /* +h0018 */ float XTL;
        /* +h001C */ float XTR;
        /* +h0020 */ float YT;
        /* +h0024 */ float XBL;
        /* +h0028 */ float XBR;
        /* +h002C */ float YB;
    };
    static_assert(sizeof(PathingTrapezoid) == 48, "struct PathingTrapezoid has incorect size");

    struct PathingMap { // total: 0x54/84
        /* +h0000 */ uint32_t zplane; // ground plane = UINT_MAX, rest 0 based index
        /* +h0004 */ uint32_t h0004;
        /* +h0008 */ uint32_t h0008;
        /* +h000C */ uint32_t h000C;
        /* +h0010 */ uint32_t h0010;
        /* +h0014 */ uint32_t h0014;
        /* +h0018 */ PathingTrapezoid* trapezoids;
        /* +h001C */ uint32_t trapezoid_count;
        /* +h0020 */ uint32_t h0020[13];
    };
    static_assert(sizeof(PathingMap) == 84, "struct PathingMap has incorect size");

    using PathingMapArray = Array<PathingMap>;
}
