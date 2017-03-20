#include <GWCA\Managers\SkillbarMgr.h>

#include <GWCA\Context\GameContext.h>
#include <GWCA\Context\WorldContext.h>

#include <GWCA\Managers\CtoSMgr.h>
#include <GWCA\Managers\AgentMgr.h>
#include <GWCA\Managers\MemoryMgr.h>

namespace {
	typedef void(__fastcall *UseSkill_t)(DWORD, DWORD, DWORD, DWORD);
	UseSkill_t UseSkill;
}

GW::Skill GW::SkillbarMgr::GetSkillConstantData(DWORD SkillID) {
	static GW::Skill* skillconstants = nullptr;
	if (skillconstants == nullptr) {
		// Skill array.
		GW::Skill** SkillArray = (GW::Skill**)Scanner::Find("\x8D\x04\xB6\x5E\xC1\xE0\x05\x05", "xxxxxxxx", 0);
		printf("SkillArray = 0x%X\n", (DWORD)SkillArray);
		if (SkillArray) {
			skillconstants = *(SkillArray + 8);
		}
	}
	return skillconstants[SkillID];
}

void GW::SkillbarMgr::ChangeSecondary(DWORD profession, int heroindex) {
	GW::AgentID id = Agents::GetHeroAgentID(heroindex);
	CtoS::SendPacket(0xC, 0x3B, id, profession);
}

void GW::SkillbarMgr::LoadSkillbar(DWORD * skillids, int heroindex) {
	GW::AgentID id = Agents::GetHeroAgentID(heroindex);
	CtoS::SendPacket(0x2C, 0x56, id, 0x8, skillids[0], skillids[1], skillids[2], skillids[3], skillids[4], skillids[5], skillids[6], skillids[7]);
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

	GW::AgentID id = Agents::GetHeroAgentID(heroindex);

	set_attributes_buffer->header = 0x9;

	set_attributes_buffer->id = id;

	set_attributes_buffer->attributecount1 = attributecount;
	set_attributes_buffer->attributecount2 = attributecount;

	for (DWORD i = 0; i < attributecount; ++i) {
		set_attributes_buffer->attributeids[i] = attributeids[i];
		set_attributes_buffer->attributevalues[i] = attributevalues[i];
	}

	CtoS::SendPacket<tSetAttributes>(set_attributes_buffer);
}

void GW::SkillbarMgr::UseSkill(DWORD Slot, DWORD Target /*= 0*/, DWORD CallTarget /*= 0*/) {
	static UseSkill_t useskill_func = nullptr;
	if (useskill_func == nullptr) {
		useskill_func = (UseSkill_t)Scanner::Find("\x55\x8B\xEC\x83\xEC\x10\x53\x56\x8B\xD9\x57\x8B\xF2\x89\x5D\xF0", "xxxxxxxxxxxxxxxx", 0);
		printf("UseSkillFunction = 0x%X\n", (DWORD)useskill_func);
	}
	if (useskill_func) {
		useskill_func(Agents::GetPlayerId(), Slot, Target, CallTarget);
	}
}

void GW::SkillbarMgr::UseSkillByID(DWORD SkillID, DWORD Target /*= 0*/, DWORD CallTarget /*= 0*/) {
	CtoS::SendPacket(0x14, 0x40, SkillID, 0, Target, CallTarget);
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
