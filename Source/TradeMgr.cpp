#include "stdafx.h"

#include <GWCA/Packets/Opcodes.h>
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
        NULL,           // enable_hooks
        NULL,           // disable_hooks
    };

    void Trade::OpenTradeWindow(uint32_t agent_id) {
        CtoS::SendPacket(0x8, GAME_CMSG_TRADE_INITIATE, agent_id);
    }

    void Trade::AcceptTrade() {
        CtoS::SendPacket(0x4, GAME_CMSG_TRADE_ACCEPT);
    }

    void Trade::CancelTrade() {
        CtoS::SendPacket(0x4, GAME_CMSG_TRADE_CANCEL);
    }

    void Trade::ChangeOffer() {
        CtoS::SendPacket(0x4, GAME_CMSG_TRADE_CANCEL_OFFER);
    }

    void Trade::SubmitOffer(uint32_t gold) {
        CtoS::SendPacket(0x8, GAME_CMSG_TRADE_SEND_OFFER, gold);
    }

    void Trade::OfferItem(uint32_t item_id, uint32_t quantity) {
        CtoS::SendPacket(0xC, GAME_CMSG_TRADE_ADD_ITEM, item_id, quantity);
    }
} // namespace GW
