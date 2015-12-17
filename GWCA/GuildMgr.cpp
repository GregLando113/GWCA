#include "GuildMgr.h"

#include "GWAPIMgr.h"

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
	GW::Guild* playerguild = GetGuildArray()[GetPlayerGuildIndex()];

	api().CtoS().SendPacket(0x18, 0xAA, playerguild->GuildHallKey[0], playerguild->GuildHallKey[1], playerguild->GuildHallKey[2], playerguild->GuildHallKey[3]);
}

void GWAPI::GuildMgr::TravelGH(DWORD* key)
{
	api().CtoS().SendPacket(0x18, 0xAA, key[0], key[1], key[2], key[3]);
}

void GWAPI::GuildMgr::LeaveGH()
{
	api().CtoS().SendPacket(0x8, 0xAC, 0x1);
}
