#include "stdafx.h"

#include <GWCA/Utilities/Export.h>
#include <GWCA/Utilities/Scanner.h>

#include <GWCA/GameContainers/Array.h>
#include <GWCA/GameContainers/Vector.h>

#include <GWCA/Packets/StoC.h>

#include <GWCA/Managers/StoCMgr.h>

namespace {
    typedef bool(__fastcall *StoCHandler_t)(GW::Packet::StoC::PacketBase *pak);

    struct StoCHandler {
        uint32_t      *packet_template;
        uint32_t      template_size;
        StoCHandler_t handler_func;
    };

    typedef GW::Array<StoCHandler> StoCHandlerArray;

    StoCHandlerArray game_server_handler;
    StoCHandler *original_functions = nullptr;
    std::map<uint32_t, std::map<uint32_t, GW::StoC::CallbackFunc<GW::Packet::StoC::PacketBase>>> event_calls;
    uint32_t last_identifier = 0;

    bool __fastcall StoCHandler_Func(GW::Packet::StoC::PacketBase *pak) {
        bool do_not_process = false;
        for (auto call : event_calls[pak->header]) {
            if (call.second(pak)) do_not_process = true;
        }
        return do_not_process ? true : original_functions[pak->header].handler_func(pak);
    }

    void OriginalHandler(GW::Packet::StoC::PacketBase *packet) {
        original_functions[packet->header].handler_func(packet);
    }
}

namespace GW {
    void GW::StoC::Initialize() {
        uintptr_t found = Scanner::Find("\x50\x52\x8B\x55\x0C\xC7\x45\xF8", "xxxxxxxx", -0x23);
        printf("[SCAN] StoCHandler pattern = %p\n", (void *)found);
        if (!found) return;

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

        GameServer **addr = *(GameServer ***)found;
        game_server_handler = (*addr)->gs_codec->handlers;

        original_functions = new StoCHandler[game_server_handler.size()];

        for (uint32_t i = 0; i < game_server_handler.size(); ++i) {
            original_functions[i] = game_server_handler[i];
        }
    }

    uint32_t GW::StoC::AddCallback(uint32_t header,
        std::function<bool (Packet::StoC::PacketBase *)> callback) {
        if (original_functions == nullptr) Initialize();
        last_identifier++;
        event_calls[header][last_identifier] = callback;
        game_server_handler[header].handler_func = StoCHandler_Func;
        return last_identifier;
    }

    void GW::StoC::RemoveCallback(uint32_t header, uint32_t identifier) {
        event_calls[header].erase(identifier);
    }

    void GW::StoC::EmulatePacket(Packet::StoC::PacketBase *packet) {
        if (original_functions == nullptr) Initialize();
        OriginalHandler(packet);
    }

    void GW::StoC::RestoreHooks() {
        if (original_functions == nullptr) return;
        for (uint32_t i = 0; i < game_server_handler.size(); ++i) {
            if (game_server_handler[i].handler_func != original_functions[i].handler_func) {
                game_server_handler[i].handler_func = original_functions[i].handler_func;
            }
        }
        delete[] original_functions;
    }
} // namespace GW
