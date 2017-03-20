#include <GWCA\Managers\EffectMgr.h>

#include <GWCA\Utilities\Hooker.h>

#include <GWCA\Context\GameContext.h>
#include <GWCA\Context\WorldContext.h>
#include <GWCA\Managers\GameThreadMgr.h>
#include <GWCA\Managers\CtoSMgr.h>

namespace {
	DWORD alcohol_level = 0;
	// post processing effects hook
	typedef void(__fastcall *PPEFunc_t)(DWORD Intensity, DWORD Tint);
	GW::THook<PPEFunc_t> ppe_hook;
	void __fastcall ppe_detour(DWORD Intensity, DWORD Tint) {
		alcohol_level = Intensity;
		ppe_hook.Original()(Intensity, Tint);
	}
}

void GW::Effects::SetupPostProcessingEffectHook() {
	if (ppe_hook.Empty()) {
		PPEFunc_t ppe_original = (PPEFunc_t)Scanner::Find("\x55\x8B\xEC\x83\xEC\x10\x89\x4D\xF8\xC7\x45\xFC", "xxxxxxxxxxxx", 0);
		printf("PostProcessEffectFunction = %X\n", (DWORD)ppe_original);
		ppe_hook.Detour(ppe_original, ppe_detour, 6);
	}	
}

void GW::Effects::RestoreHooks() {
	ppe_hook.Retour();
}

DWORD GW::Effects::GetAlcoholLevel() {
	return alcohol_level;
}

void GW::Effects::GetDrunkAf(DWORD Intensity, DWORD Tint) {
	ppe_hook.Original()(Intensity, Tint);
}

GW::Effect GW::Effects::GetPlayerEffectById(GW::Constants::SkillID SkillID) {
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

GW::Buff GW::Effects::GetPlayerBuffBySkillId(GW::Constants::SkillID SkillID) {
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

GW::EffectArray GW::Effects::GetPlayerEffectArray() {
	GW::AgentEffectsArray ageffects = GetPartyEffectArray();
	if (ageffects.valid()) {
		return ageffects[0].Effects;
	} else {
		return GW::EffectArray();
	}
}

GW::AgentEffectsArray GW::Effects::GetPartyEffectArray() {
	return GameContext::instance()->world->partyeffects;
}

GW::BuffArray GW::Effects::GetPlayerBuffArray() {
	GW::AgentEffectsArray ageffects = GetPartyEffectArray();
	if (ageffects.valid()) {
		return ageffects[0].Buffs;
	} else {
		return GW::BuffArray();
	}
}

void GW::Effects::DropBuff(DWORD buffId) {
	CtoS::SendPacket(0x8, 0x23, buffId);
}
