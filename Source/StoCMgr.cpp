#include "stdafx.h"

#include <GWCA/Utilities/Debug.h>
#include <GWCA/Utilities/Export.h>
#include <GWCA/Utilities/Hooker.h>
#include <GWCA/Utilities/Macros.h>
#include <GWCA/Utilities/Scanner.h>

#include <GWCA/GameContainers/Array.h>

#include <GWCA/Packets/StoC.h>

#include <GWCA/Managers/Module.h>
#include <GWCA/Managers/StoCMgr.h>
#include <GWCA/Managers/GameThreadMgr.h>

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

    StoCHandlerArray* game_server_handlers;
    StoCHandler *original_functions = nullptr;

    // Callbacks are triggered by weighting
    struct CallbackEntry {
        int altitude;
        HookEntry* entry;
        GW::StoC::PacketCallback callback;
    };
    std::vector<std::vector<CallbackEntry>> packet_entries;

    bool __cdecl StoCHandler_Func(Packet::StoC::PacketBase *pak) {
        GW::HookBase::EnterHook();
        HookStatus status;
        auto it = packet_entries[pak->header].begin();
        // Pre callbacks
        while (it != packet_entries[pak->header].end()) {
            if (it->altitude > 0)
                break;
            it->callback(&status, pak);
            ++status.altitude;
            it++;
        }

        if (!status.blocked)
            original_functions[pak->header].handler_func(pak);

        // Post callbacks
        while (it != packet_entries[pak->header].end()) {
            it->callback(&status, pak);
            ++status.altitude;
            it++;
        }

        GW::HookBase::LeaveHook();
        return true;
    }

    void OriginalHandler(Packet::StoC::PacketBase *packet) {
        original_functions[packet->header].handler_func(packet);
    }

    void Init() {
        uintptr_t StoCHandler_Addr;
        {
            uintptr_t address = Scanner::Find("\x75\x04\x33\xC0\x5D\xC3\x8B\x41\x08\xA8\x01\x75", "xxxxxxxxxxxx", -6);
            GWCA_INFO("[SCAN] StoCHandler pattern = %p\n", (void *)address);
            if (Verify(address)) {
                StoCHandler_Addr = *(uintptr_t*)address;
                GameServer** addr = reinterpret_cast<GameServer**>(StoCHandler_Addr);
                game_server_handlers = &(*addr)->gs_codec->handlers;
            }
        }

        original_functions = new StoCHandler[game_server_handlers->size()];
        packet_entries.resize(game_server_handlers->size());
    }

    void EnableHooks() {
        for (uint32_t i = 0; i < game_server_handlers->size(); ++i)
            original_functions[i] = game_server_handlers->at(i);
    }

    void DisableHooks() {
        if (original_functions == nullptr) return;
        for (uint32_t i = 0; i < game_server_handlers->size(); ++i)
            game_server_handlers->at(i).handler_func = original_functions[i].handler_func;
        delete[] original_functions;
        original_functions = nullptr;
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
        const PacketCallback& callback,
        int altitude)
    {
        RemoveCallback(header, entry);
        auto it = packet_entries[header].begin();
        while (it != packet_entries[header].end()) {
            if (it->altitude > altitude)
                break;
            it++;
        }
        packet_entries[header].insert(it, { altitude,entry, callback});
        game_server_handlers->at(header).handler_func = StoCHandler_Func;
    }

    GWCA_API void StoC::RegisterPostPacketCallback(HookEntry* entry, uint32_t header, const PacketCallback& callback)
    {
        return RegisterPacketCallback(entry, header, callback, 0x8000);
    }

    void StoC::RemoveCallback(uint32_t header, HookEntry *entry) {
        auto it = packet_entries[header].begin();
        while (it != packet_entries[header].end()) {
            if (it->entry == entry) {
                packet_entries[header].erase(it);
                break;
            }
            it++;
        }
    }

    GWCA_API void StoC::RemovePostCallback(uint32_t header, HookEntry* entry)
    {
        RemoveCallback(header, entry);
    }

    void StoC::EmulatePacket(Packet::StoC::PacketBase *packet) {
        if (!Verify(original_functions))
            return;
		OriginalHandler(packet);
    }

} // namespace GW
