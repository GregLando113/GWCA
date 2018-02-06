#include <GWCA\Managers\AgentMgr.h>

#include <GWCA\Managers\MemoryMgr.h>
#include <GWCA\Managers\GameThreadMgr.h>
#include <GWCA\Managers\CtoSMgr.h>
#include <GWCA\Managers\MapMgr.h>
#include <GWCA\Managers\ItemMgr.h>

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

	GW::AgentList *AgentListPtr = nullptr;
}

void GW::Agents::Initialize() {
	// Agent Array
	BYTE* agArrayPtr = (BYTE*)MemoryMgr::agArrayPtr;
	PlayerAgentIDPtr = (DWORD*)(agArrayPtr - 0x54);
	TargetAgentIDPtr = (DWORD*)(agArrayPtr - 0x500);
	MouseOverAgentIDPtr = (DWORD*)(agArrayPtr - 0x4F4);

	AgentListPtr = (AgentList*)(agArrayPtr - 0x40);
}

void GW::Agents::SetupLastDialogHook() {
	if (lastdialoglog_hook.Empty()) {
		BYTE* DialogFunc = (BYTE*)Scanner::Find("\x55\x8B\xEC\x83\xEC\x28\x53\x56\x57\x8B\xF2\x8B\xD9", "xxxxxxxxxxxxx", -0x28);
		printf("DialogFunc = %p\n", DialogFunc);
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
DWORD GW::Agents::GetMouseoverId() {
	return *MouseOverAgentIDPtr;
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
		printf("ChangeTargetFunction = %p\n", changetarget_func);
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
		printf("MoveFunction = %p\n", move_func);
	}
	if (move_func) move_func(&pos);
}

GW::MapAgentArray GW::Agents::GetMapAgentArray() {
	return GameContext::instance()->world->mapagents;
}

GW::Agent* GW::Agents::GetAgentByID(DWORD id) {
	GW::AgentArray agents = GetAgentArray();
	if (agents.valid() && id > 0 && id < agents.size()) {
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

wchar_t *GW::Agents::GetPlayerNameByLoginNumber(DWORD loginnumber) {
	PlayerArray &players = GameContext::instance()->world->players;
	if (loginnumber >= players.size())
		return nullptr;
	return players[loginnumber].Name;
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
	assert(AsyncDecodeStr);

	wchar_t  buffer[256] = L"";
	wchar_t *str = nullptr;

	if (!agent) return L"";

	if (agent->GetIsCharacterType()) {
		if (agent->LoginNumber) {
			GW::PlayerArray players = GameContext::instance()->world->players;
			if (!players.valid()) return L"";

			GW::Player *player = &players[agent->LoginNumber];
			if (!player) return L"";
			return std::wstring(player->Name);
		} else {
			// @Remark:
			// For living npcs it's not elegant, but the game does it as well. See arround GetLivingName(AgentID id)@007C2A00.
			// It first look in the AgentInfo arrays, if it doesn't find it, it does a bunch a shit and fallback on NPCArray.
			// If we only use NPCArray, we have a problem because 2 agents can share the same PlayerNumber.
			// In Isle of Nameless, few npcs (Zaischen Weapond Collector) share the PlayerNumber with "The Guide" so using NPCArray only won't work.
			// But, the dummies (Suit of xx Armor) don't have there NameString in AgentInfo array, so we need NPCArray.
			GW::Array<AgentInfo> npcs = GameContext::instance()->world->agentInfos;
			if (agent->Id >= npcs.size()) return L"";
			str = npcs[agent->Id].NameString;
			if (!str) {
				GW::NPCArray npcs = GameContext::instance()->world->npcs;
				if (!npcs.valid()) return L"";
				str = npcs[agent->PlayerNumber].NameString;
			}
			assert(str);
			AsyncDecodeStr(str, __decode_str_callback, buffer);
			return std::wstring(buffer);
		}
	} else if (agent->GetIsGadgetType()) {
		AgentContext *ctx = GameContext::instance()->agent;
		GadgetContext *gadget = GameContext::instance()->gadget;
		if (!ctx || !gadget) return L"";
		auto *GadgetIds = ctx->GadgetData[agent->Id].GadgetIds;
		if (!GadgetIds) return L"";

		str = GadgetIds->NameString;
		if (!GadgetIds->NameString) {
			size_t id = GadgetIds->GadgetId;
			if (gadget->GadgetInfo.size() <= id) return L"";
			str = gadget->GadgetInfo[id].NameString;
		}

		assert(str);
		AsyncDecodeStr(str, __decode_str_callback, buffer);
		return std::wstring(buffer);
	} else if (agent->GetIsItemType()) {
		GW::ItemArray items = GW::Items::GetItemArray();
		if (!items.valid()) return L"";
		GW::Item *item = items[agent->ItemID];
		if (!item) return L"";
		str = item->NameString;
		assert(str);
		AsyncDecodeStr(str, __decode_str_callback, buffer);
		return std::wstring(buffer);
	}

	return L"";
}
