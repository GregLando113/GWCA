#include "stdafx.h"

#include <GWCA/Utilities/Export.h>
#include <GWCA/Utilities/Hooker.h>
#include <GWCA/Utilities/Macros.h>
#include <GWCA/Utilities/Scanner.h>

#include <GWCA/GameContainers/Array.h>
#include <GWCA/GameContainers/GamePos.h>

#include <GWCA/Packets/StoC.h>

#include <GWCA/Managers/Module.h>
#include <GWCA/Managers/StoCMgr.h>

namespace {
    using namespace GW;

    typedef bool (__cdecl *StoCHandler_pt)(Packet::StoC::PacketBase *pak);
    struct StoCHandler {
        uint32_t      *packet_template;
        uint32_t       template_size;
        StoCHandler_pt handler_func;
    };
    typedef Array<StoCHandler> StoCHandlerArray;

    struct GameServer {
        uint8_t h0000[8];
        struct {
            uint8_t h0000[12];
            struct {
                uint8_t h0000[12];
                void *next;
                uint8_t h0010[12];
                uint32_t ClientCodecArray[4];
                StoCHandlerArray handlers;
            } *ls_codec;
            uint8_t h0010[12];
            // Client codec
            uint32_t ClientCodecArray[4];
            StoCHandlerArray handlers;
        } *gs_codec;
    };

    StoCHandlerArray game_server_handlers;
    StoCHandler *original_functions = nullptr;

    std::vector<std::unordered_map<HookEntry *, StoC::PacketCallback>> packets_callbacks;

    bool __cdecl StoCHandler_Func(Packet::StoC::PacketBase *pak) {
        GW::HookBase::EnterHook();
        HookStatus status;

        for (auto& it : packets_callbacks[pak->header]) {
            it.second(&status, pak);
            ++status.altitude;
        }

        if (!status.blocked)
            original_functions[pak->header].handler_func(pak);
        GW::HookBase::LeaveHook();
        return true;
    }

    void OriginalHandler(Packet::StoC::PacketBase *packet) {
        original_functions[packet->header].handler_func(packet);
    }

    void Init() {
        uintptr_t StoCHandler_Addr;
        {
            uintptr_t address = Scanner::Find(
                "\x75\x04\x33\xC0\x5D\xC3\x8B\x41\x08\xA8\x01\x75", "xxxxxxxxxxxx", -6);
            printf("[SCAN] StoCHandler pattern = %p\n", (void *)address);
            if (Verify(address))
                StoCHandler_Addr = *(uintptr_t *)address;
        }

        GameServer **addr = reinterpret_cast<GameServer **>(StoCHandler_Addr);
        game_server_handlers = (*addr)->gs_codec->handlers;

        original_functions = new StoCHandler[game_server_handlers.size()];
        packets_callbacks.resize(game_server_handlers.size());
    }

    void EnableHooks() {
        for (uint32_t i = 0; i < game_server_handlers.size(); ++i)
            original_functions[i] = game_server_handlers[i];
    }

    void DisableHooks() {
        if (original_functions == nullptr) return;
        for (uint32_t i = 0; i < game_server_handlers.size(); ++i)
            game_server_handlers[i].handler_func = original_functions[i].handler_func;
        delete[] original_functions;
    }
}

namespace GW {

    Module StoCModule = {
        "StoCModule",   // name
        NULL,           // param
        ::Init,         // init_module
        NULL,           // exit_module
        ::EnableHooks,  // enable_hooks
        ::DisableHooks, // disable_hooks
    };

    void StoC::RegisterPacketCallback(
        HookEntry *entry,
        uint32_t header,
        PacketCallback callback)
    {
        packets_callbacks[header].insert({entry, callback});
        game_server_handlers[header].handler_func = StoCHandler_Func;
    }

    void StoC::RemoveCallback(uint32_t header, HookEntry *entry) {
        auto& callbacks = packets_callbacks[header];
        auto it = callbacks.find(entry);
        if (it != callbacks.end())
            callbacks.erase(it);
    }

    void StoC::EmulatePacket(Packet::StoC::PacketBase *packet) {
        if (!Verify(original_functions))
            return;
        OriginalHandler(packet);
    }

} // namespace GW
