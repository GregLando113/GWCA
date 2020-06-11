#include "stdafx.h"
#include <GWCA/Utilities/Debug.h>
#include <GWCA/Managers/CtoSMgr.h>

namespace {
    using namespace GW;

    typedef void(__cdecl *SendPacket_pt)(
        uint32_t context, uint32_t size, void* packet);
    SendPacket_pt SendPacket_Func;
    SendPacket_pt RetSendPacket;
    std::vector<std::unordered_map<HookEntry*, CtoS::PacketCallback>> packets_callbacks;
    bool __cdecl CtoSHandler_Func(uint32_t context, uint32_t size, void* packet) {
        HookBase::EnterHook();
        HookStatus status;
        uint32_t header = *(uint32_t*)packet;
        if (header < packets_callbacks.size()) {
            auto callbacks = packets_callbacks.at(header);
            for (auto& it : packets_callbacks[header]) {
                it.second(&status, packet);
                ++status.altitude;
            }
        }
        if (!status.blocked)
            RetSendPacket(context, size, packet);
        GW::HookBase::LeaveHook();
        return true;
    }
    uintptr_t game_srv_object_addr;

    

    void Init() {
        SendPacket_Func = (SendPacket_pt)Scanner::Find(
            "\xF7\xD9\xC7\x47\x54\x01\x00\x00\x00\x1B\xC9\x81", "xxxxxxxxxxxx", -0xC3);
        GWCA_INFO("[SCAN] SendPacket = %p\n", SendPacket_Func);
        if (Verify(SendPacket_Func))
            HookBase::CreateHook(SendPacket_Func, CtoSHandler_Func, (void**)&RetSendPacket);
        {
            uintptr_t address = Scanner::Find(
                "\xC3\xA1\x00\x00\x00\x00\x85\xC0\x74\xF1", "xx????xxxx", +2);
            GWCA_INFO("[SCAN] CtoGSObjectPtr = %p\n", (void *)address);
            if (Verify(address))
                game_srv_object_addr = *(uintptr_t *)address;
        }
        packets_callbacks.resize(180);
    }
    void Exit() {
        if (SendPacket_Func)
            HookBase::RemoveHook(SendPacket_Func);
    }
}

namespace GW {

    Module CtoSModule = {
        "CtoSModule",   // name
        NULL,           // param
        ::Init,         // init_module
        ::Exit,           // exit_module
        NULL,           // enable_hooks
        NULL,           // disable_hooks
    };
    void CtoS::RegisterPacketCallback(
        HookEntry* entry,
        uint32_t header,
        PacketCallback callback)
    {
        packets_callbacks[header].insert({ entry, callback });
    }
    void CtoS::RemoveCallback(uint32_t header, HookEntry* entry) {
        auto& callbacks = packets_callbacks[header];
        auto it = callbacks.find(entry);
        if (it != callbacks.end())
            callbacks.erase(it);
    }
    void CtoS::SendPacket(uint32_t size, void *buffer) {
        if (Verify(SendPacket_Func && game_srv_object_addr)) {
            SendPacket_Func(*(uint32_t *)game_srv_object_addr, size, buffer);
        }
    }

    void CtoS::SendPacket(uint32_t size, ...) {
        uint32_t *pak = &size + 1;
        SendPacket(size, pak);
    }
} // namespace GW
