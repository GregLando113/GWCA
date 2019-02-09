#include "stdafx.h"

#include <GWCA/CtoSHeaders.h>
#include <GWCA/Constants/Constants.h>

#include <GWCA/Utilities/Export.h>
#include <GWCA/Utilities/Hooker.h>

#include <GWCA/GameContainers/Vector.h>

#include <GWCA/GameEntities/NPC.h>
#include <GWCA/GameEntities/Hero.h>
#include <GWCA/GameEntities/Agent.h>
#include <GWCA/GameEntities/Party.h>
#include <GWCA/GameEntities/Player.h>
#include <GWCA/GameEntities/Pathing.h>

#include <GWCA/Context/GameContext.h>
#include <GWCA/Context/AgentContext.h>
#include <GWCA/Context/PartyContext.h>
#include <GWCA/Context/WorldContext.h>
#include <GWCA/Context/GadgetContext.h>

#include <GWCA/Managers/UIMgr.h>
#include <GWCA/Managers/MapMgr.h>
#include <GWCA/Managers/CtoSMgr.h>
#include <GWCA/Managers/ItemMgr.h>
#include <GWCA/Managers/AgentMgr.h>
#include <GWCA/Managers/MemoryMgr.h>
#include <GWCA/Managers/GameThreadMgr.h>

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
        printf("[SCAN] DialogFunc = %p\n", DialogFunc);
        lastdialoglog_ret = (BYTE*)lastdialoglog_hook.Detour(DialogFunc, (BYTE*)dialoglog_detour, 9);
    }
}
void GW::Agents::RestoreLastDialogHook() {
    HookBase::DisableHooks(&lastdialoglog_hook);
}
DWORD GW::Agents::GetLastDialogId() { 
    return lastdialog_id;
}
void GW::Agents::Dialog(DWORD id) {
    CtoS::SendPacket(0x8, CtoGS_MSGDialog, id);
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

float GW::Agents::GetDistance(Vec2f a, Vec2f b) {
    return sqrtf((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

float GW::Agents::GetSqrDistance(Vec2f a, Vec2f b) {
    return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}

void GW::Agents::ChangeTarget(GW::AgentID agentid) {
    typedef void(__fastcall *ChangeTarget_t)(DWORD AgentID, DWORD smth);
    static ChangeTarget_t changetarget_func = nullptr;
    if (!changetarget_func) {
        changetarget_func = (ChangeTarget_t)Scanner::Find("\x33\xC0\x3B\xDA\x0F\x95\xC0\x33", "xxxxxxxx", -0x78);
        printf("[SCAN] ChangeTargetFunction = %p\n", changetarget_func);
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
        printf("[SCAN] MoveFunction = %p\n", move_func);
    }
    if (move_func) move_func(&pos);
}

GW::MapAgentArray GW::Agents::GetMapAgentArray() {
    return GameContext::instance()->world->map_agents;
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
    CtoS::SendPacket(0xC, CtoGS_MSGGoNPC, Agent->agent_id, CallTarget);
}

void GW::Agents::GoPlayer(GW::Agent* Agent) {
    CtoS::SendPacket(0x8, CtoGS_MSGGoPlayer, Agent->agent_id);
}

void GW::Agents::GoSignpost(GW::Agent* Agent, BOOL CallTarget /*= 0*/) {
    CtoS::SendPacket(0xC, CtoGS_MSGGoGadget, Agent->agent_id, CallTarget);
}

void GW::Agents::CallTarget(GW::Agent* Agent) {
    CtoS::SendPacket(0xC, CtoGS_MSGCallTarget, 0xA, Agent->agent_id);
}

DWORD GW::Agents::GetAmountOfPlayersInInstance() {
    // -1 because the 1st array element is nil
    return GameContext::instance()->world->players.size() - 1;
}

wchar_t *GW::Agents::GetPlayerNameByLoginNumber(DWORD loginnumber) {
    PlayerArray &players = GameContext::instance()->world->players;
    if (loginnumber >= players.size())
        return nullptr;
    return players[loginnumber].name;
}

DWORD GW::Agents::GetAgentIdByLoginNumber(DWORD loginnumber) {
    return GameContext::instance()->world->players[loginnumber].agent_id;
}

GW::AgentID GW::Agents::GetHeroAgentID(DWORD heroindex) {
    if (heroindex == 0) return GetPlayerId();

    GW::GameContext* ctx = GameContext::instance();
    if (ctx == nullptr) return 0;
    if (ctx->party == nullptr) return 0;
    if (ctx->party->player_party == nullptr) return 0;
    HeroPartyMemberArray heroarray = ctx->party->player_party->heroes;

    if (!heroarray.valid() || (DWORD)heroindex > heroarray.size()) return 0;

    return heroarray[--heroindex].agent_id;
}

GW::PlayerArray GW::Agents::GetPlayerArray() {
    return GameContext::instance()->world->players;
}

GW::NPCArray GW::Agents::GetNPCArray() {
    return GameContext::instance()->world->npcs;
}

std::wstring GW::Agents::GetAgentName(GW::Agent *agent) {
    // @Remark: I'm not conviced that the name is decoded synchronously so that could avoid crashes
    // if it is not the cases. We should still avoid to call this function.
    static std::wstring buffer;
    buffer.clear();
    AsyncGetAgentName(agent, buffer);
    if (buffer.size() != 0)
        return buffer;
    return L"";
}

void GW::Agents::AsyncGetAgentName(GW::Agent *agent, std::wstring& res) {
    wchar_t *str = nullptr;
    res = L"";

    if (!agent) return;
    if (agent->GetIsCharacterType()) {
        if (agent->login_number) {
            GW::PlayerArray players = GameContext::instance()->world->players;
            if (!players.valid()) return;

            GW::Player *player = &players[agent->login_number];
            if (player) res = player->name;
        } else {
            // @Remark:
            // For living npcs it's not elegant, but the game does it as well. See arround GetLivingName(AgentID id)@007C2A00.
            // It first look in the AgentInfo arrays, if it doesn't find it, it does a bunch a shit and fallback on NPCArray.
            // If we only use NPCArray, we have a problem because 2 agents can share the same PlayerNumber.
            // In Isle of Nameless, few npcs (Zaischen Weapond Collector) share the PlayerNumber with "The Guide" so using NPCArray only won't work.
            // But, the dummies (Suit of xx Armor) don't have there NameString in AgentInfo array, so we need NPCArray.
            GW::Array<AgentInfo> npcs = GameContext::instance()->world->agent_infos;
            if (agent->agent_id >= npcs.size()) return;
            str = npcs[agent->agent_id].name_enc;
            if (!str) {
                GW::NPCArray npcs = GameContext::instance()->world->npcs;
                if (!npcs.valid()) return;
                str = npcs[agent->player_number].name_enc;
                if (!str) return;
            }
            GW::UI::AsyncDecodeStr(str, &res);
        }
    } else if (agent->GetIsGadgetType()) {
        AgentContext *ctx = GameContext::instance()->agent;
        GadgetContext *gadget = GameContext::instance()->gadget;
        if (!ctx || !gadget) return;
        auto *GadgetIds = ctx->gadget_data[agent->agent_id].gadget_ids;
        if (!GadgetIds) return;

        str = GadgetIds->name_enc;
        if (!GadgetIds->name_enc) {
            size_t id = GadgetIds->gadget_id;
            if (gadget->GadgetInfo.size() <= id) return;
            str = gadget->GadgetInfo[id].name_enc;
            if (!str) return;
        }

        GW::UI::AsyncDecodeStr(str, &res);
    } else if (agent->GetIsItemType()) {
        GW::ItemArray items = GW::Items::GetItemArray();
        if (!items.valid()) return;
        GW::Item *item = items[agent->item_id];
        if (!item || !item->name_enc) return;
        str = item->name_enc;
        GW::UI::AsyncDecodeStr(str, &res);
    }
}
