#include "EffectMgr.h"

#include "GameThreadMgr.h"
#include "GameContext.h"
#include "CtoSMgr.h"

DWORD GWCA::EffectMgr::alcohol_level_ = NULL;
GWCA::EffectMgr::PPEFunc_t GWCA::EffectMgr::ppe_retour_func_ = NULL;


GWCA::EffectMgr::EffectMgr() {
	ppe_retour_func_ = (PPEFunc_t)hk_post_process_effect_.Detour(MemoryMgr::PostProcessEffectFunction, (BYTE*)AlcoholHandler, 6);
}

void GWCA::EffectMgr::RestoreHooks() {
	hk_post_process_effect_.Retour();
}

GWCA::GW::Effect GWCA::EffectMgr::GetPlayerEffectById(GwConstants::SkillID SkillID) {
	DWORD id = static_cast<DWORD>(SkillID);
	GW::AgentEffectsArray AgEffects = GetPartyEffectArray();

	if (AgEffects.valid()){
		GW::EffectArray Effects = AgEffects[0].Effects;
		if (Effects.valid()){
			for (DWORD i = 0; i < Effects.size(); i++) {
				if (Effects[i].SkillId == id) return Effects[i];
			}
		}
	}

	return GW::Effect::Nil();
}

GWCA::GW::Buff GWCA::EffectMgr::GetPlayerBuffBySkillId(GwConstants::SkillID SkillID) {
	DWORD id = static_cast<DWORD>(SkillID);
	GW::AgentEffectsArray AgEffects = GetPartyEffectArray();

	if (AgEffects.valid()){
		GW::BuffArray Buffs = AgEffects[0].Buffs;
		if (Buffs.valid()){
			for (DWORD i = 0; i < Buffs.size(); i++) {
				if (Buffs[i].SkillId == id) return Buffs[i];
			}
		}
	}

	return GW::Buff::Nil();
}

GWCA::GW::EffectArray GWCA::EffectMgr::GetPlayerEffectArray() {
	GW::AgentEffectsArray ageffects = GetPartyEffectArray();
	if (ageffects.valid()){
		return ageffects[0].Effects;
	} else {
		return GW::EffectArray();
	}
}

void __fastcall GWCA::EffectMgr::AlcoholHandler(DWORD Intensity, DWORD Tint) {
	alcohol_level_ = Intensity;
	return ppe_retour_func_(Intensity, Tint);
}

void GWCA::EffectMgr::GetDrunkAf(DWORD Intensity,DWORD Tint) {
	GameThreadMgr::Instance().Enqueue(ppe_retour_func_, Intensity, Tint);
}

GWCA::GW::AgentEffectsArray GWCA::EffectMgr::GetPartyEffectArray() {
	return GameContext::instance()->world->partyeffects;
}

GWCA::GW::BuffArray GWCA::EffectMgr::GetPlayerBuffArray() {
	GW::AgentEffectsArray ageffects = GetPartyEffectArray();
	if (ageffects.valid()){
		return ageffects[0].Buffs;
	} else {
		return GW::BuffArray();
	}
}

void GWCA::EffectMgr::DropBuff(DWORD buffId) {
	CtoSMgr::Instance().SendPacket(0x8, 0x23, buffId);
}

