#pragma once

#include <GWCA/GameContainers/Array.h>

namespace GW {
    struct PathingMap;

    typedef Array<PathingMap> PathingMapArray;

    struct MapContext {
        /* +h0000 */ float map_boundaries[5];
        /* +h0014 */ uint8_t h0014[24];
        /* +h002C */ Array<void *> spawns1; // Seem to be arena spawns. struct is X,Y,unk 4 byte value,unk 4 byte value.
        /* +h003C */ Array<void *> spawns2; // Same as above
        /* +h004C */ Array<void *> spawns3; // Same as above
        /* +h005C */ float h005C[6]; // Some trapezoid i think.
        struct sub1 {
            struct sub2 {
                uint8_t pad1[24];
                PathingMapArray pmaps;
            } *sub2;
            //... Bunch of arrays and shit
        } *sub1;
        //... Player coords and shit beyond this point if they are desirable :p
    };
}
