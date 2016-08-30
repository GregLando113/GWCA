#include "..\..\Managers\AgentMgr.h"

#include "..\..\Managers\MemoryMgr.h"
#include "..\..\Managers\GameThreadMgr.h"
#include "..\..\Managers\CtoSMgr.h"
#include "..\..\Managers\MapMgr.h"

#include "..\..\Structures\Context\GameContext.h"
#include "..\..\Structures\Context\PartyContext.h"

BYTE* GWCA::AgentMgr::dialog_log_ret_ = NULL;
DWORD GWCA::AgentMgr::last_dialog_id_ = 0;

GWCA::AgentMgr::AgentMgr() : GWCAManager() {
	change_target_ = (ChangeTarget_t)MemoryMgr::ChangeTargetFunction;
	move_ = (Move_t)MemoryMgr::MoveFunction;
	dialog_log_ret_ = (BYTE*)hk_dialog_log_.Detour(MemoryMgr::DialogFunc, (BYTE*)AgentMgr::detourDialogLog, 9);
}

void GWCA::AgentMgr::RestoreHooks() {
	hk_dialog_log_.Retour();
}

GWCA::GW::AgentArray GWCA::AgentMgr::GetAgentArray() {
	return *(GW::AgentArray*)MemoryMgr::agArrayPtr;
}

float GWCA::AgentMgr::GetDistance(const GW::GamePos& a, const GW::GamePos& b) {
	return sqrtf((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

float GWCA::AgentMgr::GetSqrDistance(const GW::GamePos& a,const GW::GamePos& b) {
	return (a.x- b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
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

GWCA::GW::AgentID GWCA::AgentMgr::GetHeroAgentID(int heroindex) {
	if (heroindex == 0) return GetPlayerId();

	auto ctx = GameContext::instance();
	auto heroarray = ctx->party->partyinfo->heroes;

	if (!heroarray.valid() || (DWORD)heroindex > heroarray.size()) return 0;

	return heroarray[--heroindex].agentid;
}

GWCA::GW::PlayerArray GWCA::AgentMgr::GetPlayerArray() {
	return *MemoryMgr::ReadPtrChain<GW::PlayerArray*>(MemoryMgr::GetContextPtr(), 2, 0x2C, 0x80C);
}

GWCA::GW::NPCArray GWCA::AgentMgr::GetNPCArray() {
	return *MemoryMgr::ReadPtrChain<GW::NPCArray*>(MemoryMgr::GetContextPtr(), 2, 0x2C, 0x7FC);
}
