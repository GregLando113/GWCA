#include "stdafx.h"

#include <GWCA/CtoSHeaders.h>
#include <GWCA/Utilities/Export.h>

#include <GWCA/Managers/Module.h>
#include <GWCA/Managers/CtoSMgr.h>
#include <GWCA/Managers/TradeMgr.h>

namespace GW {
    Module TradeModule = {
        "TradeModule",  // name
        NULL,           // param
        NULL,           // init_module
        NULL,           // exit_module
        NULL,           // exit_module
        NULL,           // remove_hooks
    };

    void Trade::OpenTradeWindow(uint32_t agent_id) {
        CtoS::SendPacket(0x8, CtoGS_MSGStartTrade, agent_id);
    }

    void Trade::AcceptTrade() {
        CtoS::SendPacket(0x4, CtoGS_MSGAcceptTrade);
    }

    void Trade::CancelTrade() {
        CtoS::SendPacket(0x4, CtoGS_MSGCancelTrade);
    }

    void Trade::ChangeOffer() {
        CtoS::SendPacket(0x4, CtoGS_MSGChangeOffer);
    }

    void Trade::SubmitOffer(uint32_t gold) {
        CtoS::SendPacket(0x8, CtoGS_MSGSubmitOffer, gold);
    }

    void Trade::OfferItem(uint32_t item_id, uint32_t quantity) {
        CtoS::SendPacket(0xC, CtoGS_MSGAddItemTrade, item_id, quantity);
    }
} // namespace GW
