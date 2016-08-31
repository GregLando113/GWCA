#include <GWCA\Managers\EffectMgr.h>

#include <GWCA\Context\GameContext.h>
#include <GWCA\Context\WorldContext.h>
#include <GWCA\Managers\MemoryMgr.h>
#include <GWCA\Managers\GameThreadMgr.h>
#include <GWCA\Managers\CtoSMgr.h>

DWORD GW::EffectMgr::alcohol_level_ = NULL;
GW::EffectMgr::PPEFunc_t GW::EffectMgr::ppe_retour_func_ = NULL;


GW::EffectMgr::EffectMgr() {
	ppe_retour_func_ = (PPEFunc_t)hk_post_process_effect_.Detour(MemoryMgr::PostProcessEffectFunction, (BYTE*)AlcoholHandler, 6);
}

void GW::EffectMgr::RestoreHooks() {
	hk_post_process_effect_.Retour();
}

GW::Effect GW::EffectMgr::GetPlayerEffectById(GW::Constants::SkillID SkillID) {
	DWORD id = static_cast<DWORD>(SkillID);
	GW::AgentEffectsArray AgEffects = GetPartyEffectArray();

	if (AgEffects.valid()) {
		GW::EffectArray Effects = AgEffects[0].Effects;
		if (Effects.valid()) {
			for (DWORD i = 0; i < Effects.size(); i++) {
				if (Effects[i].SkillId == id) return Effects[i];
			}
		}
	}

	return GW::Effect::Nil();
}

GW::Buff GW::EffectMgr::GetPlayerBuffBySkillId(GW::Constants::SkillID SkillID) {
	DWORD id = static_cast<DWORD>(SkillID);
	GW::AgentEffectsArray AgEffects = GetPartyEffectArray();

	if (AgEffects.valid()) {
		GW::BuffArray Buffs = AgEffects[0].Buffs;
		if (Buffs.valid()) {
			for (DWORD i = 0; i < Buffs.size(); i++) {
				if (Buffs[i].SkillId == id) return Buffs[i];
			}
		}
	}

	return GW::Buff::Nil();
}

GW::EffectArray GW::EffectMgr::GetPlayerEffectArray() {
	GW::AgentEffectsArray ageffects = GetPartyEffectArray();
	if (ageffects.valid()) {
		return ageffects[0].Effects;
	} else {
		return GW::EffectArray();
	}
}

void __fastcall GW::EffectMgr::AlcoholHandler(DWORD Intensity, DWORD Tint) {
	alcohol_level_ = Intensity;
	return ppe_retour_func_(Intensity, Tint);
}

void GW::EffectMgr::GetDrunkAf(DWORD Intensity, DWORD Tint) {
	GameThreadMgr::Instance().Enqueue(ppe_retour_func_, Intensity, Tint);
}

GW::AgentEffectsArray GW::EffectMgr::GetPartyEffectArray() {
	return GameContext::instance()->world->partyeffects;
}

GW::BuffArray GW::EffectMgr::GetPlayerBuffArray() {
	GW::AgentEffectsArray ageffects = GetPartyEffectArray();
	if (ageffects.valid()) {
		return ageffects[0].Buffs;
	} else {
		return GW::BuffArray();
	}
}

void GW::EffectMgr::DropBuff(DWORD buffId) {
	CtoSMgr::Instance().SendPacket(0x8, 0x23, buffId);
}
