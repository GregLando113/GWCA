#pragma once

#include <GWCA/Utilities/Export.h>

namespace GW {

    struct Buff;
    struct Effect;
    struct AgentEffects;

    typedef Array<Buff> BuffArray;
    typedef Array<Effect> EffectArray;
    typedef Array<AgentEffects> AgentEffectsArray;

    namespace Constants {
        enum class SkillID;
    }

    struct Module;
    extern Module EffectModule;

    namespace Effects {
        // Returns current level of intoxication, 0-5 scale.
        // If > 0 then skills that benefit from drunk will work.
        // Important: requires SetupPostProcessingEffectHook() above.
        GWCA_API uint32_t GetAlcoholLevel();

        // Have fun with this ;))))))))))
        GWCA_API void GetDrunkAf(uint32_t intensity, uint32_t tint);

        // Get full array of effects and buffs for player and heroes.
        GWCA_API AgentEffectsArray GetPartyEffectArray();

        // Get array of effects on the player.
        GWCA_API EffectArray GetPlayerEffectArray();

        // Get array of buffs on the player.
        GWCA_API BuffArray GetPlayerBuffArray();

        // Drop buffid buff.
        GWCA_API void DropBuff(uint32_t buff_id);

        // Gets effect struct of effect on player with SkillID, returns Effect::Nil() if no match.
        GWCA_API Effect *GetPlayerEffectById(Constants::SkillID skill_id);

        // Gets Buff struct of Buff on player with SkillID, returns Buff::Nil() if no match.
        GWCA_API Buff *GetPlayerBuffBySkillId(Constants::SkillID skill_id);

        GWCA_API void CreateEffect(uint32_t effect_id, uint32_t skill_id, float duration);
        GWCA_API void ReapplieEffect(uint32_t effect_id, float duration);
        GWCA_API void RemoveEffect(uint32_t effect_id);
    };
}
