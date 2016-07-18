#include "GuildMgr.h"

#include "GameContext.h"
#include "GameContext.h"
#include "CtoSMgr.h"

wchar_t* GWCA::GuildMgr::GetPlayerGuildAnnouncer() {
	return GameContext::instance()->guild->lastannouncementwriter;
}

wchar_t* GWCA::GuildMgr::GetPlayerGuildAnnouncement() {
	return GameContext::instance()->guild->guildannouncement;
}

DWORD GWCA::GuildMgr::GetPlayerGuildIndex() {
	GameContext::instance()->guild->playerguildindex;
}

GWCA::GW::GuildArray GWCA::GuildMgr::GetGuildArray() {
	return GameContext::instance()->guild->guilds;
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
