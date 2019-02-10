#include "stdafx.h"

#include <GWCA/Utilities/Export.h>
#include <GWCA/Utilities/Macros.h>
#include <GWCA/Utilities/Scanner.h>

#include <GWCA/GameContainers/Array.h>
#include <GWCA/GameContainers/Vector.h>

#include <GWCA/Packets/StoC.h>

#include <GWCA/Managers/Module.h>
#include <GWCA/Managers/StoCMgr.h>

namespace {
    using namespace GW;

    typedef bool (__fastcall *StoCHandler_pt)(Packet::StoC::PacketBase *pak);

    struct StoCHandler {
        uint32_t      *packet_template;
        uint32_t       template_size;
        StoCHandler_pt handler_func;
    };

    typedef Array<StoCHandler> StoCHandlerArray;

    StoCHandlerArray game_server_handler;
    StoCHandler *original_functions = nullptr;
    std::map<uint32_t, std::map<uint32_t, StoC::CallbackFunc<Packet::StoC::PacketBase>>> event_calls;
    uint32_t last_identifier = 0;

    bool __fastcall StoCHandler_Func(Packet::StoC::PacketBase *pak) {
        bool do_not_process = false;
        for (auto call : event_calls[pak->header]) {
            if (call.second(pak)) do_not_process = true;
        }
        return do_not_process ? true : original_functions[pak->header].handler_func(pak);
    }

    void OriginalHandler(Packet::StoC::PacketBase *packet) {
        original_functions[packet->header].handler_func(packet);
    }

    uintptr_t StoCHandler_Addr;

    void Init() {
        {
            uintptr_t address = Scanner::Find("\x50\x52\x8B\x55\x0C\xC7\x45\xF8", "xxxxxxxx", -0x23);
            printf("[SCAN] StoCHandler pattern = %p\n", (void *)address);
            if (Verify(address))
                StoCHandler_Addr = *(uintptr_t *)address;
        }
    }

    void CreateHooks() {
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

        GameServer **addr = (GameServer **)StoCHandler_Addr;
        game_server_handler = (*addr)->gs_codec->handlers;

        original_functions = new StoCHandler[game_server_handler.size()];

        for (uint32_t i = 0; i < game_server_handler.size(); ++i) {
            original_functions[i] = game_server_handler[i];
        }
    }

    void RemoveHooks() {
        if (original_functions == nullptr) return;
        for (uint32_t i = 0; i < game_server_handler.size(); ++i) {
            if (game_server_handler[i].handler_func != original_functions[i].handler_func) {
                game_server_handler[i].handler_func = original_functions[i].handler_func;
            }
        }
        delete[] original_functions;
    }
}

namespace GW {

    Module StoCModule = {
        "StoCModule",   // name
        NULL,           // param
        ::Init,         // init_module
        NULL,           // exit_module
        ::CreateHooks,  // exit_module
        ::RemoveHooks,  // remove_hooks
    };

    uint32_t StoC::AddCallback(uint32_t header,
        std::function<bool (Packet::StoC::PacketBase *)> callback) {
        if (!Verify(original_functions))
            return 0;
        last_identifier++;
        event_calls[header][last_identifier] = callback;
        game_server_handler[header].handler_func = StoCHandler_Func;
        return last_identifier;
    }

    void StoC::RemoveCallback(uint32_t header, uint32_t identifier) {
        event_calls[header].erase(identifier);
    }

    void StoC::EmulatePacket(Packet::StoC::PacketBase *packet) {
        if (!Verify(original_functions))
            return;
        OriginalHandler(packet);
    }

} // namespace GW
