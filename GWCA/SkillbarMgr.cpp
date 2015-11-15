#include "SkillbarMgr.h"

#include "GWAPIMgr.h"


GWAPI::GW::Skill GWAPI::SkillbarMgr::GetSkillConstantData(DWORD SkillID)
{
	return SkillConstants_[SkillID];
}

void GWAPI::SkillbarMgr::UseSkill(DWORD Slot, DWORD Target /*= 0*/, DWORD CallTarget /*= 0*/)
{
	api().Gamethread().Enqueue(UseSkill_, api().Agents().GetPlayerId(), Slot, Target, CallTarget);
}

GWAPI::SkillbarMgr::SkillbarMgr(GWAPIMgr& api) : GWCAManager(api)
{
	SkillConstants_ = (GW::Skill*)MemoryMgr::SkillArray;
	UseSkill_ = (UseSkill_t)MemoryMgr::UseSkillFunction;
}

GWAPI::GW::Skillbar GWAPI::SkillbarMgr::GetPlayerSkillbar() 
{
	GW::SkillbarArray sb = GetSkillbarArray();
	if (sb.valid()) {
		return sb[0];
	} else {
		return GW::Skillbar::Nil();
	}
}

GWAPI::GW::SkillbarArray GWAPI::SkillbarMgr::GetSkillbarArray()
{
	return *MemoryMgr::ReadPtrChain<GW::SkillbarArray*>(MemoryMgr::GetContextPtr(), 2, 0x2C, 0x6F0);
}

void GWAPI::SkillbarMgr::UseSkillByID(DWORD SkillID, DWORD Target /*= 0*/, DWORD CallTarget /*= 0*/)
{
	api().CtoS().SendPacket(0x14, 0x40, SkillID, 0, Target, CallTarget);
}

int GWAPI::SkillbarMgr::GetSkillSlot(GwConstants::SkillID SkillID) {
	DWORD id = static_cast<DWORD>(SkillID);
	GW::Skillbar bar = GetPlayerSkillbar();
	for (int i = 0; i < 8; ++i) {
		if (bar.Skills[i].SkillId == id) {
			return i;
		}
	}
	return -1;
}
