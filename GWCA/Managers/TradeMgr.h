#pragma once

namespace GW {
    namespace Trade {
        GWCA_API void OpenTradeWindow(uint32_t agent_id);
        GWCA_API void AcceptTrade();
        GWCA_API void CancelTrade();
        GWCA_API void ChangeOffer();
        GWCA_API void SubmitOffer(uint32_t gold);
        GWCA_API void OfferItem(uint32_t item_id, uint32_t quantity);
    };
}
