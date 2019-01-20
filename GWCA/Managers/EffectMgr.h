#pragma once

namespace GW {

    namespace Effects {
        GWCA_API void RestoreHooks();

        // will save the alcohol level when it changes
        GWCA_API void SetupPostProcessingEffectHook();

        // Returns current level of intoxication, 0-5 scale.
        // If > 0 then skills that benefit from drunk will work.
        // Important: requires SetupPostProcessingEffectHook() above.
        GWCA_API DWORD GetAlcoholLevel();

        // Have fun with this ;))))))))))
        GWCA_API void GetDrunkAf(DWORD Intensity, DWORD Tint);

        // Get full array of effects and buffs for player and heroes.
        GWCA_API GW::AgentEffectsArray GetPartyEffectArray();

        // Get array of effects on the player.
        GWCA_API GW::EffectArray GetPlayerEffectArray();

        // Get array of buffs on the player.
        GWCA_API GW::BuffArray GetPlayerBuffArray();

        // Drop buffid buff.
        GWCA_API void DropBuff(DWORD buffId);

        // Gets effect struct of effect on player with SkillID, returns Effect::Nil() if no match.
        GWCA_API GW::Effect GetPlayerEffectById(Constants::SkillID SkillID);

        // Gets Buff struct of Buff on player with SkillID, returns Buff::Nil() if no match.
        GWCA_API GW::Buff GetPlayerBuffBySkillId(Constants::SkillID SkillID);

        GWCA_API void CreateEffect(DWORD effect_id, DWORD skill_id, float duration);
        GWCA_API void ReapplieEffect(DWORD effect_id, float duration);
        GWCA_API void RemoveEffect(DWORD effect_id);
    };
}
