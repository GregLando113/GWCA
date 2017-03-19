#pragma once

#include <Windows.h>

namespace GW {
    namespace Trade {
        void OpenTradeWindow(DWORD agentId);
        void AcceptTrade();
        void CancelTrade();
        void ChangeOffer();
        void SubmitOffer(DWORD gold);
        void OfferItem(DWORD itemId, DWORD quantity);
    };
}
