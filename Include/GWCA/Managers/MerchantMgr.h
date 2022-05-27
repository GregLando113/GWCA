#pragma once
#include <GWCA/Constants/Types.h>

#include <GWCA/Utilities/Export.h>
#include <GWCA/GameContainers/Array.h>

namespace GW {

    struct Module;
    extern Module MerchantModule;

    typedef Array<ItemID> MerchItemArray;

    namespace Merchant {

        struct TransactionInfo {
            uint32_t  item_count = 0;
            ItemID *item_ids = nullptr;
            uint32_t *item_quantities = nullptr;
        };

        struct QuoteInfo {
            uint32_t  unknown = 0;
            uint32_t  item_count = 0;
            ItemID *item_ids = nullptr;
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
        GWCA_API MerchItemArray GetMerchantItemsArray();
    };
}
