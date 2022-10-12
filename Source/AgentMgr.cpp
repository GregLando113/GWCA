#include "stdafx.h"

#include <GWCA/Context/GameContext.h>
#include <GWCA/Context/AgentContext.h>
#include <GWCA/Context/GadgetContext.h>
#include <GWCA/Context/WorldContext.h>

#include <GWCA/GameEntities/Agent.h>
#include <GWCA/GameEntities/NPC.h>
#include <GWCA/GameEntities/Player.h>
#include <GWCA/GameEntities/Item.h>

#include <GWCA/Managers/Module.h>
#include <GWCA/Managers/PlayerMgr.h>
#include <GWCA/Managers/PartyMgr.h>
#include <GWCA/Managers/AgentMgr.h>
#include <GWCA/Managers/ItemMgr.h>
#include <GWCA/Managers/UIMgr.h>

#include <GWCA/Utilities/Hooker.h>
#include <GWCA/Utilities/Scanner.h>

namespace {
    using namespace GW;

    uint32_t last_dialog_id = 0;
    uint32_t dialog_agent_id = 0;

    HookEntry OnSendDialog_HookEntry;
    typedef void (*SendDialog_pt)(uint32_t dialog_id);
    SendDialog_pt RetSendDialog = 0;
    SendDialog_pt SendDialog_Func = 0;
    SendDialog_pt RetSendSignpostDialog = 0;
    SendDialog_pt SendSignpostDialog_Func = 0;

    void OnSendDialog(uint32_t dialog_id) {
        GW::Hook::EnterHook();
        // Pass this through UI, we'll pick it up in OnSendDialog_UIMessage
        UI::SendUIMessage(UI::UIMessage::kSendDialog, (void*)dialog_id);
        GW::Hook::LeaveHook();
    };
    void OnDialogBody_UIMessage(GW::HookStatus*, UI::UIMessage message_id, void* wparam, void*) {
        GWCA_ASSERT(message_id == UI::UIMessage::kDialogBody && wparam);
        UI::DialogBodyInfo* info = (UI::DialogBodyInfo*)wparam;
        // Save dialog agent id; used in OnSendDialog_UIMessage to tell which type of packet to send.
        dialog_agent_id = info->agent_id;
    }
    void OnSendDialog_UIMessage(GW::HookStatus* status, UI::UIMessage message_id, void* wparam, void*) {
        GWCA_ASSERT(message_id == UI::UIMessage::kSendDialog);
        if (status->blocked)
            return;
        last_dialog_id = (uint32_t)wparam;
        GW::Agent* a = GW::Agents::GetAgentByID(dialog_agent_id);
        if (a && a->GetIsGadgetType()) {
            RetSendSignpostDialog(last_dialog_id);
        }
        else {
            RetSendDialog(last_dialog_id);
        }
    }

    typedef void(*ChangeTarget_pt)(uint32_t agent_id, uint32_t unk1);
    ChangeTarget_pt ChangeTarget_Func = 0;

    typedef void(*MovementChange_pt)(uint32_t type, void* unk1, void* type_ptr);
    MovementChange_pt MovementChange_Func = 0;

    enum class InteractionActionType : uint32_t {
        Enemy,
        Player,
        NPC,
        Item,
        Follow,
        Gadget
    };

    typedef void(*InteractAgent_pt)(InteractionActionType action_type, uint32_t agent_id, uint32_t check_if_call_target_key_held);
    InteractAgent_pt InteractAgent_Func = 0;

    typedef void(*InteractPlayer_pt)(uint32_t agent_id);
    InteractPlayer_pt InteractPlayer_Func = 0;

    typedef void(*InteractCallableAgent_pt)(uint32_t agent_id, uint32_t call_target);
    InteractCallableAgent_pt InteractNPC_Func = 0;
    InteractCallableAgent_pt InteractNPC_Ret = 0;
    InteractCallableAgent_pt InteractItem_Func = 0;
    InteractCallableAgent_pt InteractGadget_Func = 0;
    InteractCallableAgent_pt InteractEnemy_Func = 0;

    void OnInteractNPC(uint32_t agent_id, uint32_t call_target) {
        GW::Hook::EnterHook();
        InteractNPC_Ret(agent_id, call_target);
        GW::Hook::LeaveHook();
    }

    typedef void(*CallTarget_pt)(CallTargetType type, uint32_t agent_id);
    CallTarget_pt CallTarget_Func = 0;
    CallTarget_pt CallTarget_Ret = 0;

    void OnCallTarget(CallTargetType type, uint32_t agent_id) {
        GW::Hook::EnterHook();
        CallTarget_Ret(type, agent_id);
        GW::Hook::LeaveHook();
    }

    typedef void(*Move_pt)(GamePos *pos);
    Move_pt Move_Func = 0;

    uintptr_t AgentArrayPtr = 0;
    uintptr_t PlayerAgentIdPtr = 0;
    uintptr_t TargetAgentIdPtr = 0;
    uintptr_t MouseOverAgentIdPtr = 0;
    uintptr_t IsAutoRunningPtr = 0;

    AgentList *AgentListPtr = nullptr;

    void Init() {
        uintptr_t address = 0;

        address = Scanner::Find( "\x3B\xDF\x0F\x95", "xxxx", -0x0089);
        if (address) {
            ChangeTarget_Func = (ChangeTarget_pt)address;

            TargetAgentIdPtr = *(uintptr_t*)(address + 0x94);
            if (!Scanner::IsValidPtr(TargetAgentIdPtr))
                TargetAgentIdPtr = 0;

            MouseOverAgentIdPtr = TargetAgentIdPtr + 0x8;
            if (!Scanner::IsValidPtr(MouseOverAgentIdPtr))
                MouseOverAgentIdPtr = 0;
        }

        address = Scanner::Find("\xFF\x50\x10\x47\x83\xC6\x04\x3B\xFB\x75\xE1", "xxxxxxxxxxx", +0xD);
        if (Scanner::IsValidPtr(*(uintptr_t*)address))
            AgentArrayPtr = *(uintptr_t*)address;

        address = Scanner::Find("\x5D\xE9\x00\x00\x00\x00\x55\x8B\xEC\x53","xx????xxxx", -0xE);
        if (Scanner::IsValidPtr(*(uintptr_t*)address))
            PlayerAgentIdPtr = *(uintptr_t*)address;

        address = Scanner::Find("\x89\x4b\x24\x8b\x4b\x28\x83\xe9\x00", "xxxxxxxxx");
        if (address) {
            SendDialog_Func = (SendDialog_pt)Scanner::FunctionFromNearCall(address + 0x15);
            SendSignpostDialog_Func = (SendDialog_pt)Scanner::FunctionFromNearCall(address + 0x25);
        }


        address = Scanner::Find("\xc7\x45\xf0\x98\x3a\x00\x00", "xxxxxxx", 0x41);
        InteractAgent_Func = (InteractAgent_pt)Scanner::FunctionFromNearCall(address);
        if (InteractAgent_Func) {
            address = (uintptr_t)InteractAgent_Func;
            InteractEnemy_Func = (InteractCallableAgent_pt)Scanner::FunctionFromNearCall(address + 0x73);
            InteractPlayer_Func = (InteractPlayer_pt)Scanner::FunctionFromNearCall(address + 0xB2);
            Move_Func = (Move_pt)Scanner::FunctionFromNearCall(address + 0xC7);
            CallTarget_Func = (CallTarget_pt)Scanner::FunctionFromNearCall(address + 0xD6);
            InteractNPC_Func = (InteractCallableAgent_pt)Scanner::FunctionFromNearCall(address + 0xE7);
            InteractItem_Func = (InteractCallableAgent_pt)Scanner::FunctionFromNearCall(address + 0xF8);
            // NB: What is UI message 0x100001a0 ?
            InteractGadget_Func = (InteractCallableAgent_pt)Scanner::FunctionFromNearCall(address + 0x120);
        }
        HookBase::CreateHook(CallTarget_Func, OnCallTarget, (void**)&CallTarget_Ret);
        HookBase::CreateHook(InteractNPC_Func, OnInteractNPC, (void**)&InteractNPC_Ret);
        HookBase::CreateHook(SendDialog_Func, OnSendDialog, (void**)&RetSendDialog);
        HookBase::CreateHook(SendSignpostDialog_Func, OnSendDialog, (void**)&RetSendSignpostDialog);
        UI::RegisterUIMessageCallback(&OnSendDialog_HookEntry, UI::UIMessage::kDialogBody, OnDialogBody_UIMessage, 0x1);
        UI::RegisterUIMessageCallback(&OnSendDialog_HookEntry, UI::UIMessage::kSendDialog, OnSendDialog_UIMessage, 0x1);

        GWCA_INFO("[SCAN] ChangeTargetFunction = %p", ChangeTarget_Func);
        GWCA_INFO("[SCAN] TargetAgentIdPtr = %p", TargetAgentIdPtr);
        GWCA_INFO("[SCAN] MouseOverAgentIdPtr = %p", MouseOverAgentIdPtr);
        GWCA_INFO("[SCAN] AgentArrayPtr = %p", AgentArrayPtr);
        GWCA_INFO("[SCAN] PlayerAgentIdPtr = %p", PlayerAgentIdPtr);
        GWCA_INFO("[SCAN] SendDialog Function = %p", SendDialog_Func);
        GWCA_INFO("[SCAN] SendSignpostDialog_Func = %p", SendSignpostDialog_Func);
        GWCA_INFO("[SCAN] InteractEnemy Function = %p", InteractEnemy_Func);
        GWCA_INFO("[SCAN] InteractPlayer Function = %p", InteractPlayer_Func);
        GWCA_INFO("[SCAN] InteractNPC Function = %p", InteractNPC_Func);
        GWCA_INFO("[SCAN] InteractItem Function = %p", InteractItem_Func);
        GWCA_INFO("[SCAN] InteractIGadget Function = %p", InteractGadget_Func);
        GWCA_INFO("[SCAN] MoveTo Function = %p", Move_Func);
        GWCA_INFO("[SCAN] CallTarget Function = %p", CallTarget_Func);

#ifdef _DEBUG
        GWCA_ASSERT(ChangeTarget_Func);
        GWCA_ASSERT(TargetAgentIdPtr);
        GWCA_ASSERT(MouseOverAgentIdPtr);
        GWCA_ASSERT(AgentArrayPtr);
        GWCA_ASSERT(PlayerAgentIdPtr);
        GWCA_ASSERT(SendDialog_Func);
        GWCA_ASSERT(SendSignpostDialog_Func);
        GWCA_ASSERT(InteractEnemy_Func);
        GWCA_ASSERT(InteractPlayer_Func);
        GWCA_ASSERT(Move_Func);
        GWCA_ASSERT(CallTarget_Func);
        GWCA_ASSERT(InteractNPC_Func);
        GWCA_ASSERT(InteractItem_Func);
        GWCA_ASSERT(InteractGadget_Func);
#endif



    }
    void EnableHooks() {
        if (SendDialog_Func)
            HookBase::EnableHooks(SendDialog_Func);
        if (SendSignpostDialog_Func)
            HookBase::EnableHooks(SendSignpostDialog_Func);
        if (CallTarget_Func)
            HookBase::EnableHooks(CallTarget_Func);
        if (InteractNPC_Func)
            HookBase::EnableHooks(InteractNPC_Func);
    }
    void DisableHooks() {
        if(SendDialog_Func)
            HookBase::DisableHooks(SendDialog_Func);
        if(SendSignpostDialog_Func)
            HookBase::DisableHooks(SendSignpostDialog_Func);
        if(CallTarget_Func)
            HookBase::DisableHooks(CallTarget_Func);
        if(InteractNPC_Func)
            HookBase::DisableHooks(InteractNPC_Func);
    }
    void Exit() {
        HookBase::RemoveHook(SendDialog_Func);
        HookBase::RemoveHook(SendSignpostDialog_Func);
        HookBase::RemoveHook(CallTarget_Func);
        HookBase::RemoveHook(InteractNPC_Func);
    }
}

namespace GW {

    Module AgentModule = {
        "AgentModule",      // name
        NULL,               // param
        ::Init,             // init_module
        ::Exit,             // exit_module
        ::EnableHooks,               // enable_hooks
        ::DisableHooks,               // disable_hooks
    };

    namespace Agents {

        bool GetIsAgentTargettable(const GW::Agent* agent) {
            if (!agent) return false;
            if (!agent->GetIsLivingType())
                return true;
            const GW::AgentLiving* l = agent->GetAsAgentLiving();
            if (l->IsPlayer())
                return true;
            const GW::NPC* npc = GW::Agents::GetNPCByID(l->player_number);
            if (npc && (npc->npc_flags & 0x10000) == 0)
                return true;
            return false;
        }

        uint32_t GetLastDialogId() {
            return last_dialog_id;
        }
        bool SendDialog(uint32_t dialog_id) {
            return UI::SendUIMessage(UI::UIMessage::kSendDialog, (void*)dialog_id);
        }

        AgentArray* GetAgentArray() {
            auto* agents = (AgentArray*)AgentArrayPtr;
            return agents && agents->valid() ? agents : nullptr;
        }
        uint32_t GetPlayerId() {
            return *(uint32_t*)PlayerAgentIdPtr;
        }
        uint32_t GetTargetId() {
            return *(uint32_t*)TargetAgentIdPtr;
        }
        uint32_t GetMouseoverId() {
            return *(uint32_t*)MouseOverAgentIdPtr;
        }

        bool ChangeTarget(AgentID agent_id)
        {
            if (!ChangeTarget_Func)
                return false;
            ChangeTarget_Func(agent_id, 0);
            return true;
        }

        bool ChangeTarget(const Agent* agent) {
            if (!agent)
                return false;
            return ChangeTarget(agent->agent_id);
        }

        bool Move(float x, float y, uint32_t zplane /*= 0*/) {
            GamePos pos;
            pos.x = x;
            pos.y = y;
            pos.zplane = zplane;
            return Move(pos);
        }

        bool Move(GamePos pos) {
            if (!Move_Func)
                return false;
            Move_Func(&pos);
            return true;
        }
        uint32_t GetAmountOfPlayersInInstance() {
            auto* w = GetWorldContext();
            // -1 because the 1st array element is nil
            return w && w->players.valid() ? w->players.size() - 1 : 0;
        }

        MapAgentArray* GetMapAgentArray() {
            auto* w = GetWorldContext();
            return w ? &w->map_agents : nullptr;
        }

        MapAgent* GetMapAgentByID(uint32_t agent_id) {
            auto* agents = agent_id ? GetMapAgentArray() : nullptr;
            return agents && agent_id < agents->size() ? &agents->at(agent_id) : nullptr;
        }

        Agent* GetAgentByID(uint32_t agent_id) {
            auto* agents = agent_id ? GetAgentArray() : nullptr;
            return agents && agent_id < agents->size() ? agents->at(agent_id) : nullptr;
        }

        Agent* GetPlayerByID(uint32_t player_id) {
            return GetAgentByID(PlayerMgr::GetPlayerAgentId(player_id));
        }

        AgentLiving* GetCharacter() {
            Agent* a = GetPlayerByID(PlayerMgr::GetPlayerNumber());
            return a ? a->GetAsAgentLiving() : nullptr;
        }

        AgentLiving* GetPlayerAsAgentLiving()
        {
            Agent* a = GetPlayer();
            return a ? a->GetAsAgentLiving() : nullptr;
        }

        AgentLiving* GetTargetAsAgentLiving()
        {
            Agent* a = GetTarget();
            return a ? a->GetAsAgentLiving() : nullptr;
        }

        bool GoNPC(const Agent* agent, uint32_t call_target) {
            if (!(InteractNPC_Func && agent && agent->GetIsLivingType()))
                return false;
            InteractNPC_Func(agent->agent_id, call_target ? 1U : 0U);
            return true;
        }
        bool PickUpItem(const Agent* agent, uint32_t call_target) {
            if (!(InteractItem_Func && agent && agent->GetIsItemType()))
                return false;
            InteractItem_Func(agent->agent_id, call_target ? 1U : 0U);
            return true;
        }

        bool GoPlayer(const Agent* agent, uint32_t call_target) {
            if (!(InteractPlayer_Func && agent && agent->GetIsLivingType() && agent->GetAsAgentLiving()->IsPlayer()))
                return false;
            InteractPlayer_Func(agent->agent_id);
            if (call_target) {
                CallTarget_Func(CallTargetType::Following, agent->agent_id);
            }
            return true;
        }

        bool GoSignpost(const Agent* agent, uint32_t call_target) {
            if (!(InteractGadget_Func && agent && agent->GetIsGadgetType()))
                return false;
            InteractGadget_Func(agent->agent_id, call_target ? 1U : 0U);
            return true;
        }

        bool CallTarget(const Agent* agent, CallTargetType type) {
            if (!(CallTarget_Func && agent))
                return false;
            CallTarget_Func(type, agent->agent_id);
            return true;
        }

        wchar_t* GetPlayerNameByLoginNumber(uint32_t login_number) {
            return PlayerMgr::GetPlayerName(login_number);
        }

        uint32_t GetAgentIdByLoginNumber(uint32_t login_number) {
            auto* player = PlayerMgr::GetPlayerByID(login_number);
            return player ? player->agent_id : 0;
        }

        uint32_t GetHeroAgentID(uint32_t hero_index) {
            return PartyMgr::GetHeroAgentID(hero_index);
        }

        PlayerArray* GetPlayerArray() {
            auto* w = GetWorldContext();
            return w && w->players.valid() ? &w->players : nullptr;
        }

        NPCArray* GetNPCArray() {
            auto* w = GetWorldContext();
            return w && w->npcs.valid() ? &w->npcs : nullptr;
        }

        NPC* GetNPCByID(uint32_t npc_id) {
            auto* npcs = GetNPCArray();
            return npcs && npc_id < npcs->size() ? &npcs->at(npc_id) : nullptr;
        }

        wchar_t* GetAgentEncName(uint32_t agent_id) {
            const Agent* agent = GetAgentByID(agent_id);
            if (agent) {
                return GetAgentEncName(agent);
            }
            GW::AgentInfoArray& agent_infos = GetWorldContext()->agent_infos;
            if (!agent_infos.valid() || agent_id >= agent_infos.size()) {
                return nullptr;
            }
            return agent_infos[agent_id].name_enc;
        }

        wchar_t* GetAgentEncName(const Agent* agent) {
            if (!agent)
                return nullptr;
            if (agent->GetIsLivingType()) {
                const AgentLiving* ag = agent->GetAsAgentLiving();
                if (ag->login_number) {
                    PlayerArray* players = GetPlayerArray();
                    if (!players)
                        return nullptr;
                    Player* player = &players->at(ag->login_number);
                    if (player)
                        return player->name_enc;
                }
                // @Remark:
                // For living npcs it's not elegant, but the game does it as well. See arround GetLivingName(AgentID id)@007C2A00.
                // It first look in the AgentInfo arrays, if it doesn't find it, it does a bunch a shit and fallback on NPCArray.
                // If we only use NPCArray, we have a problem because 2 agents can share the same PlayerNumber.
                // In Isle of Nameless, few npcs (Zaischen Weapond Collector) share the PlayerNumber with "The Guide" so using NPCArray only won't work.
                // But, the dummies (Suit of xx Armor) don't have there NameString in AgentInfo array, so we need NPCArray.
                Array<AgentInfo>& agent_infos = GetWorldContext()->agent_infos;
                if (ag->agent_id >= agent_infos.size()) return nullptr;
                if (agent_infos[ag->agent_id].name_enc)
                    return agent_infos[ag->agent_id].name_enc;
                NPC* npc = GetNPCByID(ag->player_number);
                return npc ? npc->name_enc : nullptr;
            }
            if (agent->GetIsGadgetType()) {
                AgentContext* ctx = GetAgentContext();
                GadgetContext* gadget = GetGameContext()->gadget;
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
                const AgentItem* ag = agent->GetAsAgentItem();
                Item* item = Items::GetItemById(ag->item_id);
                return item ? item->name_enc : nullptr;
            }
            return nullptr;
        }

        bool AsyncGetAgentName(const Agent* agent, std::wstring& res) {
            wchar_t* str = GetAgentEncName(agent);
            if (!str) return false;
            UI::AsyncDecodeStr(str, &res);
            return true;
        }
    }
} // namespace GW
