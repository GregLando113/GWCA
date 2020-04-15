#include "stdafx.h"

#include <GWCA/Utilities/Macros.h>
#include <GWCA/Constants/Constants.h>

#include <GWCA/GameContainers/GamePos.h>

#include <GWCA/GameEntities/Agent.h>
#include <GWCA/GameEntities/Skill.h>

#include <GWCA/Context/GameContext.h>
#include <GWCA/Context/WorldContext.h>

#include <GWCA/Managers/MemoryMgr.h>

namespace GW {

    uint32_t SkillbarSkill::GetRecharge() const {
        if (recharge == 0) return 0;
        return recharge - MemoryMgr::GetSkillTimer();
    }

    const SkillbarSkill *Skillbar::GetSkillById(Constants::SkillID skill_id) const {
        for (size_t i = 0; i < _countof(skills); i++) {
            if (skills[i].skill_id == static_cast<uint32_t>(skill_id))
                return &skills[i];
        }
        return NULL;
    }

    uint32_t Effect::GetTimeElapsed() const {
        return MemoryMgr::GetSkillTimer() - timestamp;
    }

    uint32_t Effect::GetTimeRemaining() const {
        return (uint32_t)(duration * 1000) - GetTimeElapsed();
    }

}
