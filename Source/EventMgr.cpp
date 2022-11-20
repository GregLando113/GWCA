#include "stdafx.h"

#include <GWCA/Utilities/Debug.h>
#include <GWCA/Utilities/Hooker.h>
#include <GWCA/Utilities/Macros.h>
#include <GWCA/Utilities/Scanner.h>

#include <GWCA/GameEntities/FriendsList.h>

#include <GWCA/Managers/Module.h>
#include <GWCA/Managers/EventMgr.h>

namespace {
    using namespace GW;

    typedef uint32_t(__cdecl *SendEventMessage_pt)(void *event_context,uint32_t unk1,EventMgr::EventID event_id,void* data_buffer,uint32_t data_length);
    SendEventMessage_pt SendEventMessage_Func = 0;
    SendEventMessage_pt SendEventMessage_Ret = 0;

    // Callbacks are triggered by weighting
    struct CallbackEntry {
        int altitude;
        HookEntry* entry;
        EventMgr::EventCallback callback;
    };
    std::unordered_map<EventMgr::EventID, std::vector<CallbackEntry>> Event_callbacks;

    
    uint32_t __cdecl OnSendEventMessage(void *event_context,uint32_t unk1,EventMgr::EventID event_id,void* data_buffer,uint32_t data_length)
    {
        HookBase::EnterHook();
        HookStatus status;
        uint32_t ret = 1;
        auto found = Event_callbacks.find(event_id);
        if (found == Event_callbacks.end()) {
            HookBase::LeaveHook();
            return SendEventMessage_Ret(event_context, unk1, event_id, data_buffer, data_length);
        }

        auto it = found->second.begin();
        const auto& end = found->second.end();
        // Pre callbacks
        while (it != end) {
            if (it->altitude > 0)
                break;
            it->callback(&status, event_id, data_buffer, data_length);
            ++status.altitude;
            it++;
        }
        if (!status.blocked) {
            ret = SendEventMessage_Ret(event_context, unk1, event_id, data_buffer, data_length);
        }

        // Post callbacks
        while (it != end) {
            it->callback(&status, event_id, data_buffer, data_length);
            ++status.altitude;
            it++;
        }

        HookBase::LeaveHook();
        return ret;
    }


    void Init() {

        uintptr_t address = Scanner::Find("\x68\x08\x08\x00\x00\x8d\x85\xf4\xf7\xff\xff", "xxxxxxxxxxx", 0x16);
        SendEventMessage_Func = (SendEventMessage_pt)Scanner::FunctionFromNearCall(address);
        HookBase::CreateHook(SendEventMessage_Func, OnSendEventMessage, (void**)&SendEventMessage_Ret);

        GWCA_INFO("[SCAN] SendEventMessage_Func = %p", SendEventMessage_Func);

#ifdef _DEBUG
        GWCA_ASSERT(SendEventMessage_Func);
#endif
    }

    void EnableHooks() {
        if (SendEventMessage_Func)
            HookBase::EnableHooks(SendEventMessage_Func);
    }

    void DisableHooks() {
        if(SendEventMessage_Func)
            HookBase::DisableHooks(SendEventMessage_Func);
    }

    void Exit() {
        HookBase::RemoveHook(SendEventMessage_Func);
    }
}

namespace GW {

    Module EventMgrModule = {
        "EventMgrModule",  // name
        NULL,               // param
        ::Init,             // init_module
        ::Exit,             // exit_module
        ::EnableHooks,               // enable_hooks
        ::DisableHooks,               // disable_hooks
    };

    namespace EventMgr {
        GWCA_API void RegisterEventCallback(HookEntry* entry, EventID event_id, const EventCallback& callback, int altitude)
        {
            auto found = Event_callbacks.find(event_id);
            if (found == Event_callbacks.end()) {
                Event_callbacks[event_id] = std::vector<CallbackEntry>();
            }
            auto it = Event_callbacks[event_id].begin();
            while (it != Event_callbacks[event_id].end()) {
                if (it->altitude > altitude)
                    break;
                it++;
            }
            Event_callbacks[event_id].insert(it, { altitude, entry, callback});
        }
        GWCA_API void RemoveEventCallback(HookEntry* entry)
        {
            for (auto& it : Event_callbacks) {
                auto it2 = it.second.begin();
                while (it2 != it.second.end()) {
                    if (it2->entry == entry) {
                        it.second.erase(it2);
                        return;
                    }
                    it2++;
                }
            }
        }
    }
} // namespace GW


