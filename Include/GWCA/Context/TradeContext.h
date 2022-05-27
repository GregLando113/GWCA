#pragma once
#include <GWCA/Constants/Types.h>

#include <GWCA/GameContainers/Array.h>
#include <GWCA/GameEntities/Item.h>


namespace GW {
    enum class TradeFlags {
        Closed,
        Initiated,
        OfferSent,
        Accepted = 4
    };
    struct TradeItem {
        ItemID item_id;
        uint32_t quantity;
    };
    struct Trader {
        uint32_t gold;
        Array<TradeItem> items;
    };
    struct TradeContext {

        static TradeContext* instance();

        /* +h0000 */ uint32_t flags; // this is actually a flags
        /* +h0004 */ uint32_t h0004[3]; // Seemingly 3 null dwords
        /* +h0010 */ Trader player;
        /* +h0024 */ Trader partner;

        // bool GetPartnerAccepted();
        // bool GetPartnerOfferSent();

        bool GetIsTradeInitiated() const { return (flags & (int)TradeFlags::Closed) != 0; }
        bool GetIsTradeAccepted()  const { return (flags & (int)TradeFlags::Accepted)  != 0; }
    };
}
