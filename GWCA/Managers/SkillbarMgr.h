#pragma once

#include <Windows.h>

#include <GWCA\Constants\Skills.h>
#include <GWCA\GameEntities\Skill.h>

namespace GW {

	namespace SkillbarMgr {

		// Get the skill slot in the player bar of the player.
		// Returns -1 if the skill is not there
		int GetSkillSlot(Constants::SkillID SkillID);

		// Use Skill in slot (Slot) on (Agent), optionally call that you are using said skill.
		void UseSkill(DWORD Slot, DWORD Target = 0, DWORD CallTarget = 0);

		// Send raw packet to use skill with ID (SkillID). 
		// Same as above except the skillbar client struct will not be registered as casting.
		void UseSkillByID(DWORD SkillID, DWORD Target = 0, DWORD CallTarget = 0);

		// Get skill structure of said id, houses pretty much everything you would want to know about the skill.
		GW::Skill GetSkillConstantData(DWORD SkillID);

		void ChangeSecondary(DWORD profession, int hero_index = 0);

		void LoadSkillbar(DWORD skillids[8], int hero_index = 0);

		bool LoadSkillTemplate(const char *temp);
		bool LoadSkillTemplate(const char *temp, int hero_index);

		void SetAttributes(DWORD attributecount, DWORD* attributeids, DWORD* attributevalues, int hero_index = 0);
	};
}
