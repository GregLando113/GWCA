#include <GWCA\Managers\TradeMgr.h>

#include <GWCA\Managers\CtoSMgr.h>

void GW::Trade::OpenTradeWindow(DWORD agentId) {
    CtoS::SendPacket(0x8, 0x43, agentId);
}

void GW::Trade::AcceptTrade() {
	CtoS::SendPacket(0x4, 0xB4);
}

void GW::Trade::CancelTrade() {
	CtoS::SendPacket(0x4, 0xAE);
}

void GW::Trade::ChangeOffer() {
	CtoS::SendPacket(0x4, 0xB3);
}

void GW::Trade::SubmitOffer(DWORD gold) {
	CtoS::SendPacket(0x8, 0xB0, gold);
}

void GW::Trade::OfferItem(DWORD itemId, DWORD quantity) {
	CtoS::SendPacket(0xC, 0xAF, itemId, quantity);
}
