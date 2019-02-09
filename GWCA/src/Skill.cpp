#include "stdafx.h"

#include <GWCA/Constants/Constants.h>

#include <GWCA/GameEntities/Agent.h>
#include <GWCA/GameEntities/Skill.h>

#include <GWCA/Context/GameContext.h>
#include <GWCA/Context/WorldContext.h>

#include <GWCA/Managers/MemoryMgr.h>

uint32_t GW::SkillbarSkill::GetRecharge() const {
    if (recharge == 0) return 0;
    return recharge - MemoryMgr::GetSkillTimer();
}

GW::Skillbar GW::Skillbar::GetPlayerSkillbar() {
    GW::SkillbarArray sb = SkillbarArray::GetSkillbarArray();
    if (sb.valid()) {
        return sb[0];
    } else {
        return GW::Skillbar();
    }
}

GW::Maybe<GW::SkillbarSkill> GW::Skillbar::GetSkillById(GW::Constants::SkillID skillId) {
    for (auto skill : skills) {
        if (skill.skill_id == static_cast<uint32_t>(skillId))
            return Just<SkillbarSkill>(skill);
    }

    return Nothing<SkillbarSkill>();
}

GW::SkillbarArray GW::SkillbarArray::GetSkillbarArray() {
    return GameContext::instance()->world->skillbar;
}

uint32_t GW::Effect::GetTimeElapsed() const {
    return MemoryMgr::GetSkillTimer() - timestamp;
}

uint32_t GW::Effect::GetTimeRemaining() const {
    return (uint32_t)(duration * 1000) - GetTimeElapsed();
}
