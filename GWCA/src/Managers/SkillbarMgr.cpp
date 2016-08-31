#include <GWCA\Managers\SkillbarMgr.h>

#include <GWCA\Context\GameContext.h>
#include <GWCA\Context\WorldContext.h>

#include <GWCA\Managers\GameThreadMgr.h>
#include <GWCA\Managers\CtoSMgr.h>
#include <GWCA\Managers\AgentMgr.h>
#include <GWCA\Managers\MemoryMgr.h>

GW::Skill GW::SkillbarMgr::GetSkillConstantData(DWORD SkillID) {
	return SkillConstants_[SkillID];
}

void GW::SkillbarMgr::ChangeSecondary(DWORD profession, int heroindex) {
	GW::AgentID id = AgentMgr::Instance().GetHeroAgentID(heroindex);
	CtoSMgr::Instance().SendPacket(0xC, 0x3B, id, profession);
}

void GW::SkillbarMgr::LoadSkillbar(DWORD * skillids, int heroindex) {
	GW::AgentID id = AgentMgr::Instance().GetHeroAgentID(heroindex);
	CtoSMgr::Instance().SendPacket(0x2C, 0x56, id, 0x8, skillids[0], skillids[1], skillids[2], skillids[3], skillids[4], skillids[5], skillids[6], skillids[7]);
}

void GW::SkillbarMgr::SetAttributes(DWORD attributecount, DWORD * attributeids, DWORD * attributevalues, int heroindex) {

	struct tSetAttributes {
		DWORD header = 0x9;
		GW::AgentID id;
		DWORD attributecount1;
		DWORD attributeids[16];
		DWORD attributecount2;
		DWORD attributevalues[16];
	};

	static tSetAttributes* set_attributes_buffer = new tSetAttributes();

	GW::AgentID id = AgentMgr::Instance().GetHeroAgentID(heroindex);

	set_attributes_buffer->header = 0x9;

	set_attributes_buffer->id = id;

	set_attributes_buffer->attributecount1 = attributecount;
	set_attributes_buffer->attributecount2 = attributecount;

	for (DWORD i = 0; i < attributecount; ++i) {
		set_attributes_buffer->attributeids[i] = attributeids[i];
		set_attributes_buffer->attributevalues[i] = attributevalues[i];
	}

	CtoSMgr::Instance().SendPacket<tSetAttributes>(set_attributes_buffer);
}

void GW::SkillbarMgr::UseSkill(DWORD Slot, DWORD Target /*= 0*/, DWORD CallTarget /*= 0*/) {
	GameThreadMgr::Instance().Enqueue(UseSkill_, AgentMgr::Instance().GetPlayerId(), Slot, Target, CallTarget);
}

GW::SkillbarMgr::SkillbarMgr() {
	SkillConstants_ = (GW::Skill*)MemoryMgr::SkillArray;
	UseSkill_ = (UseSkill_t)MemoryMgr::UseSkillFunction;
}

void GW::SkillbarMgr::UseSkillByID(DWORD SkillID, DWORD Target /*= 0*/, DWORD CallTarget /*= 0*/) {
	CtoSMgr::Instance().SendPacket(0x14, 0x40, SkillID, 0, Target, CallTarget);
}

int GW::SkillbarMgr::GetSkillSlot(GW::Constants::SkillID SkillID) {
	DWORD id = static_cast<DWORD>(SkillID);
	GW::Skillbar bar = Skillbar::GetPlayerSkillbar();
	if (!bar.IsValid()) return -1;
	for (int i = 0; i < 8; ++i) {
		if (bar.Skills[i].SkillId == id) {
			return i;
		}
	}
	return -1;
}
