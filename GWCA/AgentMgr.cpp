#include "AgentMgr.h"

#include "GameThreadMgr.h"
#include "CtoSMgr.h"
#include "MapMgr.h"

BYTE* GWCA::AgentMgr::dialog_log_ret_ = NULL;
DWORD GWCA::AgentMgr::last_dialog_id_ = 0;


GWCA::GW::AgentArray GWCA::AgentMgr::GetAgentArray() {
	return *(GW::AgentArray*)MemoryMgr::agArrayPtr;
}

std::vector<GWCA::GW::Agent*> * GWCA::AgentMgr::GetParty() {
	std::vector<GW::Agent*>* party = new std::vector<GW::Agent*>(GetPartySize());
	GW::AgentArray agents = GetAgentArray();

	for (size_t i = 0; i < agents.size(); ++i) {
		if (agents[i]->Allegiance == 1
			&& (agents[i]->TypeMap & 0x20000)) {

			party->push_back(agents[i]);
		}
	}

	return party;
}

size_t GWCA::AgentMgr::GetPartySize() {
	size_t ret = 0;
	size_t* retptr = NULL;
	for (BYTE i = 0; i < 3; ++i) {
		retptr = MemoryMgr::ReadPtrChain<size_t*>(MemoryMgr::GetContextPtr(), 3, 0x4C, 0x54, 0x0C + 0x10 * i);
		if (retptr == NULL)
			return NULL;
		else
			ret += *retptr;
	}
	return ret;
}

DWORD GWCA::AgentMgr::GetDistance(GW::Agent* a, GW::Agent* b) {
	return (DWORD)sqrtl((DWORD)(a->X - b->X) * (DWORD)(a->X - b->X) + (DWORD)(a->Y - b->Y) * (DWORD)(a->Y - b->Y));
}

DWORD GWCA::AgentMgr::GetSqrDistance(GW::Agent* a, GW::Agent* b) {
	return (DWORD)(a->X - b->X) * (DWORD)(a->X - b->X) + (DWORD)(a->Y - b->Y) * (DWORD)(a->Y - b->Y);
}

GWCA::AgentMgr::AgentMgr() : GWCAManager() {
	change_target_ = (ChangeTarget_t)MemoryMgr::ChangeTargetFunction;
	move_ = (Move_t)MemoryMgr::MoveFunction;
	dialog_log_ret_ = (BYTE*)hk_dialog_log_.Detour(MemoryMgr::DialogFunc, (BYTE*)AgentMgr::detourDialogLog, 9);
}

void GWCA::AgentMgr::RestoreHooks() {
	hk_dialog_log_.Retour();
}

void GWCA::AgentMgr::ChangeTarget(GW::Agent* Agent) {
	GameThreadMgr::Instance().Enqueue(change_target_, Agent->Id,0);
}

void GWCA::AgentMgr::Move(float X, float Y, DWORD ZPlane /*= 0*/) {
	static GW::GamePos* buf = new GW::GamePos();

	buf->x = X;
	buf->y = Y;
	buf->zplane = ZPlane;

	GameThreadMgr::Instance().Enqueue(move_, buf);
}

void GWCA::AgentMgr::Move(const GW::GamePos& pos) {
	static GW::GamePos* buf = new GW::GamePos();

	*buf = pos;

	GameThreadMgr::Instance().Enqueue(move_, buf);
}

void GWCA::AgentMgr::Dialog(DWORD id) {
	CtoSMgr::Instance().SendPacket(0x8, 0x35, id);
}

GWCA::GW::PartyMemberArray GWCA::AgentMgr::GetPartyMemberArray() {
	return *MemoryMgr::ReadPtrChain<GW::PartyMemberArray*>(MemoryMgr::GetContextPtr(), 3, 0x4C, 0x54, 0x4);
}

bool GWCA::AgentMgr::GetIsPartyLoaded() {
	if (MapMgr::Instance().GetInstanceType() == GwConstants::InstanceType::Loading) return false;

	GW::PartyMemberArray party = GetPartyMemberArray();
	if (!party.valid()) return false;

	for (DWORD i = 0; i < party.size(); i++){
		if ((party[i].state & 1) == 0) return false;
	}

	return true;
}

GWCA::GW::MapAgentArray GWCA::AgentMgr::GetMapAgentArray() {
	return *MemoryMgr::ReadPtrChain<GW::MapAgentArray*>(MemoryMgr::GetContextPtr(), 2, 0x2C, 0x7C);
}

GWCA::GW::Agent* GWCA::AgentMgr::GetPlayer() {
	GW::AgentArray agents = GetAgentArray();
	int id = GetPlayerId();
	if (agents.valid() && id > 0) {
		return agents[id];
	} else {
		return nullptr;
	}
}

GWCA::GW::Agent* GWCA::AgentMgr::GetTarget() {
	GW::AgentArray agents = GetAgentArray();
	int id = GetTargetId();
	if (agents.valid() && id > 0) {
		return agents[id];
	} else {
		return nullptr;
	}
}

void GWCA::AgentMgr::GoNPC(GW::Agent* Agent, DWORD CallTarget /*= 0*/) {
	CtoSMgr::Instance().SendPacket(0xC, 0x33, Agent->Id, CallTarget);
}

void GWCA::AgentMgr::GoPlayer(GW::Agent* Agent) {
	CtoSMgr::Instance().SendPacket(0x8, 0x2D, Agent->Id);
}

void GWCA::AgentMgr::GoSignpost(GW::Agent* Agent, BOOL CallTarget /*= 0*/) {
	CtoSMgr::Instance().SendPacket(0xC, 0x4B, Agent->Id, CallTarget);
}

void GWCA::AgentMgr::CallTarget(GW::Agent* Agent) {
	CtoSMgr::Instance().SendPacket(0xC, 0x1C, 0xA, Agent->Id);
}

void __declspec(naked) GWCA::AgentMgr::detourDialogLog() {
	_asm MOV AgentMgr::last_dialog_id_, ESI
	_asm JMP AgentMgr::dialog_log_ret_
}

DWORD GWCA::AgentMgr::GetAmountOfPlayersInInstance() {
	// -1 because the 1st array element is nil
	return MemoryMgr::ReadPtrChain<DWORD>(MemoryMgr::GetContextPtr(), 3, 0x2C, 0x814, 0) - 1;
}

wchar_t* GWCA::AgentMgr::GetPlayerNameByLoginNumber(DWORD loginnumber) {
	return MemoryMgr::ReadPtrChain<wchar_t*>(MemoryMgr::GetContextPtr(), 4, 0x2C, 0x80C, 0x28 + 0x4C * loginnumber, 0);
}

DWORD GWCA::AgentMgr::GetAgentIdByLoginNumber(DWORD loginnumber) {
	return MemoryMgr::ReadPtrChain<DWORD>(MemoryMgr::GetContextPtr(), 4, 0x2C, 0x80C, 0x4C * loginnumber, 0);
}

bool GWCA::AgentMgr::GetPartyTicked() {
	GW::PartyMemberArray party = GetPartyMemberArray();
	if (party.valid()) {
		for (size_t i = 0; i < party.size(); ++i) {
			if ((party[i].state & 2) == 0) {
				return false;
			}
		}
		return true;
	} else {
		return false;
	}
}

bool GWCA::AgentMgr::GetTicked(DWORD index) {
	GW::PartyMemberArray party = GetPartyMemberArray();
	if (party.valid()) {
		return (party[index].state & 2) != 0;
	} else {
		return false;
	}
}

bool GWCA::AgentMgr::GetTicked() {
	GW::PartyMemberArray party = GetPartyMemberArray();
	GW::Agent* me = GetPlayer();
	if (party.valid() && me) {
		for (DWORD i = 0; i < party.size();i++){
			if (party[i].loginnumber == me->LoginNumber){
				return (party[i].state & 2) != 0;
			}
		}
		return false;
	} else {
		return false;
	}
}

void GWCA::AgentMgr::Tick(bool flag) {
	CtoSMgr::Instance().SendPacket(0x8, 0xA9, flag);
}

const char* GWCA::AgentMgr::GetProfessionAcronym(GwConstants::Profession profession) {
	switch (profession) {
	case GwConstants::Profession::Warrior: return "W";
	case GwConstants::Profession::Ranger: return "R";
	case GwConstants::Profession::Monk: return "Mo";
	case GwConstants::Profession::Necromancer: return "N";
	case GwConstants::Profession::Mesmer: return "Me";
	case GwConstants::Profession::Elementalist: return "E";
	case GwConstants::Profession::Assassin: return "A";
	case GwConstants::Profession::Ritualist: return "Rt";
	case GwConstants::Profession::Paragon: return "P";
	case GwConstants::Profession::Dervish: return "D";
	default: return "";
	}
}

GWCA::GW::PlayerArray GWCA::AgentMgr::GetPlayerArray() {
	return *MemoryMgr::ReadPtrChain<GW::PlayerArray*>(MemoryMgr::GetContextPtr(), 2, 0x2C, 0x80C);
}
