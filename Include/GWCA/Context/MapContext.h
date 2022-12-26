#pragma once

#include <GWCA/GameContainers/List.h>
#include <GWCA/GameContainers/Array.h>
#include <GWCA/Utilities/Export.h>

namespace GW {
    struct PathingMap;
    struct MapProp;
    struct PropByType;
    struct PropModelInfo;

    typedef Array<PathingMap> PathingMapArray;

    struct PropsContext {
        /* +h0000 */ uint8_t pad1[0x6C];
        /* +h006C */ Array<TList<PropByType>> propsByType;
        /* +h007C */ uint8_t h007C[0x28];
        /* +h00A4 */ Array<PropModelInfo> propModels;
        /* +h00B4 */ uint8_t h00B4[0xE0];
        /* +h0194 */ Array<MapProp*> propArray;
    };
    static_assert(sizeof(PropsContext) == 0x1A4, "struct PropsContext has incorect size");


    struct MapContext {
        /* +h0000 */ float map_boundaries[5];
        /* +h0014 */ uint8_t h0014[24];
        /* +h002C */ Array<void *> spawns1; // Seem to be arena spawns. struct is X,Y,unk 4 byte value,unk 4 byte value.
        /* +h003C */ Array<void *> spawns2; // Same as above
        /* +h004C */ Array<void *> spawns3; // Same as above
        /* +h005C */ float h005C[6]; // Some trapezoid i think.
        /* +h0074 */ struct sub1 {
            struct sub2 {
                uint32_t pad1[6];
                PathingMapArray pmaps;
            } *sub2;
            /* +h0004 */ Array<uint32_t> pathing_map_block;
            /* +h0014 */ uint32_t h0014[0x13];
            /* +h0060 */ Array<TList<void*>> something_else_for_props;
            //... Bunch of arrays and shit
        } *sub1;
        /* +h0078 */ uint8_t pad1[4];
        /* +h007C */ PropsContext *props;
        //... Player coords and shit beyond this point if they are desirable :p
    };

    GWCA_API MapContext* GetMapContext();
}
