#pragma once
#include <GWCA/GameEntities/Item.h>
#include <GWCA/GameContainers/Array.h>

namespace GW {
    struct ItemContext { // total: 0x10C/268 BYTEs

        static ItemContext* instance();

        /* +h0000 */ Array<void *> h0000;
        /* +h0010 */ Array<void *> h0010;
        /* +h0020 */ DWORD h0020;
        /* +h0024 */ Array<Bag *> bags_array;
        /* +h0034 */ char h0034[12];
        /* +h0040 */ Array<void *> h0040;
        /* +h0050 */ Array<void *> h0050;
        /* +h0060 */ char h0060[88];
        /* +h00B8 */ Array<Item *> item_array;
        /* +h00C8 */ char h00C8[48];
        /* +h00F8 */ Inventory *inventory;
        /* +h00FC */ Array<void *> h00FC;
    };
}
