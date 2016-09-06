#include <GWCA\Managers\PlayerMgr.h>

#include <GWCA\Managers\CtoSMgr.h>

void GW::PlayerMgr::SetActiveTitle(GW::Constants::TitleID id) {
	CtoSMgr::Instance().SendPacket(0x8, 0x51, (DWORD)id);
}

void GW::PlayerMgr::RemoveActiveTitle() {
	CtoSMgr::Instance().SendPacket(0x4, 0x52);
}
