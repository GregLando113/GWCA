#pragma once

namespace GW {
    namespace Merchant {

        struct TransactionInfo {
            uint32_t itemcount;
            uint32_t* itemids;
            uint32_t* itemquantities;
        };

        struct QuoteInfo {
            uint32_t unknown; // = 0
            uint32_t itemcount;
            uint32_t* itemids;
        };

        enum TransactionType : uint32_t {
            MerchantBuy = 0x1,
            CollectorBuy,
            CrafterBuy,
            WeaponsmithCustomize,

            MerchantSell = 0xB,
            TraderBuy,
            TraderSell,

            UnlockRunePriestOfBalth = 0xF
        };

        GWCA_API void TransactItems(TransactionType type,
            uint32_t gold_give, TransactionInfo give,
            uint32_t gold_recv, TransactionInfo recv
        );

        GWCA_API void RequestQuote(TransactionType type,
            QuoteInfo give,
            QuoteInfo recv
        );

        // note: can contain pointers to random items from your inventory
        GWCA_API GW::MerchItemArray GetMerchantItemsArray();
    };
}
