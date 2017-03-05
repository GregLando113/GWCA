#pragma once

#include "GWCAManager.h"
#include <GWCA\Context\GameContext.h>

namespace GW {
    class TradeMgr : public GWCAManager<TradeMgr> {
        friend class GWCAManager<TradeMgr>;

    public:
        void OpenTradeWindow(DWORD agentId);
        void AcceptTrade();
        void CancelTrade();
        void ChangeOffer();
        void SubmitOffer(DWORD gold);
        void OfferItem(DWORD itemId, DWORD quantity);
    };
}