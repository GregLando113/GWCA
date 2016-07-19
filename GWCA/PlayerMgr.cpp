#include "PlayerMgr.h"

#include "CtoSMgr.h"

void GWCA::PlayerMgr::SetActiveTitle(GwConstants::TitleID id) {
	CtoSMgr::Instance().SendPacket(0x8, 0x51, (DWORD)id);
}

void GWCA::PlayerMgr::RemoveActiveTitle() {
	CtoSMgr::Instance().SendPacket(0x4, 0x52);
}