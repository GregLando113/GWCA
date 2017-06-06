#include <GWCA\Managers\AgentMgr.h>

#include <GWCA\Managers\MemoryMgr.h>
#include <GWCA\Managers\GameThreadMgr.h>
#include <GWCA\Managers\CtoSMgr.h>
#include <GWCA\Managers\MapMgr.h>

#include <GWCA\Context\WorldContext.h>
#include <GWCA\Context\GameContext.h>
#include <GWCA\Context\PartyContext.h>

namespace {
	GW::Hook lastdialoglog_hook;
	BYTE* lastdialoglog_ret = nullptr;
	DWORD lastdialog_id = 0;
	void __declspec(naked) dialoglog_detour() {
		_asm MOV lastdialog_id, ESI
		_asm JMP lastdialoglog_ret
	}

	DWORD* PlayerAgentIDPtr = nullptr;
	DWORD* TargetAgentIDPtr = nullptr;
	DWORD* MouseOverAgentIDPtr = nullptr;
}

void GW::Agents::Initialize() {
	// Agent Array
	BYTE* agArrayPtr = (BYTE*)MemoryMgr::agArrayPtr;
	PlayerAgentIDPtr = (DWORD*)(agArrayPtr - 0x54);
	TargetAgentIDPtr = (DWORD*)(agArrayPtr - 0x500);
	MouseOverAgentIDPtr = (DWORD*)(agArrayPtr - 0x4F4);
}

void GW::Agents::SetupLastDialogHook() {
	if (lastdialoglog_hook.Empty()) {
		BYTE* DialogFunc = (BYTE*)Scanner::Find("\x55\x8B\xEC\x83\xEC\x28\x53\x56\x57\x8B\xF2\x8B\xD9", "xxxxxxxxxxxxx", -0x28);
		printf("DialogFunc = 0x%X\n", (DWORD)DialogFunc);
		lastdialoglog_ret = (BYTE*)lastdialoglog_hook.Detour(DialogFunc, (BYTE*)dialoglog_detour, 9);
	}
}
void GW::Agents::RestoreLastDialogHook() {
	lastdialoglog_hook.Retour();
}
DWORD GW::Agents::GetLastDialogId() { 
	return lastdialog_id;
}
void GW::Agents::Dialog(DWORD id) {
	CtoS::SendPacket(0x8, 0x35, id);
}

GW::AgentArray GW::Agents::GetAgentArray() {
	return *(GW::AgentArray*)MemoryMgr::agArrayPtr;
}
DWORD GW::Agents::GetPlayerId() {
	return *PlayerAgentIDPtr;
}
DWORD GW::Agents::GetTargetId() {
	return *TargetAgentIDPtr;
}

float GW::Agents::GetDistance(Vector2f a, Vector2f b) {
	return sqrtf((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

float GW::Agents::GetSqrDistance(Vector2f a, Vector2f b) {
	return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}

void GW::Agents::ChangeTarget(GW::AgentID agentid) {
	typedef void(__fastcall *ChangeTarget_t)(DWORD AgentID, DWORD smth);
	static ChangeTarget_t changetarget_func = nullptr;
	if (!changetarget_func) {
		changetarget_func = (ChangeTarget_t)Scanner::Find("\x33\xC0\x3B\xDA\x0F\x95\xC0\x33", "xxxxxxxx", -0x78);
		printf("ChangeTargetFunction = %X\n", (DWORD)changetarget_func);
	}
	auto& arr = GetAgentArray();
	if (changetarget_func && arr.valid() && arr[agentid] != nullptr) changetarget_func(agentid, 0);
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
	static Move_t move_func = nullptr;
	if (!move_func) {
		move_func = (Move_t)Scanner::Find("\xD9\x07\xD8\x5D\xF0\xDF\xE0\xF6\xC4\x01", "xxxxxxxxxx", -0x12);
		printf("MoveFunction = %X\n", (DWORD)move_func);
	}
	if (move_func) move_func(&pos);
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
	CtoS::SendPacket(0xC, 0x33, Agent->Id, CallTarget);
}

void GW::Agents::GoPlayer(GW::Agent* Agent) {
	CtoS::SendPacket(0x8, 0x2D, Agent->Id);
}

void GW::Agents::GoSignpost(GW::Agent* Agent, BOOL CallTarget /*= 0*/) {
	CtoS::SendPacket(0xC, 0x4B, Agent->Id, CallTarget);
}

void GW::Agents::CallTarget(GW::Agent* Agent) {
	CtoS::SendPacket(0xC, 0x1C, 0xA, Agent->Id);
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

// GetAgentName stuff.
typedef void(__fastcall *Callback_t)(wchar_t *buff, wchar_t *str);
typedef void(__fastcall *AsyncDecodeStr_t)(wchar_t *s, Callback_t cb, void *param);
static void __fastcall __decode_str_callback(wchar_t *buff, wchar_t *str) {
	while (*str) *buff++ = *str++;
	*buff = 0;
}

std::wstring GW::Agents::GetAgentName(GW::Agent *agent) {
	AsyncDecodeStr_t AsyncDecodeStr = (AsyncDecodeStr_t)MemoryMgr::AsyncDecodeStringPtr;
	if (!agent || agent->Type != 0xDB) return L"";

	if (agent->TypeMap & 0x400000) {
		GW::PlayerArray players = GameContext::instance()->world->players;
		if (!players.valid()) return L"";

		GW::Player *player = &players[agent->PlayerNumber];
		if (!player) return L"";
		return std::wstring(player->Name);
	} else {
		wchar_t dec_name[256] = L"";
		GW::NPCArray npcs = GameContext::instance()->world->npcs;
		if (!npcs.valid()) return L"";

		wchar_t *enc_name = npcs[agent->PlayerNumber].nameString;
		if (!enc_name || !AsyncDecodeStr) return L"";

		AsyncDecodeStr(enc_name, __decode_str_callback, dec_name);
		return std::wstring(dec_name);
	/*
		AgentInfoArray agentInfos = GameContext::instance()->world->agentInfos;
		if (!agentInfos.valid()) return L"";
	*/
	}
}
