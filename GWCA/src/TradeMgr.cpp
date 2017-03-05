#include <GWCA\Managers\TradeMgr.h>
#include <GWCA\Managers\CtoSMgr.h>

void GW::TradeMgr::OpenTradeWindow(DWORD agentId) {
    CtoSMgr::Instance().SendPacket(0x8, 0x43, agentId);
}

void GW::TradeMgr::AcceptTrade() {
    CtoSMgr::Instance().SendPacket(0x4, 0xB4);
}

void GW::TradeMgr::CancelTrade() {
    CtoSMgr::Instance().SendPacket(0x4, 0xAE);
}

void GW::TradeMgr::ChangeOffer() {
    CtoSMgr::Instance().SendPacket(0x4, 0xB3);
}

void GW::TradeMgr::SubmitOffer(DWORD gold) {
    CtoSMgr::Instance().SendPacket(0x8, 0xB0, gold);
}

void GW::TradeMgr::OfferItem(DWORD itemId, DWORD quantity) {
    CtoSMgr::Instance().SendPacket(0xC, 0xAF, itemId, quantity);
}