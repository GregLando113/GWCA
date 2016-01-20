#include "PartyMgr.h"

#include "AgentMgr.h"
#include "CtoSMgr.h"

GWCA::PartyMgr::PartyMgr() {
	BYTE* addr_tick = (BYTE*)0x0054E6B0;
	DWORD tick_length = GWCA::Hook::CalculateDetourLength(addr_tick);
	ori_tick_ = (Tick_t)hk_tick_.Detour(addr_tick, (BYTE*)DetourTick, tick_length);
}

void GWCA::PartyMgr::RestoreHooks() {
	hk_tick_.Retour();
}

bool GWCA::PartyMgr::GetIsPartyTicked() {
	GW::PlayerPartyMemberArray& party = GetPlayerPartyArray();
	if (!party.valid()) return false;
	for (GW::PlayerPartyMember member : party) {
		if (!member.ticked()) return false;
	}
	return true;
}

bool GWCA::PartyMgr::GetIsPartyLoaded() {
	GW::PlayerPartyMemberArray& party = GetPlayerPartyArray();
	if (!party.valid()) return false;
	for (GW::PlayerPartyMember member : party) {
		if (!member.connected()) return false;
	}
	return true;
}

bool GWCA::PartyMgr::GetIsTicked(DWORD player_index) {
	GW::PlayerPartyMemberArray& party = GetPlayerPartyArray();
	if (party.valid()) {
		return (party[player_index].ticked());
	} else {
		return false;
	}
}

bool GWCA::PartyMgr::GetIsPlayerTicked() {
	GW::PlayerPartyMemberArray& party = GetPlayerPartyArray();
	if (!party.valid()) return false;
	GW::Agent* me = AgentMgr::Instance().GetPlayer();
	if (me == nullptr) return false;
	for (DWORD i = 0; i < party.size(); i++) {
		if (party[i].loginnumber == me->LoginNumber) {
			return party[i].ticked();
		}
	}
	return false;
}

void GWCA::PartyMgr::Tick(bool flag) {
	CtoSMgr::Instance().SendPacket(0x8, 0xA9, flag);
}

DWORD __stdcall GWCA::PartyMgr::DetourTick(DWORD unk1) {
	// this func is always called twice so use this hack to tick only once
	static bool toggle = true;
	toggle = !toggle;
	if (toggle) return 4;

	PartyMgr::Instance().Tick(!PartyMgr::Instance().GetIsPlayerTicked());
	return 4;
}

