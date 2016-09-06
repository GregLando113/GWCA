#include "SkillbarMgr.h"

#include "GameThreadMgr.h"
#include "CtoSMgr.h"
#include "AgentMgr.h"

GWCA::GW::Skill GWCA::SkillbarMgr::GetSkillConstantData(DWORD SkillID) {
	return SkillConstants_[SkillID];
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
