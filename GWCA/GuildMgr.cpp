#include "GuildMgr.h"

#include "GWAPIMgr.h"
#include "GameContext.h"

wchar_t* GWAPI::GuildMgr::GetPlayerGuildAnnouncer()
{
	return MemoryMgr::ReadPtrChain<wchar_t*>(MemoryMgr::GetContextPtr(), 2, 0x3C, 0x278);
}

wchar_t* GWAPI::GuildMgr::GetPlayerGuildAnnouncement()
{
	return MemoryMgr::ReadPtrChain<wchar_t*>(MemoryMgr::GetContextPtr(), 2, 0x3C, 0x78);
}

DWORD GWAPI::GuildMgr::GetPlayerGuildIndex()
{
	return *MemoryMgr::ReadPtrChain<DWORD*>(MemoryMgr::GetContextPtr(), 2, 0x3C, 0x60);
}

GWAPI::GW::GuildArray GWAPI::GuildMgr::GetGuildArray()
{
	return *MemoryMgr::ReadPtrChain<GW::GuildArray*>(MemoryMgr::GetContextPtr(), 2, 0x3C, 0x2F8);
}

void GWAPI::GuildMgr::TravelGH()
{
	GW::GHKey playerguild = GameContext::instance()->guild->playerghkey;

	api().CtoS().SendPacket(0x18, 0xAA, playerguild.k[0], playerguild.k[1], playerguild.k[2], playerguild.k[3]);
}

void GWAPI::GuildMgr::TravelGH(GW::GHKey key)
{
	api().CtoS().SendPacket(0x18, 0xAA, key.k[0], key.k[1], key.k[2], key.k[3]);
}

void GWAPI::GuildMgr::LeaveGH()
{
	api().CtoS().SendPacket(0x8, 0xAC, 0x1);
}
