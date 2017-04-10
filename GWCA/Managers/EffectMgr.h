#pragma once

#include <Windows.h>

#include <GWCA\Constants\Skills.h>
#include <GWCA\GameEntities\Skill.h>

namespace GW {

	namespace Effects {
		void RestoreHooks();

		// will save the alcohol level when it changes
		void SetupPostProcessingEffectHook();

		// Returns current level of intoxication, 0-5 scale.
		// If > 0 then skills that benefit from drunk will work.
		// Important: requires SetupPostProcessingEffectHook() above.
		DWORD GetAlcoholLevel();

		// Have fun with this ;))))))))))
		void GetDrunkAf(DWORD Intensity, DWORD Tint);

		// Get full array of effects and buffs for player and heroes.
		GW::AgentEffectsArray GetPartyEffectArray();

		// Get array of effects on the player.
		GW::EffectArray GetPlayerEffectArray();

		// Get array of buffs on the player.
		GW::BuffArray GetPlayerBuffArray();

		// Drop buffid buff.
		void DropBuff(DWORD buffId);

		// Gets effect struct of effect on player with SkillID, returns Effect::Nil() if no match.
		GW::Effect GetPlayerEffectById(Constants::SkillID SkillID);

		// Gets Buff struct of Buff on player with SkillID, returns Buff::Nil() if no match.
		GW::Buff GetPlayerBuffBySkillId(Constants::SkillID SkillID);

		void CreateEffect(DWORD effect_id, DWORD skill_id, float duration);
		void ReapplieEffect(DWORD effect_id, float duration);
		void RemoveEffect(DWORD effect_id);
	};
}
