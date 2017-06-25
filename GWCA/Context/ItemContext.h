#ifndef _ITEM_CONTEXT_INC
#define _ITEM_CONTEXT_INC

#include <Windows.h>

#include <GWCA\GameContainers\Array.h>
#include <GWCA\GameEntities\Item.h>

namespace GW {
    struct ItemContext { // total: 0x10C/268 BYTEs
        /* +h0000 */ Array<void*> h0000;
        /* +h0010 */ Array<void*> h0010;
        /* +h0020 */ DWORD h0020;
        /* +h0024 */ Array<Bag*> BagsArray;
        /* +h0034 */ char h0034[0xC];
        /* +h0040 */ Array<void*> h0040;
        /* +h0050 */ Array<void*> h0050;
        /* +h0060 */ char h0060[0x58];
        /* +h00B8 */ Array<Item*> itemarray;
        /* +h00C8 */ char h00C8[0x30];
        /* +h00F8 */ Inventory *inventory;
        /* +h00FC */ Array<void*> h00FC;
    };
}

#endif // _ITEM_CONTEXT_INC
