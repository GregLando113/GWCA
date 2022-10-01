#include "stdafx.h"

#include <GWCA/Utilities/Debug.h>
#include <GWCA/Utilities/Macros.h>
#include <GWCA/Utilities/Hooker.h>
#include <GWCA/Utilities/Scanner.h>

#include <GWCA/Managers/CtoSMgr.h>
#include <GWCA/Managers/GameThreadMgr.h>
#include <GWCA/Managers/RenderMgr.h>

#define GWCA_CTOS_DISABLED 1

namespace {
    using namespace GW;

    typedef void(__cdecl *SendPacket_pt)(
        uint32_t context, uint32_t size, void* packet);
    SendPacket_pt SendPacket_Func = 0;
    SendPacket_pt RetSendPacket = 0;

    std::vector<std::unordered_map<HookEntry*, CtoS::PacketCallback>> packets_callbacks;
    bool __cdecl CtoSHandler_Func(uint32_t context, uint32_t size, void* packet) {
        HookBase::EnterHook();
        HookStatus status;
        uint32_t header = *(uint32_t*)packet;
        if (header < packets_callbacks.size()) {
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
#ifndef GWCA_CTOS_DISABLED
        SendPacket_Func = (SendPacket_pt)Scanner::FindAssertion("p:\\code\\net\\msg\\msgconn.cpp", "bytes >= sizeof(dword)", -0x67);
#endif
        uintptr_t address = Scanner::FindAssertion("p:\\code\\gw\\net\\cli\\gcgamecmd.cpp","No valid case for switch variable 'code'", -0x32);
        if (Verify(address))
            game_srv_object_addr = *(uintptr_t *)address;

        packets_callbacks.resize(180);

        GWCA_INFO("[SCAN] SendPacket = %p", SendPacket_Func);
        GWCA_INFO("[SCAN] CtoGSObjectPtr = %p", game_srv_object_addr);

#ifdef _DEBUG
#ifndef GWCA_CTOS_DISABLED
        GWCA_ASSERT(SendPacket_Func);
#endif
        GWCA_ASSERT(game_srv_object_addr);
#endif
        HookBase::CreateHook(SendPacket_Func, CtoSHandler_Func, (void**)&RetSendPacket);

    }
    void EnableHooks() {
        if (SendPacket_Func)
            HookBase::EnableHooks(SendPacket_Func);
    }
    void DisableHooks() {
        if (SendPacket_Func)
            HookBase::DisableHooks(SendPacket_Func);
    }
    void Exit() {
        HookBase::RemoveHook(SendPacket_Func);
    }
}


namespace GW {

    Module CtoSModule = {
        "CtoSModule",   // name
        NULL,           // param
        ::Init,         // init_module
        ::Exit,           // exit_module
        ::EnableHooks,           // enable_hooks
        ::DisableHooks,           // disable_hooks
    };
    void CtoS::RegisterPacketCallback(
        HookEntry* entry,
        uint32_t header,
        const PacketCallback& callback)
    {
        packets_callbacks[header].insert({ entry, callback });
    }
    void CtoS::RemoveCallback(uint32_t header, HookEntry* entry) {
        auto& callbacks = packets_callbacks[header];
        auto it = callbacks.find(entry);
        if (it != callbacks.end())
            callbacks.erase(it);
    }
    bool CtoS::SendPacket(uint32_t size, void *buffer) {
        if (!(Verify(SendPacket_Func && game_srv_object_addr)))
            return false;
        if (GameThread::IsInGameThread() || Render::GetIsInRenderLoop()) {
            // Already in game thread, don't need to worry about buffer lifecycle
            SendPacket_Func(*(uint32_t*)game_srv_object_addr, size, buffer);
            return true;
        }
        // Copy the packet and enqueue in the game thread
        void* buffer_cpy = malloc(size);
        GWCA_ASSERT(buffer_cpy != NULL);
        memcpy(buffer_cpy, buffer, size);
        GameThread::Enqueue([buffer_cpy, size]() {
            SendPacket_Func(*(uint32_t*)game_srv_object_addr, size, buffer_cpy);
            free(buffer_cpy);
        });
        return true;
    }

    bool CtoS::SendPacket(uint32_t size, ...) {
        uint32_t *pak = &size + 1;
        return SendPacket(size, pak);
    }
} // namespace GW
