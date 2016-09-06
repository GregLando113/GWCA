#include <GWCA\Managers\AgentMgr.h>

#include <GWCA\Managers\MemoryMgr.h>
#include <GWCA\Managers\GameThreadMgr.h>
#include <GWCA\Managers\CtoSMgr.h>
#include <GWCA\Managers\MapMgr.h>

#include <GWCA\Context\WorldContext.h>
#include <GWCA\Context\GameContext.h>
#include <GWCA\Context\PartyContext.h>

BYTE* GW::AgentMgr::dialog_log_ret_ = NULL;
DWORD GW::AgentMgr::last_dialog_id_ = 0;

GW::AgentMgr::AgentMgr() : GWCAManager() {
	change_target_ = (ChangeTarget_t)MemoryMgr::ChangeTargetFunction;
	move_ = (Move_t)MemoryMgr::MoveFunction;
	dialog_log_ret_ = (BYTE*)hk_dialog_log_.Detour(MemoryMgr::DialogFunc, (BYTE*)AgentMgr::detourDialogLog, 9);
}

void GW::AgentMgr::RestoreHooks() {
	hk_dialog_log_.Retour();
}

GW::AgentArray GW::AgentMgr::GetAgentArray() {
	return *(GW::AgentArray*)MemoryMgr::agArrayPtr;
}

float GW::AgentMgr::GetDistance(Vector2f a, Vector2f b) {
	return sqrtf((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

float GW::AgentMgr::GetSqrDistance(Vector2f a, Vector2f b) {
	return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}

void GW::AgentMgr::ChangeTarget(GW::Agent* Agent) {
	GameThreadMgr::Instance().Enqueue(change_target_, Agent->Id, 0);
}

void GW::AgentMgr::Move(float X, float Y, DWORD ZPlane /*= 0*/) {
	static GW::GamePos* buf = new GW::GamePos();

	buf->x = X;
	buf->y = Y;
	buf->zplane = ZPlane;

	GameThreadMgr::Instance().Enqueue(move_, buf);
}

void GW::AgentMgr::Move(const GW::GamePos& pos) {
	static GW::GamePos* buf = new GW::GamePos();

	*buf = pos;

	GameThreadMgr::Instance().Enqueue(move_, buf);
}

void GW::AgentMgr::Dialog(DWORD id) {
	CtoSMgr::Instance().SendPacket(0x8, 0x35, id);
}

GW::MapAgentArray GW::AgentMgr::GetMapAgentArray() {
	return GameContext::instance()->world->mapagents;
}

GW::Agent* GW::AgentMgr::GetPlayer() {
	GW::AgentArray agents = GetAgentArray();
	int id = GetPlayerId();
	if (agents.valid() && id > 0) {
		return agents[id];
	} else {
		return nullptr;
	}
}

GW::Agent* GW::AgentMgr::GetTarget() {
	GW::AgentArray agents = GetAgentArray();
	int id = GetTargetId();
	if (agents.valid() && id > 0) {
		return agents[id];
	} else {
		return nullptr;
	}
}

void GW::AgentMgr::GoNPC(GW::Agent* Agent, DWORD CallTarget /*= 0*/) {
	CtoSMgr::Instance().SendPacket(0xC, 0x33, Agent->Id, CallTarget);
}

void GW::AgentMgr::GoPlayer(GW::Agent* Agent) {
	CtoSMgr::Instance().SendPacket(0x8, 0x2D, Agent->Id);
}

void GW::AgentMgr::GoSignpost(GW::Agent* Agent, BOOL CallTarget /*= 0*/) {
	CtoSMgr::Instance().SendPacket(0xC, 0x4B, Agent->Id, CallTarget);
}

void GW::AgentMgr::CallTarget(GW::Agent* Agent) {
	CtoSMgr::Instance().SendPacket(0xC, 0x1C, 0xA, Agent->Id);
}

void __declspec(naked) GW::AgentMgr::detourDialogLog() {
	_asm MOV AgentMgr::last_dialog_id_, ESI
	_asm JMP AgentMgr::dialog_log_ret_
}

DWORD GW::AgentMgr::GetAmountOfPlayersInInstance() {
	// -1 because the 1st array element is nil
	return GameContext::instance()->world->players.size() - 1;
}

wchar_t* GW::AgentMgr::GetPlayerNameByLoginNumber(DWORD loginnumber) {
	return GameContext::instance()->world->players[loginnumber].Name;
}

DWORD GW::AgentMgr::GetAgentIdByLoginNumber(DWORD loginnumber) {
	return GameContext::instance()->world->players[loginnumber].AgentID;
}

GW::AgentID GW::AgentMgr::GetHeroAgentID(DWORD heroindex) {
	if (heroindex == 0) return GetPlayerId();

	auto ctx = GameContext::instance();
	auto heroarray = ctx->party->partyinfo->heroes;

	if (!heroarray.valid() || (DWORD)heroindex > heroarray.size()) return 0;

	return heroarray[--heroindex].agentid;
}

GW::PlayerArray GW::AgentMgr::GetPlayerArray() {
	return GameContext::instance()->world->players;
}

GW::NPCArray GW::AgentMgr::GetNPCArray() {
	return GameContext::instance()->world->npcs;
}
