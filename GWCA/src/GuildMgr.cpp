#include <GWCA\Managers\GuildMgr.h>

#include <GWCA\Context\GameContext.h>
#include <GWCA\Context\GuildContext.h>
#include <GWCA\Managers\CtoSMgr.h>

GW::GuildContext* GW::GuildMgr::GetGuildContext() {
	return GameContext::instance()->guild;
}

wchar_t* GW::GuildMgr::GetPlayerGuildAnnouncer() {
	return GameContext::instance()->guild->AnnouncementAuthor;
}

wchar_t* GW::GuildMgr::GetPlayerGuildAnnouncement() {
	return GameContext::instance()->guild->Announcement;
}

DWORD GW::GuildMgr::GetPlayerGuildIndex() {
	return GameContext::instance()->guild->playerguildindex;
}

GW::GuildArray GW::GuildMgr::GetGuildArray() {
	return GameContext::instance()->guild->guilds;
}

void GW::GuildMgr::TravelGH() {
	GW::GHKey playerguild = GameContext::instance()->guild->playerghkey;

	CtoS::SendPacket(0x18, 0xAA, playerguild.k[0], playerguild.k[1], playerguild.k[2], playerguild.k[3]);
}

void GW::GuildMgr::TravelGH(GW::GHKey key) {
	CtoS::SendPacket(0x18, 0xAA, key.k[0], key.k[1], key.k[2], key.k[3]);
}

void GW::GuildMgr::LeaveGH() {
	CtoS::SendPacket(0x8, 0xAC, 0x1);
}
