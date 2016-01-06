#include "GuildMgr.h"

#include "GameContext.h"
#include "CtoSMgr.h"

wchar_t* GWCA::GuildMgr::GetPlayerGuildAnnouncer() {
	return MemoryMgr::ReadPtrChain<wchar_t*>(MemoryMgr::GetContextPtr(), 2, 0x3C, 0x278);
}

wchar_t* GWCA::GuildMgr::GetPlayerGuildAnnouncement() {
	return MemoryMgr::ReadPtrChain<wchar_t*>(MemoryMgr::GetContextPtr(), 2, 0x3C, 0x78);
}

DWORD GWCA::GuildMgr::GetPlayerGuildIndex() {
	return *MemoryMgr::ReadPtrChain<DWORD*>(MemoryMgr::GetContextPtr(), 2, 0x3C, 0x60);
}

GWCA::GW::GuildArray GWCA::GuildMgr::GetGuildArray() {
	return *MemoryMgr::ReadPtrChain<GW::GuildArray*>(MemoryMgr::GetContextPtr(), 2, 0x3C, 0x2F8);
}

void GWCA::GuildMgr::TravelGH() {
	GW::GHKey playerguild = GameContext::instance()->guild->playerghkey;

	CtoSMgr::Instance().SendPacket(0x18, 0xAA, playerguild.k[0], playerguild.k[1], playerguild.k[2], playerguild.k[3]);
}

void GWCA::GuildMgr::TravelGH(GW::GHKey key) {
	CtoSMgr::Instance().SendPacket(0x18, 0xAA, key.k[0], key.k[1], key.k[2], key.k[3]);
}

void GWCA::GuildMgr::LeaveGH() {
	CtoSMgr::Instance().SendPacket(0x8, 0xAC, 0x1);
}
