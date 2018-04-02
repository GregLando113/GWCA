#include <GWCA\Managers\TradeMgr.h>

#include <GWCA\Managers\CtoSMgr.h>

void GW::Trade::OpenTradeWindow(DWORD agentId) {
    CtoS::SendPacket(0x8, 0x50, agentId);
}

void GW::Trade::AcceptTrade() {
	CtoS::SendPacket(0x4, 7);
}

void GW::Trade::CancelTrade() {
	CtoS::SendPacket(0x4, 1);
}

void GW::Trade::ChangeOffer() {
	CtoS::SendPacket(0x4, 6);
}

void GW::Trade::SubmitOffer(DWORD gold) {
	CtoS::SendPacket(0x8, 3, gold);
}

void GW::Trade::OfferItem(DWORD itemId, DWORD quantity) {
	CtoS::SendPacket(0xC, 2, itemId, quantity);
}
