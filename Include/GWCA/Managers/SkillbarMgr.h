#pragma once
#include <GWCA/Constants/Types.h>

#include <GWCA/Utilities/Hook.h>
#include <GWCA/Utilities/Export.h>

namespace GW {

    namespace Constants {
        enum class Attribute;
        enum class SkillID;
    }

    struct Skill;
    struct Skillbar;

    typedef Array<Skillbar> SkillbarArray;

    struct Module;
    extern Module SkillbarModule;

    namespace SkillbarMgr {
        struct SkillbarAttribute {
            Constants::Attribute attribute;
            uint32_t points;
        };

        struct SkillTemplate {
            Constants::Profession primary;
            Constants::Profession secondary;
            Constants::SkillID    skills[8];
            SkillbarAttribute      attributes[16];
        };

        // Get the skill slot in the player bar of the player.
        // Returns -1 if the skill is not there
        GWCA_API int GetSkillSlot(Constants::SkillID skill_id);

        // Use Skill in slot (Slot) on (Agent), optionally call that you are using said skill.
        GWCA_API bool UseSkill(uint32_t slot, AgentID target = AgentID::None, bool call_target = false);

        // Send raw packet to use skill with ID (SkillID). 
        // Same as above except the skillbar client struct will not be registered as casting.
        GWCA_API bool UseSkillByID(Constants::SkillID skill_id, AgentID target = AgentID::None, bool call_target = false);

        // Get skill structure of said id, houses pretty much everything you would want to know about the skill.
        GWCA_API Skill* GetSkillConstantData(Constants::SkillID skill_id);

        GWCA_API bool GetIsSkillUnlocked(Constants::SkillID skill_id);

        GWCA_API bool ChangeSecondary(Constants::Profession profession, uint32_t hero_index = 0);

        // Get array of skillbars, [0] = player [1-7] = heroes.
        GWCA_API SkillbarArray* GetSkillbarArray();
        GWCA_API Skillbar *GetPlayerSkillbar();
        GWCA_API Skill* GetHoveredSkill();

        GWCA_API bool DecodeSkillTemplate(SkillTemplate *result, const char *temp);
        GWCA_API bool EncodeSkillTemplate(const SkillTemplate& in, char* result, size_t result_len);

        // @Remark:
        // `skill_ids` must contains at least 8 elements
        GWCA_API bool LoadSkillbar(Constants::SkillID *skills, size_t n_skills, uint32_t hero_index = 0);

        GWCA_API bool LoadSkillTemplate(const char *temp);
        GWCA_API bool LoadSkillTemplate(const char *temp, uint32_t hero_index);

        GWCA_API void SetAttributes(uint32_t attribute_count,
            Constants::Attribute *attribute_ids, uint32_t *attribute_values, uint32_t hero_index = 0);
        GWCA_API void SetAttributes(SkillbarAttribute*attributes, size_t n_attributes, uint32_t hero_index = 0);

        typedef HookCallback<AgentID, uint32_t, AgentID, bool> UseSkillCallback;
        GWCA_API void RegisterUseSkillCallback(
            HookEntry* entry,
            UseSkillCallback callback);

        GWCA_API void RemoveUseSkillCallback(
            HookEntry* entry);
    }
}
