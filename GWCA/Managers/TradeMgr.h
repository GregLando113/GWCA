#pragma once

namespace GW {
    namespace Trade {
        GWCA_API void OpenTradeWindow(DWORD agentId);
        GWCA_API void AcceptTrade();
        GWCA_API void CancelTrade();
        GWCA_API void ChangeOffer();
        GWCA_API void SubmitOffer(DWORD gold);
        GWCA_API void OfferItem(DWORD itemId, DWORD quantity);
    };
}
