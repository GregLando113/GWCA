#pragma once

#include <GWCA/Utilities/Export.h>

namespace GW {
    struct Module;
    extern Module TradeModule;

    namespace Trade {

        GWCA_API void OpenTradeWindow(uint32_t agent_id);
        GWCA_API void AcceptTrade();
        GWCA_API void CancelTrade();
        GWCA_API void ChangeOffer();
        GWCA_API void SubmitOffer(uint32_t gold);
        // Passing quantity = 0 will prompt the player for the amount
        GWCA_API void OfferItem(uint32_t item_id, uint32_t quantity = 0);

        typedef HookCallback<uint32_t, uint32_t> OfferItemCallback;
        GWCA_API void RegisterOfferItemCallback(
            HookEntry* entry,
            OfferItemCallback callback);
        GWCA_API void RemoveOfferItemCallback(
            HookEntry* entry);
    };
}
