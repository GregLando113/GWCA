#include "..\..\Managers\SkillbarMgr.h"

#include "..\..\Managers\GameThreadMgr.h"
#include "..\..\Managers\CtoSMgr.h"
#include "..\..\Managers\AgentMgr.h"
#include "..\..\Managers\MemoryMgr.h"

GWCA::GW::Skill GWCA::SkillbarMgr::GetSkillConstantData(DWORD SkillID) {
	return SkillConstants_[SkillID];
}

void GWCA::SkillbarMgr::ChangeSecondary(DWORD profession, int heroindex) {
	GW::AgentID id = AgentMgr::Instance().GetHeroAgentID(heroindex);
	CtoSMgr::Instance().SendPacket(0xC, 0x3B, id, profession);
}

void GWCA::SkillbarMgr::LoadSkillbar(DWORD * skillids, int heroindex) {
	GW::AgentID id = AgentMgr::Instance().GetHeroAgentID(heroindex);
	CtoSMgr::Instance().SendPacket(0x2C, 0x56, id, 0x8, skillids[0], skillids[1], skillids[2], skillids[3], skillids[4], skillids[5], skillids[6], skillids[7]);
}

void GWCA::SkillbarMgr::SetAttributes(DWORD attributecount, DWORD * attributeids, DWORD * attributevalues, int heroindex) {

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

void GWCA::SkillbarMgr::UseSkill(DWORD Slot, DWORD Target /*= 0*/, DWORD CallTarget /*= 0*/) {
	GameThreadMgr::Instance().Enqueue(UseSkill_, AgentMgr::Instance().GetPlayerId(), Slot, Target, CallTarget);
}

GWCA::SkillbarMgr::SkillbarMgr() {
	SkillConstants_ = (GW::Skill*)MemoryMgr::SkillArray;
	UseSkill_ = (UseSkill_t)MemoryMgr::UseSkillFunction;
}

GWCA::GW::Skillbar GWCA::SkillbarMgr::GetPlayerSkillbar() {
	GW::SkillbarArray sb = GetSkillbarArray();
	if (sb.valid()) {
		return sb[0];
	} else {
		return GW::Skillbar::Nil();
	}
}

GWCA::GW::SkillbarArray GWCA::SkillbarMgr::GetSkillbarArray() {
	return *MemoryMgr::ReadPtrChain<GW::SkillbarArray*>(MemoryMgr::GetContextPtr(), 2, 0x2C, 0x6F0);
}

void GWCA::SkillbarMgr::UseSkillByID(DWORD SkillID, DWORD Target /*= 0*/, DWORD CallTarget /*= 0*/) {
	CtoSMgr::Instance().SendPacket(0x14, 0x40, SkillID, 0, Target, CallTarget);
}

int GWCA::SkillbarMgr::GetSkillSlot(GwConstants::SkillID SkillID) {
	DWORD id = static_cast<DWORD>(SkillID);
	GW::Skillbar bar = GetPlayerSkillbar();
	for (int i = 0; i < 8; ++i) {
		if (bar.Skills[i].SkillId == id) {
			return i;
		}
	}
	return -1;
}
