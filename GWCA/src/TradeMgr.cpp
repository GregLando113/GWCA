#include <Windows.h>

#include <GWCA/CtoSHeaders.h>
#include <GWCA/Utilities/Export.h>

#include <GWCA/Managers/CtoSMgr.h>
#include <GWCA/Managers/TradeMgr.h>

void GW::Trade::OpenTradeWindow(DWORD agentId) {
    CtoS::SendPacket(0x8, CtoGS_MSGStartTrade, agentId);
}

void GW::Trade::AcceptTrade() {
    CtoS::SendPacket(0x4, CtoGS_MSGAcceptTrade);
}

void GW::Trade::CancelTrade() {
    CtoS::SendPacket(0x4, CtoGS_MSGCancelTrade);
}

void GW::Trade::ChangeOffer() {
    CtoS::SendPacket(0x4, CtoGS_MSGChangeOffer);
}

void GW::Trade::SubmitOffer(DWORD gold) {
    CtoS::SendPacket(0x8, CtoGS_MSGSubmitOffer, gold);
}

void GW::Trade::OfferItem(DWORD itemId, DWORD quantity) {
    CtoS::SendPacket(0xC, CtoGS_MSGAddItemTrade, itemId, quantity);
}
