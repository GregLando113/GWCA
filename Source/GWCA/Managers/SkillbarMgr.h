#pragma once

namespace GW {

    namespace Constants {
        enum class SkillID;
    }

    struct Skill;
    struct Skillbar;

    typedef Array<Skillbar> SkillbarArray;

    struct Module;
    extern Module SkillbarModule;

    namespace SkillbarMgr {

        // Get the skill slot in the player bar of the player.
        // Returns -1 if the skill is not there
        GWCA_API int GetSkillSlot(Constants::SkillID skill_id);

        // Use Skill in slot (Slot) on (Agent), optionally call that you are using said skill.
        GWCA_API void UseSkill(uint32_t slot, uint32_t target = 0, uint32_t call_target = 0);

        // Send raw packet to use skill with ID (SkillID). 
        // Same as above except the skillbar client struct will not be registered as casting.
        GWCA_API void UseSkillByID(uint32_t skill_id, uint32_t target = 0, uint32_t call_target = 0);

        // Get skill structure of said id, houses pretty much everything you would want to know about the skill.
        GWCA_API Skill GetSkillConstantData(uint32_t skill_id);

        GWCA_API void ChangeSecondary(uint32_t profession, int hero_index = 0);

        // Get array of skillbars, [0] = player [1-7] = heroes.
        GWCA_API SkillbarArray GetSkillbarArray();
        GWCA_API Skillbar *GetPlayerSkillbar();

        // @Remark:
        // `skill_ids` must contains at least 8 elements
        GWCA_API void LoadSkillbar(uint32_t *skill_ids, int hero_index = 0);

        GWCA_API bool LoadSkillTemplate(const char *temp);
        GWCA_API bool LoadSkillTemplate(const char *temp, int hero_index);

        GWCA_API void SetAttributes(uint32_t attribute_count,
            uint32_t *attribute_ids, uint32_t *attribute_values, int hero_index = 0);
    };
}
