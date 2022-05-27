#pragma once
#include <GWCA/Constants/Types.h>
#include <GWCA/Utilities/Export.h>

namespace GW {
    struct Module;
    extern Module TradeModule;

    namespace Trade {

        GWCA_API void OpenTradeWindow(AgentID agent_id);
        GWCA_API void AcceptTrade();
        GWCA_API void CancelTrade();
        GWCA_API void ChangeOffer();
        GWCA_API void SubmitOffer(uint32_t gold);
        // Passing quantity = 0 will prompt the player for the amount
        GWCA_API void OfferItem(ItemID item_id, uint32_t quantity = 0);

        typedef HookCallback<ItemID, uint32_t> OfferItemCallback;
        GWCA_API void RegisterOfferItemCallback(
            HookEntry* entry,
            OfferItemCallback callback);
        GWCA_API void RemoveOfferItemCallback(
            HookEntry* entry);
    };
}
