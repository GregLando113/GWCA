#pragma once

#include <stdint.h>

#include <GWCA/GameContainers/Array.h>
#include <GWCA/GameEntities/Item.h>


namespace GW {

    struct TradeContext {
        static constexpr uint32_t TRADE_CLOSED = 0;
        static constexpr uint32_t TRADE_INITIATED = 1;
        static constexpr uint32_t TRADE_OFFER_SEND = 2;
        static constexpr uint32_t TRADE_ACCEPTED = 4;

        struct Item {
            ItemID item_id;
            uint32_t quantity;
        };

        struct Trader {
            uint32_t gold;
            Array<TradeContext::Item> items;
        };

        /* +h0000 */ uint32_t flags; // this is actually a flags
        /* +h0004 */ uint32_t h0004[3]; // Seemingly 3 null dwords
        /* +h0010 */ Trader player;
        /* +h0024 */ Trader partner;

        // bool GetPartnerAccepted();
        // bool GetPartnerOfferSent();

        bool GetIsTradeInitiated() const { return (flags & TRADE_INITIATED) != 0; }
        bool GetIsTradeAccepted()  const { return (flags & TRADE_ACCEPTED)  != 0; }
    };
}
