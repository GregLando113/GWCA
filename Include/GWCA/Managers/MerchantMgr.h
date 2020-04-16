#pragma once

#include <GWCA/GameContainers/Array.h>
#include <GWCA/Utilities/Export.h>

#include <cstdint>



namespace GW {

    struct Module;
    extern Module MerchantModule;

    typedef uint32_t ItemID;
    typedef Array<ItemID> MerchItemArray;

    namespace Merchant {

        struct TransactionInfo {
            uint32_t  item_count;
            uint32_t *item_ids;
            uint32_t *item_quantities;
        };

        struct QuoteInfo {
            uint32_t  unknown; // = 0
            uint32_t  item_count;
            uint32_t *item_ids;
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

        GWCA_API void TransactItems(
            TransactionType type,
            uint32_t gold_give, TransactionInfo give,
            uint32_t gold_recv, TransactionInfo recv);

        GWCA_API void RequestQuote(
            TransactionType type,
            QuoteInfo give,
            QuoteInfo recv);

        // note: can contain pointers to random items from your inventory
        GWCA_API MerchItemArray GetMerchantItemsArray();
    };
}
