#include "stdafx.h"

#include <GWCA/Constants/Constants.h>

#include <GWCA/Utilities/Scanner.h>
#include <GWCA/Utilities/Hooker.h>

#include <GWCA/GameEntities/Quest.h>

#include <GWCA/Context/WorldContext.h>

#include <GWCA/Managers/Module.h>

#include <GWCA/Managers/UIMgr.h>
#include <GWCA/Managers/QuestMgr.h>

namespace {
    using namespace GW;

    typedef void(__cdecl* DoAction_pt)(uint32_t identifier);

    HookEntry SetActiveQuest_HookEntry;
    DoAction_pt SetActiveQuest_Func = 0;
    DoAction_pt SetActiveQuest_Ret = 0;

    void OnSetActiveQuest(uint32_t quest_id) {
        GW::Hook::EnterHook();
        UI::SendUIMessage(UI::UIMessage::kSendSetActiveQuest, (void*)quest_id);
        GW::Hook::LeaveHook();
    };
    void OnSetActiveQuest_UIMessage(GW::HookStatus* status, UI::UIMessage message_id, void* wparam, void*) {
        GWCA_ASSERT(message_id == UI::UIMessage::kSendSetActiveQuest);
        if (!status->blocked) {
            SetActiveQuest_Ret((uint32_t)wparam);
        }
    }

    HookEntry AbandonQuest_HookEntry;
    DoAction_pt AbandonQuest_Func = 0;
    DoAction_pt AbandonQuest_Ret = 0;

    void OnAbandonQuest(uint32_t quest_id) {
        GW::Hook::EnterHook();
        UI::SendUIMessage(UI::UIMessage::kSendAbandonQuest, (void*)quest_id);
        GW::Hook::LeaveHook();
    }
    void OnAbandonQuest_UIMessage(GW::HookStatus* status, UI::UIMessage message_id, void* wparam, void*) {
        GWCA_ASSERT(message_id == UI::UIMessage::kSendAbandonQuest && wparam);
        if (!status->blocked) {
            AbandonQuest_Ret((uint32_t)wparam);
        }
    }

    void Init() {
        DWORD address = 0;

        address = Scanner::FindAssertion("p:\\code\\gw\\ui\\game\\quest\\questlog.cpp", "MISSION_MAP_OUTPOST == MissionCliGetMap()", -0x128); // UI Callback for quest log window

        AbandonQuest_Func = (DoAction_pt)Scanner::FunctionFromNearCall(address + 0x100);
        if (AbandonQuest_Func) {
            HookBase::CreateHook(AbandonQuest_Func, OnAbandonQuest, (void**)&AbandonQuest_Ret);
            UI::RegisterUIMessageCallback(&AbandonQuest_HookEntry, UI::UIMessage::kSendAbandonQuest, OnAbandonQuest_UIMessage, 0x1);
        }
            
        SetActiveQuest_Func = (DoAction_pt)Scanner::FunctionFromNearCall(address + 0x96);
        if (SetActiveQuest_Func) {
            HookBase::CreateHook(SetActiveQuest_Func, OnSetActiveQuest, (void**)&SetActiveQuest_Ret);
            UI::RegisterUIMessageCallback(&SetActiveQuest_HookEntry, UI::UIMessage::kSendSetActiveQuest, OnSetActiveQuest_UIMessage, 0x1);
        }

        GWCA_INFO("[SCAN] AbandonQuest_Func = %p", AbandonQuest_Func);
        GWCA_INFO("[SCAN] SetActiveQuest_Func = %p", SetActiveQuest_Func);

#ifdef _DEBUG
        GWCA_ASSERT(AbandonQuest_Func);
        GWCA_ASSERT(SetActiveQuest_Func);
#endif
    }
    void EnableHooks() {
        if (AbandonQuest_Func)
            HookBase::EnableHooks(AbandonQuest_Func);
        if (SetActiveQuest_Func)
            HookBase::EnableHooks(SetActiveQuest_Func);
    }

    void DisableHooks() {
        if (AbandonQuest_Func)
            HookBase::DisableHooks(AbandonQuest_Func);
        if (SetActiveQuest_Func)
            HookBase::DisableHooks(SetActiveQuest_Func);
    }

    void Exit() {
        HookBase::RemoveHook(AbandonQuest_Func);
        HookBase::RemoveHook(SetActiveQuest_Func);
    }
}

namespace GW {

    Module QuestModule = {
        "QuestModule",     // name
        NULL,               // param
        Init,               // init_module
        Exit,               // exit_module
        EnableHooks,        // enable_hooks
        DisableHooks,        // remove_hooks
    };
    namespace QuestMgr {

        bool SetActiveQuestId(GW::Constants::QuestID quest_id) {
            if (!(SetActiveQuest_Func && GetQuest(quest_id)))
                return false;
            SetActiveQuest_Func((uint32_t)quest_id);
            return true;
        }
        bool SetActiveQuest(Quest* quest)
        {
            return quest && SetActiveQuestId(quest->quest_id);
        }

        bool GetQuestEntryGroupName(GW::Constants::QuestID quest_id, wchar_t* out, size_t out_len) {
            const auto quest = GetQuest(quest_id);
            if (!(quest && out && out_len))
                return false;
            switch (quest->log_state & 0xf0) {
            case 0x20:
                return swprintf(out, out_len, L"\x564") != -1;
            case 0x40:
                return quest->location && swprintf(out, out_len, L"\x8102\x1978\x10A%s\x1",quest->location) != -1;
            case 0:
                return quest->location && swprintf(out, out_len, L"\x565\x10A%s\x1",quest->location) != -1;
            case 0x10:
                // Unknown, maybe current mission quest, but this type of quest isn't in the quest log.
                break;
            }
            return false;
        }

        Quest* GetActiveQuest() {
            return GetQuest(GetActiveQuestId());
        }
        QuestLog* GetQuestLog() {
            auto* w = GetWorldContext();
            return w && w->quest_log.valid() ? &w->quest_log : nullptr;
        }
        Quest* GetQuest(GW::Constants::QuestID quest_id) {
            if (quest_id == (GW::Constants::QuestID)0)
                return nullptr;
            auto l = GetQuestLog();
            if (!l) return nullptr;
            for (auto& q : *l) {
                if (q.quest_id == quest_id)
                    return &q;
            }
            return nullptr;
        }
        bool AbandonQuest(Quest* quest)
        {
            return quest && AbandonQuestId(quest->quest_id);
        }
        bool AbandonQuestId(Constants::QuestID quest_id)
        {
            if (!(AbandonQuest_Func && GetQuest(quest_id)))
                return false;
            AbandonQuest_Func((uint32_t)quest_id);
            return true;
        }
        GW::Constants::QuestID GetActiveQuestId() {
            auto* w = GetWorldContext();
            return w ? w->active_quest_id : (GW::Constants::QuestID)0;
        }
    }

} // namespace GW
