#include <GWCA\Managers\PartyMgr.h>

#include <cmath>
#include <GWCA\Context\GameContext.h>
#include <GWCA\Context\PartyContext.h>

#include <GWCA\Managers\AgentMgr.h>
#include <GWCA\Managers\CtoSMgr.h>
#include <GWCA\Packets\CtoS.h>

#include <GWCA\Utilities\Scanner.h>

namespace {
	typedef DWORD(__stdcall *Tick_t)(DWORD unk1);
	GW::THook<Tick_t> hk_tick_;

	// Parameter is always 1 or 2 creating "Ready" or "Not ready"
	DWORD __stdcall DetourTick(DWORD unk1) {
		// this func is always called twice so use this hack to tick only once
		static bool toggle = true;
		toggle = !toggle;
		if (toggle) return 4;

		GW::PartyMgr::Tick(!GW::PartyMgr::GetIsPlayerTicked());
		return 4;
	}
}

void GW::PartyMgr::SetTickToggle() {
	if (hk_tick_.Empty()) {
		Tick_t addr_tick = (Tick_t)Scanner::Find("\x74\x0A\x48\x75\x14\xB9", "xxxxxx", -33);
		printf("addr_tick = %p\n", addr_tick);
		hk_tick_.Detour(addr_tick, DetourTick);
	}
}

void GW::PartyMgr::RestoreTickToggle() {
	hk_tick_.Retour();
}

void GW::PartyMgr::Tick(bool flag) {
	CtoS::SendPacket(0x8, 0xA9, flag);
}

GW::PartyInfo* GW::PartyMgr::GetPartyInfo() {
	return GameContext::instance()->party->partyinfo;
}

DWORD GW::PartyMgr::GetPartySize() {
	GW::PartyInfo* info = GetPartyInfo();
	if (info == nullptr) return 0;
	return info->players.size() + info->heroes.size() + info->henchmen.size();
}

DWORD GW::PartyMgr::GetPartyPlayerCount() {
	if (GetPartyInfo()) {
		return GetPartyInfo()->players.size();
	} else {
		return 0;
	}
}
DWORD GW::PartyMgr::GetPartyHeroCount() {
	if (GetPartyInfo()) {
		return GetPartyInfo()->heroes.size();
	} else {
		return 0;
	}
}
DWORD GW::PartyMgr::GetPartyHenchmanCount() {
	if (GetPartyInfo()) {
		return GetPartyInfo()->henchmen.size();
	} else {
		return 0;
	}
}

bool GW::PartyMgr::GetIsPartyDefeated() {
	return GameContext::instance()->party->IsDefeated(); 
}

void GW::PartyMgr::SetHardMode(bool flag) {
	CtoS::SendPacket(0x8, 0x95, flag);
}
bool GW::PartyMgr::GetIsPartyInHardMode() {
	return GameContext::instance()->party->InHardMode();
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

void GW::PartyMgr::RespondToPartyRequest(bool accept) {
	CtoS::SendPacket(0x8, accept ? 0x96 : 0x98, 1);
}

void GW::PartyMgr::AddHero(DWORD heroid) {
	CtoS::SendPacket(0x8, 0x17, heroid);
}

void GW::PartyMgr::KickHero(DWORD heroid) {
	CtoS::SendPacket(0x8, 0x18, heroid);
}

void GW::PartyMgr::KickAllHeroes() {
	CtoS::SendPacket(0x8, 0x18, 0x26);
}

void GW::PartyMgr::LeaveParty() {
	CtoS::SendPacket(0x4, 0x9C);
}

void GW::PartyMgr::FlagHero(DWORD hero_index, GW::GamePos pos) {
	DWORD heroid = Agents::GetHeroAgentID(hero_index);
	if (heroid == 0) return;
	if (heroid == Agents::GetPlayerId()) return;
	static GW::Packet::CtoS::P019 pak;
	pak.id = heroid;
	pak.pos = pos;
	CtoS::SendPacket(&pak);
}

void GW::PartyMgr::FlagAll(GW::GamePos pos) {
	static GW::Packet::CtoS::P020 pak;
	pak.pos = pos;
	CtoS::SendPacket(&pak);
}

void GW::PartyMgr::UnflagHero(DWORD hero_index) {
	FlagHero(hero_index, GW::GamePos(HUGE_VALF, HUGE_VALF, 0));
}

void GW::PartyMgr::UnflagAll() {
	FlagAll(GW::GamePos(HUGE_VALF, HUGE_VALF, 0)); 
}
