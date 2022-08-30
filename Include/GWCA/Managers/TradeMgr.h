#pragma once

#include <GWCA/Utilities/Export.h>

namespace GW {
    struct Module;
    extern Module TradeModule;

    namespace Trade {

        GWCA_API bool OpenTradeWindow(uint32_t agent_id);
        GWCA_API bool AcceptTrade();
        GWCA_API bool CancelTrade();
        GWCA_API bool ChangeOffer();
        GWCA_API bool SubmitOffer(uint32_t gold);
        GWCA_API bool RemoveItem(uint32_t slot);

        // Passing quantity = 0 will prompt the player for the amount
        GWCA_API bool OfferItem(uint32_t item_id, uint32_t quantity = 0);

        typedef HookCallback<uint32_t, uint32_t> OfferItemCallback;
        GWCA_API void RegisterOfferItemCallback(
            HookEntry* entry,
            const OfferItemCallback& callback);
        GWCA_API void RemoveOfferItemCallback(
            HookEntry* entry);
    };
}
