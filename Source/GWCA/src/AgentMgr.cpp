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
    BYTE* last_dialog_ret = nullptr;
    uint32_t last_dialog_id = 0;
    void __declspec(naked) dialoglog_detour() {
        _asm MOV last_dialog_id, ESI
        _asm JMP last_dialog_ret
    }

    uintptr_t PlayerAgentIDPtr = 0;
    uintptr_t TargetAgentIDPtr = 0;
    uintptr_t MouseOverAgentIDPtr = 0;

    GW::AgentList *AgentListPtr = nullptr;
}

namespace GW {
    void Agents::Initialize() {
        // Agent Array
        uintptr_t AgentArrayPtr = MemoryMgr::AgentArrayPtr;
        PlayerAgentIDPtr = AgentArrayPtr - 0x54;
        TargetAgentIDPtr = AgentArrayPtr - 0x500;
        MouseOverAgentIDPtr = AgentArrayPtr - 0x4F4;

        AgentListPtr = (AgentList *)(AgentArrayPtr - 0x40);
    }

    void Agents::SetupLastDialogHook() {
        if (lastdialoglog_hook.Empty()) {
            uintptr_t dialog_func = Scanner::Find(
                "\x55\x8B\xEC\x83\xEC\x28\x53\x56\x57\x8B\xF2\x8B\xD9", "xxxxxxxxxxxxx", -0x28);
            printf("[SCAN] DialogFunc = %08lX\n", dialog_func);
            last_dialog_ret = (BYTE*)lastdialoglog_hook.Detour((uint8_t *)dialog_func, (BYTE*)dialoglog_detour, 9);
        }
    }
    void Agents::RestoreLastDialogHook() {
        HookBase::DisableHooks(&lastdialoglog_hook);
    }
    uint32_t Agents::GetLastDialogId() { 
        return last_dialog_id;
    }
    void Agents::SendDialog(uint32_t dialog_id) {
        CtoS::SendPacket(0x8, CtoGS_MSGDialog, dialog_id);
    }

    AgentArray Agents::GetAgentArray() {
        return *(AgentArray *)MemoryMgr::AgentArrayPtr;
    }
    uint32_t Agents::GetPlayerId() {
        return *(uint32_t *)PlayerAgentIDPtr;
    }
    uint32_t Agents::GetTargetId() {
        return *(uint32_t *)TargetAgentIDPtr;
    }
    uint32_t Agents::GetMouseoverId() {
        return *(uint32_t *)MouseOverAgentIDPtr;
    }

    void Agents::ChangeTarget(AgentID agent_id) {
        typedef void(__fastcall *ChangeTarget_t)(uint32_t agent_id, uint32_t unk1);
        static ChangeTarget_t change_target_func = nullptr;
        if (!change_target_func) {
            change_target_func = (ChangeTarget_t)Scanner::Find(
                "\x33\xC0\x3B\xDA\x0F\x95\xC0\x33", "xxxxxxxx", -0x78);
            printf("[SCAN] ChangeTargetFunction = %p\n", change_target_func);
        }
        AgentArray& arr = GetAgentArray();
        if (change_target_func && arr.valid() && arr[agent_id] != nullptr)
            change_target_func(agent_id, 0);
    }

    void Agents::Move(float x, float y, uint32_t zplane /*= 0*/) {
        GamePos pos;
        pos.x = x;
        pos.y = y;
        pos.zplane = zplane;
        Agents::Move(pos);
    }

    void Agents::Move(GamePos pos) {
        typedef void(__fastcall *Move_t)(GamePos *pos);
        static Move_t move_func = nullptr;
        if (!move_func) {
            move_func = (Move_t)Scanner::Find(
                "\xD9\x07\xD8\x5D\xF0\xDF\xE0\xF6\xC4\x01", "xxxxxxxxxx", -0x12);
            printf("[SCAN] MoveFunction = %p\n", move_func);
        }
        if (move_func)
            move_func(&pos);
    }

    MapAgentArray Agents::GetMapAgentArray() {
        return GameContext::instance()->world->map_agents;
    }

    Agent *Agents::GetAgentByID(uint32_t agent_id) {
        AgentArray agents = GetAgentArray();
        if (agents.valid() && agent_id > 0 && agent_id < agents.size()) {
            return agents[agent_id];
        } else {
            return nullptr;
        }
    }

    void Agents::GoNPC(Agent *agent, uint32_t call_target) {
        CtoS::SendPacket(0xC, CtoGS_MSGGoNPC, agent->agent_id, call_target);
    }

    void Agents::GoPlayer(Agent *agent) {
        CtoS::SendPacket(0x8, CtoGS_MSGGoPlayer, agent->agent_id);
    }

    void Agents::GoSignpost(Agent *agent, uint32_t call_target) {
        CtoS::SendPacket(0xC, CtoGS_MSGGoGadget, agent->agent_id, call_target);
    }

    void Agents::CallTarget(Agent *agent) {
        CtoS::SendPacket(0xC, CtoGS_MSGCallTarget, 0xA, agent->agent_id);
    }

    uint32_t Agents::GetAmountOfPlayersInInstance() {
        // -1 because the 1st array element is nil
        return GameContext::instance()->world->players.size() - 1;
    }

    wchar_t *Agents::GetPlayerNameByLoginNumber(uint32_t login_number) {
        PlayerArray &players = GameContext::instance()->world->players;
        if (login_number >= players.size())
            return nullptr;
        return players[login_number].name;
    }

    uint32_t Agents::GetAgentIdByLoginNumber(uint32_t login_number) {
        return GameContext::instance()->world->players[login_number].agent_id;
    }

    AgentID Agents::GetHeroAgentID(uint32_t hero_index) {
        if (hero_index == 0)
            return GetPlayerId();

        GameContext *ctx = GameContext::instance();
        if (ctx == nullptr) return 0;
        if (ctx->party == nullptr) return 0;
        if (ctx->party->player_party == nullptr) return 0;
        HeroPartyMemberArray heroarray = ctx->party->player_party->heroes;

        if (!heroarray.valid() || (uint32_t)hero_index > heroarray.size())
            return 0;
        else
            return heroarray[--hero_index].agent_id;
    }

    PlayerArray Agents::GetPlayerArray() {
        return GameContext::instance()->world->players;
    }

    NPCArray Agents::GetNPCArray() {
        return GameContext::instance()->world->npcs;
    }

    std::wstring Agents::GetAgentName(Agent *agent) {
        // @Remark: I'm not conviced that the name is decoded synchronously so that could avoid crashes
        // if it is not the cases. We should still avoid to call this function.
        static std::wstring buffer;
        buffer.clear();
        AsyncGetAgentName(agent, buffer);
        if (buffer.size() != 0)
            return buffer;
        return L"";
    }

    void Agents::AsyncGetAgentName(Agent *agent, std::wstring& res) {
        wchar_t *str = nullptr;
        res = L"";

        if (!agent) return;
        if (agent->GetIsCharacterType()) {
            if (agent->login_number) {
                PlayerArray players = GameContext::instance()->world->players;
                if (!players.valid()) return;

                Player *player = &players[agent->login_number];
                if (player) res = player->name;
            } else {
                // @Remark:
                // For living npcs it's not elegant, but the game does it as well. See arround GetLivingName(AgentID id)@007C2A00.
                // It first look in the AgentInfo arrays, if it doesn't find it, it does a bunch a shit and fallback on NPCArray.
                // If we only use NPCArray, we have a problem because 2 agents can share the same PlayerNumber.
                // In Isle of Nameless, few npcs (Zaischen Weapond Collector) share the PlayerNumber with "The Guide" so using NPCArray only won't work.
                // But, the dummies (Suit of xx Armor) don't have there NameString in AgentInfo array, so we need NPCArray.
                Array<AgentInfo> npcs = GameContext::instance()->world->agent_infos;
                if (agent->agent_id >= npcs.size()) return;
                str = npcs[agent->agent_id].name_enc;
                if (!str) {
                    NPCArray npcs = GameContext::instance()->world->npcs;
                    if (!npcs.valid()) return;
                    str = npcs[agent->player_number].name_enc;
                    if (!str) return;
                }
                UI::AsyncDecodeStr(str, &res);
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

            UI::AsyncDecodeStr(str, &res);
        } else if (agent->GetIsItemType()) {
            ItemArray items = Items::GetItemArray();
            if (!items.valid()) return;
            Item *item = items[agent->item_id];
            if (!item || !item->name_enc) return;
            str = item->name_enc;
            UI::AsyncDecodeStr(str, &res);
        }
    }
} // namespace GW
