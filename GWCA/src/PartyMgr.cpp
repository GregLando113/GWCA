#include <GWCA\Managers\PartyMgr.h>

#include <GWCA\Managers\AgentMgr.h>
#include <GWCA\Managers\CtoSMgr.h>
#include <GWCA\Packets\CtoS.h>

GW::PartyMgr::PartyMgr() {
	BYTE* addr_tick = (BYTE*)0x0054E6B0;
	ori_tick_ = (Tick_t)hk_tick_.Detour(addr_tick, (BYTE*)DetourTick);
}

void GW::PartyMgr::RestoreHooks() {
	hk_tick_.Retour();
}

bool GW::PartyMgr::GetIsPartyTicked() {
	GW::PartyInfo* info = GetPartyInfo();
	if (info == nullptr) return false;
	if (!info->players.valid()) return false;
	for (GW::PlayerPartyMember player : info->players) {
		if (!player.ticked()) return false;
	}
	return true;
}

bool GW::PartyMgr::GetIsPartyLoaded() {
	GW::PartyInfo* info = GetPartyInfo();
	if (info == nullptr) return false;
	if (!info->players.valid()) return false;
	for (GW::PlayerPartyMember player : info->players) {
		if (!player.connected()) return false;
	}
	return true;
}

bool GW::PartyMgr::GetIsTicked(DWORD player_index) {
	GW::PartyInfo* info = GetPartyInfo();
	if (info == nullptr) return false;
	if (!info->players.valid()) return false;
	if (player_index >= info->players.size()) return false;
	return (info->players[player_index].ticked());
}

bool GW::PartyMgr::GetIsPlayerTicked() {
	GW::PartyInfo* info = GetPartyInfo();
	if (info == nullptr) return false;
	if (!info->players.valid()) return false;
	GW::Agent* me = Agents::GetPlayer();
	if (me == nullptr) return false;
	for (DWORD i = 0; i < info->players.size(); i++) {
		if (info->players[i].loginnumber == me->LoginNumber) {
			return info->players[i].ticked();
		}
	}
	return false;
}

void GW::PartyMgr::Tick(bool flag) {
	CtoSMgr::Instance().SendPacket(0x8, 0xA9, flag);
}

void GW::PartyMgr::SetHardMode(bool flag) {
    CtoSMgr::Instance().SendPacket(0x8, 0x95, flag);
}

void GW::PartyMgr::RespondToPartyRequest(bool accept) {
    CtoSMgr::Instance().SendPacket(0x8, accept ? 0x96 : 0x98, 1);
}

DWORD __stdcall GW::PartyMgr::DetourTick(DWORD unk1) {
	// this func is always called twice so use this hack to tick only once
	static bool toggle = true;
	toggle = !toggle;
	if (toggle) return 4;

	PartyMgr::Instance().Tick(!PartyMgr::Instance().GetIsPlayerTicked());
	return 4;
}

void GW::PartyMgr::FlagHero(DWORD hero_index, GW::GamePos pos) {
	DWORD heroid = Agents::GetHeroAgentID(hero_index);
	if (heroid == 0) return;
	if (heroid == Agents::GetPlayerId()) return;
	static GW::Packet::CtoS::P019 pak;
	pak.id = heroid;
	pak.pos = pos;
	CtoSMgr::Instance().SendPacket(&pak);
}

void GW::PartyMgr::FlagAll(GW::GamePos pos) {
	static GW::Packet::CtoS::P020 pak;
	pak.pos = pos;
	CtoSMgr::Instance().SendPacket(&pak);
}
