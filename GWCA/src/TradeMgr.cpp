#include <GWCA\Managers\TradeMgr.h>

#include <GWCA\Managers\CtoSMgr.h>

void GW::Trade::OpenTradeWindow(DWORD agentId) {
    CtoS::SendPacket(0x8, 0x49, agentId);
}

void GW::Trade::AcceptTrade() {
	CtoS::SendPacket(0x4, 0xBA);
}

void GW::Trade::CancelTrade() {
	CtoS::SendPacket(0x4, 0xB4);
}

void GW::Trade::ChangeOffer() {
	CtoS::SendPacket(0x4, 0xB9);
}

void GW::Trade::SubmitOffer(DWORD gold) {
	CtoS::SendPacket(0x8, 0xB6, gold);
}

void GW::Trade::OfferItem(DWORD itemId, DWORD quantity) {
	CtoS::SendPacket(0xC, 0xB5, itemId, quantity);
}
