#include "stdafx.h"

#include <GWCA/Packets/Opcodes.h>
#include <GWCA/Constants/Constants.h>

#include <GWCA/Utilities/Debug.h>
#include <GWCA/Utilities/Export.h>
#include <GWCA/Utilities/Hooker.h>
#include <GWCA/Utilities/Macros.h>
#include <GWCA/Utilities/Scanner.h>

#include <GWCA/GameContainers/GamePos.h>

#include <GWCA/GameEntities/NPC.h>
#include <GWCA/GameEntities/Item.h>
#include <GWCA/GameEntities/Agent.h>
#include <GWCA/GameEntities/Party.h>
#include <GWCA/GameEntities/Player.h>

#include <GWCA/Context/GameContext.h>
#include <GWCA/Context/AgentContext.h>
#include <GWCA/Context/PartyContext.h>
#include <GWCA/Context/WorldContext.h>
#include <GWCA/Context/GadgetContext.h>

#include <GWCA/Managers/Module.h>

#include <GWCA/Managers/UIMgr.h>
#include <GWCA/Managers/MapMgr.h>
#include <GWCA/Managers/CtoSMgr.h>
#include <GWCA/Managers/ItemMgr.h>
#include <GWCA/Managers/AgentMgr.h>
#include <GWCA/Managers/MemoryMgr.h>
#include <GWCA/Managers/GameThreadMgr.h>
#include <GWCA/Managers/PlayerMgr.h>

namespace {
    using namespace GW;

    uint32_t last_dialog_id = 0;
    std::unordered_map<HookEntry*, Agents::DialogCallback> OnDialog_callbacks;

    typedef void (*SendDialog_pt)(uint32_t dialog_id);
    SendDialog_pt RetSendDialog;
    SendDialog_pt SendDialog_Func;

    void OnSendDialog(uint32_t dialog_id) {
        HookBase::EnterHook();

        HookStatus status;
        for (auto& it : OnDialog_callbacks) {
            it.second(&status, dialog_id);
            ++status.altitude;
        }

        if (status.blocked) {
            RetSendDialog(0);
        } else {
            last_dialog_id = dialog_id;
            RetSendDialog(dialog_id);
        }

        HookBase::LeaveHook();
    };

    typedef void(*ChangeTarget_pt)(uint32_t agent_id, uint32_t unk1);
    ChangeTarget_pt ChangeTarget_Func;

    typedef void(*MovementChange_pt)(uint32_t type, void* unk1, void* type_ptr);
    MovementChange_pt MovementChange_Func;

    typedef void(*Move_pt)(GamePos *pos);
    Move_pt Move_Func;

    uintptr_t AgentArrayPtr = 0;
    uintptr_t PlayerAgentIdPtr = 0;
    uintptr_t TargetAgentIdPtr = 0;
    uintptr_t MouseOverAgentIdPtr = 0;
    uintptr_t IsAutoRunningPtr = 0;

    AgentList *AgentListPtr = nullptr;

    void Init() {
        MovementChange_Func = (MovementChange_pt)Scanner::Find(
            "\x0C\x05\x6F\xFF\xFF\xFF", "xxxxxx", -0x9);
        GWCA_INFO("[SCAN] MovementChangeFunction = %p\n", MovementChange_Func);
        if (MovementChange_Func)
            IsAutoRunningPtr = *reinterpret_cast<uintptr_t*>((uintptr_t)MovementChange_Func + 0x40) - 0x4;
        GWCA_INFO("[SCAN] IsAutoRunningPtr = %p\n", IsAutoRunningPtr);

        //ChangeTarget_Func = (ChangeTarget_pt)Scanner::Find(
        //    "\x53\x8B\x5D\x0C\x56\x8B\x75\x08\x85", "xxxxxxxxx", -0x10);
        ChangeTarget_Func = (ChangeTarget_pt)Scanner::Find(
            "\x3B\xDF\x0F\x95", "xxxx", -0x0089);
        GWCA_INFO("[SCAN] ChangeTargetFunction = %p\n", ChangeTarget_Func);

        if (ChangeTarget_Func) {
            uintptr_t address = Scanner::Find(
                "\xFF\x50\x10\x47\x83\xC6\x04\x3B\xFB\x75\xE1","xxxxxxxxxxx", +0xD);
            AgentArrayPtr = *reinterpret_cast<uintptr_t*>(address);

            TargetAgentIdPtr = *(uintptr_t*)((uintptr_t)ChangeTarget_Func + 0x94);
            MouseOverAgentIdPtr = TargetAgentIdPtr + 0x8;
        }

        PlayerAgentIdPtr = Scanner::Find("\x5D\xE9\x00\x00\x00\x00\x55\x8B\xEC\x53","xx????xxxx", -0xE);
        if (PlayerAgentIdPtr) {
            PlayerAgentIdPtr = *(uintptr_t*)PlayerAgentIdPtr;
            GWCA_INFO("[SCAN] PlayerAgentIdPtr = %p\n", (void *)PlayerAgentIdPtr);
        }

        AgentListPtr = (AgentList * )Scanner::Find("\x8D\x0C\x88\xE8\x00\x00\x00\x00\x8B\xC3", "xxxx????xx", 0x3C);
        if (AgentListPtr) {
            AgentListPtr = *(AgentList * *)AgentListPtr;
            GWCA_INFO("[SCAN] AgentListPtr = %p\n", AgentListPtr);
        }

        uintptr_t address = Scanner::Find(
            "\xFF\x50\x10\x47\x83\xC6\x04\x3B\xFB\x75\xE1", "xxxxxxxxxxx", +0xD);
        AgentArrayPtr = *reinterpret_cast<uintptr_t*>(address);

        Move_Func = (Move_pt)Scanner::Find(
                "\xDF\xE0\xF6\xC4\x41\x7B\x64\x56\xE8", "xxxxxxxxx", -0x48);
        GWCA_INFO("[SCAN] MoveFunction = %p\n", Move_Func);

        SendDialog_Func = (SendDialog_pt)Scanner::Find(
            "\x83\xC8\x01\x89\x46\x24\x8B\x46\x28\x83\xE8\x00\x74\x0D", "xxxxxxxxxxxxxx", 0x15);
        if (SendDialog_Func) {
            SendDialog_Func = (SendDialog_pt)((uintptr_t)SendDialog_Func + *(uintptr_t*)SendDialog_Func + 4);
        }
        GWCA_INFO("[SCAN] DialogFunc = %p\n", SendDialog_Func);

        if (Verify(SendDialog_Func))
            HookBase::CreateHook(SendDialog_Func, OnSendDialog, (void **)&RetSendDialog);
    }

    void Exit() {
        if (SendDialog_Func)
            HookBase::RemoveHook(SendDialog_Func);
    }

    bool IsAutoRunning() {
        return *(uint32_t*)IsAutoRunningPtr == 1;
    }
}

namespace GW {

    Module AgentModule = {
        "AgentModule",      // name
        NULL,               // param
        ::Init,             // init_module
        ::Exit,             // exit_module
        NULL,               // enable_hooks
        NULL,               // disable_hooks
    };

    uint32_t Agents::GetLastDialogId() { 
        return last_dialog_id;
    }
    void Agents::SendDialog(uint32_t dialog_id) {
        CtoS::SendPacket(0x8, GAME_CMSG_SEND_DIALOG, dialog_id);
    }

    AgentArray Agents::GetAgentArray() {
        return *(AgentArray *)AgentArrayPtr;
    }
    uint32_t Agents::GetPlayerId() {
        return *(uint32_t *)PlayerAgentIdPtr;
    }
    uint32_t Agents::GetTargetId() {
        return *(uint32_t *)TargetAgentIdPtr;
    }
    uint32_t Agents::GetMouseoverId() {
        return *(uint32_t *)MouseOverAgentIdPtr;
    }

    void Agents::ChangeTarget(AgentID agent_id) {
        if (!Verify(ChangeTarget_Func))
            return;
        AgentArray agents = GetAgentArray();
        if (agent_id == 0 || (agents.valid() && agents[agent_id] != nullptr))
            ChangeTarget_Func(agent_id, 0);
    }

    void Agents::ChangeTarget(const Agent *agent) {
        if (agent)
            ChangeTarget(agent->agent_id);
    }

    void Agents::Move(float x, float y, uint32_t zplane /*= 0*/) {
        GamePos pos;
        pos.x = x;
        pos.y = y;
        pos.zplane = zplane;
        Agents::Move(pos);
    }

    void Agents::Move(GamePos pos) {
        if (Verify(MovementChange_Func) && IsAutoRunning()) {
            // Kill autorun, queue movement for next frame.
            uint32_t movement_type = 0xB7; // Autorun
            uint32_t unk1 = 0;
            MovementChange_Func(movement_type, &unk1,&movement_type);
            GameThread::Enqueue([pos]() {
                Move_Func((GamePos*)&pos);
                });
        } else {
            Move_Func(&pos);
        }
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



    Agent* Agents::GetPlayerByID(uint32_t player_id)
    {
        Player* p = PlayerMgr::GetPlayerByID(player_id);
        return p ? GetAgentByID(p->agent_id) : nullptr;
    }

    AgentLiving* Agents::GetCharacter() {
        Agent* a = GetPlayerByID(PlayerMgr::GetPlayerNumber());
        return a ? a->GetAsAgentLiving() : nullptr;
    }

    AgentLiving *Agents::GetPlayerAsAgentLiving()
    {
        Agent *agent = GetPlayer();
        if (agent)
            return agent->GetAsAgentLiving();
        else
            return nullptr;
    }

    AgentLiving *Agents::GetTargetAsAgentLiving()
    {
        Agent *agent = GetTarget();
        if (agent)
            return agent->GetAsAgentLiving();
        else
            return nullptr;
    }

    void Agents::GoNPC(const Agent *agent, uint32_t call_target) {
        CtoS::SendPacket(0xC, GAME_CMSG_INTERACT_LIVING, agent->agent_id, call_target);
    }

    void Agents::GoPlayer(const Agent *agent) {
        CtoS::SendPacket(0x8, GAME_CMSG_INTERACT_PLAYER, agent->agent_id);
    }

    void Agents::GoSignpost(const Agent *agent, uint32_t call_target) {
        CtoS::SendPacket(0xC, GAME_CMSG_INTERACT_GADGET, agent->agent_id, call_target);
    }

    void Agents::CallTarget(const Agent *agent) {
        CtoS::SendPacket(0xC, GAME_CMSG_TARGET_CALL, 0xA, agent->agent_id);
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

    NPC *Agents::GetNPCByID(uint32_t npc_id) {
        NPCArray npcs = GetNPCArray();
        if (npc_id >= npcs.size())
            return NULL;
        else
            return &npcs[npc_id];
    }

    std::wstring Agents::GetAgentName(const Agent *agent) {
        // @Remark: I'm not conviced that the name is decoded synchronously so that could avoid crashes
        // if it is not the cases. We should still avoid to call this function.
        static std::wstring buffer;
        buffer.clear();
        AsyncGetAgentName(agent, buffer);
        if (buffer.size() != 0)
            return buffer;
        return L"";
    }

    wchar_t* Agents::GetAgentEncName(uint32_t agent_id) {
        const Agent* agent = GetAgentByID(agent_id);
        if (agent) {
            return GetAgentEncName(agent);
        }
        GW::AgentInfoArray& agent_infos = GameContext::instance()->world->agent_infos;
        if (!agent_infos.valid() || agent_id >= agent_infos.size()) {
            return nullptr;
        }
        return agent_infos[agent_id].name_enc;
    }

    wchar_t* Agents::GetAgentEncName(const Agent* agent) {
        if (!agent) 
            return nullptr;
        if (agent->GetIsLivingType()) {
            const AgentLiving *ag = agent->GetAsAgentLiving();
            if (ag->login_number) {
                PlayerArray players = GameContext::instance()->world->players;
                if (!players.valid()) 
                    return nullptr;
                Player* player = &players[ag->login_number];
                if (player)
                    return player->name_enc;
            }
            // @Remark:
            // For living npcs it's not elegant, but the game does it as well. See arround GetLivingName(AgentID id)@007C2A00.
            // It first look in the AgentInfo arrays, if it doesn't find it, it does a bunch a shit and fallback on NPCArray.
            // If we only use NPCArray, we have a problem because 2 agents can share the same PlayerNumber.
            // In Isle of Nameless, few npcs (Zaischen Weapond Collector) share the PlayerNumber with "The Guide" so using NPCArray only won't work.
            // But, the dummies (Suit of xx Armor) don't have there NameString in AgentInfo array, so we need NPCArray.
            Array<AgentInfo> agent_infos = GameContext::instance()->world->agent_infos;
            if (ag->agent_id >= agent_infos.size()) return nullptr;
            if (agent_infos[ag->agent_id].name_enc)
                return agent_infos[ag->agent_id].name_enc;
            NPCArray npcs = GameContext::instance()->world->npcs;
            if (!npcs.valid()) 
                return nullptr;
            return npcs[ag->player_number].name_enc;
        }
        if (agent->GetIsGadgetType()) {
            AgentContext* ctx = GameContext::instance()->agent;
            GadgetContext* gadget = GameContext::instance()->gadget;
            if (!ctx || !gadget) return nullptr;
            auto* GadgetIds = ctx->agent_summary_info[agent->agent_id].extra_info_sub;
            if (!GadgetIds) 
                return nullptr;
            if (GadgetIds->gadget_name_enc)
                return GadgetIds->gadget_name_enc;
            size_t id = GadgetIds->gadget_id;
            if (gadget->GadgetInfo.size() <= id) return nullptr;
            if (gadget->GadgetInfo[id].name_enc)
                return gadget->GadgetInfo[id].name_enc;
            return nullptr;
        }
        if (agent->GetIsItemType()) {
            const AgentItem *ag = agent->GetAsAgentItem();
            ItemArray items = Items::GetItemArray();
            if (!items.valid()) return nullptr;
            Item* item = items[ag->item_id];
            if (!item || !item->name_enc) return nullptr;
            return item->name_enc;
        }
        return nullptr;
    }

    void Agents::AsyncGetAgentName(const Agent *agent, std::wstring& res) {
        wchar_t* str = GetAgentEncName(agent);
        if (!str) return;
        UI::AsyncDecodeStr(str, &res);
    }

    void Agents::RegisterDialogCallback(
        HookEntry* entry,
        DialogCallback callback)
    {
        OnDialog_callbacks.insert({ entry, callback });
    }

    void Agents::RemoveDialogCallback(
        HookEntry* entry)
    {
        auto it = OnDialog_callbacks.find(entry);
        if (it != OnDialog_callbacks.end())
            OnDialog_callbacks.erase(it);
    }
} // namespace GW
