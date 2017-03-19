#include <GWCA\Managers\AgentMgr.h>

#include <GWCA\Managers\MemoryMgr.h>
#include <GWCA\Managers\GameThreadMgr.h>
#include <GWCA\Managers\CtoSMgr.h>
#include <GWCA\Managers\MapMgr.h>

#include <GWCA\Context\WorldContext.h>
#include <GWCA\Context\GameContext.h>
#include <GWCA\Context\PartyContext.h>

namespace {
	GW::Hook dialoglog_hook;
	BYTE* dialoglog_ret = nullptr;
	DWORD last_dialog_id = 0;
	void __declspec(naked) dialoglog_detour() {
		_asm MOV last_dialog_id, ESI
		_asm JMP dialoglog_ret
	}
}

void GW::Agents::SetupDialogHook() {
	dialoglog_ret = (BYTE*)dialoglog_hook.Detour(MemoryMgr::DialogFunc, (BYTE*)dialoglog_detour, 9);
}

DWORD GW::Agents::GetLastDialogId() { 
	return last_dialog_id; 
}

void GW::Agents::RestoreDialogHook() {
	dialoglog_hook.Retour();
}

GW::AgentArray GW::Agents::GetAgentArray() {
	return *(GW::AgentArray*)MemoryMgr::agArrayPtr;
}

float GW::Agents::GetDistance(Vector2f a, Vector2f b) {
	return sqrtf((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

float GW::Agents::GetSqrDistance(Vector2f a, Vector2f b) {
	return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}

void GW::Agents::ChangeTarget(GW::Agent* Agent) {
	typedef void(__fastcall *ChangeTarget_t)(DWORD AgentID, DWORD smth);
	static ChangeTarget_t changetarget_func = nullptr;
	if (changetarget_func == nullptr) {
		PatternScanner scan("Gw.exe");
		changetarget_func = (ChangeTarget_t)scan.FindPattern("\x33\xC0\x3B\xDA\x0F\x95\xC0\x33", "xxxxxxxx", -0x78);
	}
	changetarget_func(Agent->Id, 0);
}

void GW::Agents::Move(float X, float Y, DWORD ZPlane /*= 0*/) {
	GW::GamePos pos;
	pos.x = X;
	pos.y = Y;
	pos.zplane = ZPlane;
	Agents::Move(pos);
}

void GW::Agents::Move(const GW::GamePos& pos) {
	typedef void(__fastcall *Move_t)(const GW::GamePos* Pos);
	((Move_t)MemoryMgr::MoveFunction)(&pos);
}

void GW::Agents::Dialog(DWORD id) {
	CtoSMgr::Instance().SendPacket(0x8, 0x35, id);
}

GW::MapAgentArray GW::Agents::GetMapAgentArray() {
	return GameContext::instance()->world->mapagents;
}

GW::Agent* GW::Agents::GetAgentByID(DWORD id) {
	GW::AgentArray agents = GetAgentArray();
	if (agents.valid() && id > 0) {
		return agents[id];
	} else {
		return nullptr;
	}
}

void GW::Agents::GoNPC(GW::Agent* Agent, DWORD CallTarget /*= 0*/) {
	CtoSMgr::Instance().SendPacket(0xC, 0x33, Agent->Id, CallTarget);
}

void GW::Agents::GoPlayer(GW::Agent* Agent) {
	CtoSMgr::Instance().SendPacket(0x8, 0x2D, Agent->Id);
}

void GW::Agents::GoSignpost(GW::Agent* Agent, BOOL CallTarget /*= 0*/) {
	CtoSMgr::Instance().SendPacket(0xC, 0x4B, Agent->Id, CallTarget);
}

void GW::Agents::CallTarget(GW::Agent* Agent) {
	CtoSMgr::Instance().SendPacket(0xC, 0x1C, 0xA, Agent->Id);
}

DWORD GW::Agents::GetAmountOfPlayersInInstance() {
	// -1 because the 1st array element is nil
	return GameContext::instance()->world->players.size() - 1;
}

wchar_t* GW::Agents::GetPlayerNameByLoginNumber(DWORD loginnumber) {
	return GameContext::instance()->world->players[loginnumber].Name;
}

DWORD GW::Agents::GetAgentIdByLoginNumber(DWORD loginnumber) {
	return GameContext::instance()->world->players[loginnumber].AgentID;
}

GW::AgentID GW::Agents::GetHeroAgentID(DWORD heroindex) {
	if (heroindex == 0) return GetPlayerId();

	GW::GameContext* ctx = GameContext::instance();
	if (ctx == nullptr) return 0;
	if (ctx->party == nullptr) return 0;
	if (ctx->party->partyinfo == nullptr) return 0;
	HeroPartyMemberArray heroarray = ctx->party->partyinfo->heroes;

	if (!heroarray.valid() || (DWORD)heroindex > heroarray.size()) return 0;

	return heroarray[--heroindex].agentid;
}

GW::PlayerArray GW::Agents::GetPlayerArray() {
	return GameContext::instance()->world->players;
}

GW::NPCArray GW::Agents::GetNPCArray() {
	return GameContext::instance()->world->npcs;
}
