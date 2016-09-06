#include <GWCA\GameEntities\Skill.h>

#include <GWCA\Managers\MemoryMgr.h>
#include <GWCA\Context\GameContext.h>

long GW::SkillbarSkill::GetRecharge() const {
	if (Recharge == 0) return 0;
	return Recharge - MemoryMgr::GetSkillTimer();
}

GW::Skillbar GW::Skillbar::GetPlayerSkillbar() {
	GW::SkillbarArray sb = SkillbarArray::GetSkillbarArray();
	if (sb.valid()) {
		return sb[0];
	} else {
		return GW::Skillbar();
	}
}

GW::SkillbarArray GW::SkillbarArray::GetSkillbarArray() {
	return GameContext::instance()->world->skillbar;
}

long GW::Effect::GetTimeElapsed() const {
	return MemoryMgr::GetSkillTimer() - TimeStamp;
}

long GW::Effect::GetTimeRemaining() const {
	return (long)(Duration * 1000) - GetTimeElapsed();
}
