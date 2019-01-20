#pragma once

namespace GW {

    namespace SkillbarMgr {

        // Get the skill slot in the player bar of the player.
        // Returns -1 if the skill is not there
        GWCA_API int GetSkillSlot(Constants::SkillID SkillID);

        // Use Skill in slot (Slot) on (Agent), optionally call that you are using said skill.
        GWCA_API void UseSkill(DWORD Slot, DWORD Target = 0, DWORD CallTarget = 0);

        // Send raw packet to use skill with ID (SkillID). 
        // Same as above except the skillbar client struct will not be registered as casting.
        GWCA_API void UseSkillByID(DWORD SkillID, DWORD Target = 0, DWORD CallTarget = 0);

        // Get skill structure of said id, houses pretty much everything you would want to know about the skill.
        GWCA_API GW::Skill GetSkillConstantData(DWORD SkillID);

        GWCA_API void ChangeSecondary(DWORD profession, int hero_index = 0);

        GWCA_API void LoadSkillbar(DWORD skillids[8], int hero_index = 0);

        GWCA_API bool LoadSkillTemplate(const char *temp);
        GWCA_API bool LoadSkillTemplate(const char *temp, int hero_index);

        GWCA_API void SetAttributes(DWORD attributecount, DWORD* attributeids, DWORD* attributevalues, int hero_index = 0);
    };
}
