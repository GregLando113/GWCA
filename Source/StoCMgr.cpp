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
    constexpr uint32_t STOC_HEADER_COUNT = 0x1e5;

    using namespace GW;
    CRITICAL_SECTION mutex;

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

    StoCHandlerArray* game_server_handlers = 0;
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

    bool hooks_enabled = false;
    size_t stoc_handler_count = 0;

    bool OriginalHandler(Packet::StoC::PacketBase *packet) {
        bool ok = false;
        EnterCriticalSection(&mutex);
        if (game_server_handlers && original_functions && stoc_handler_count > packet->header) {
            original_functions[packet->header].handler_func(packet);
        }
        LeaveCriticalSection(&mutex);
        return ok;
    }

    void EnableHooks() {
        EnterCriticalSection(&mutex);
        hooks_enabled = true;
        for (uint32_t i = 0; i < stoc_handler_count; ++i)
            original_functions[i] = game_server_handlers->at(i);
        LeaveCriticalSection(&mutex);
    }

    void DisableHooks() {
        EnterCriticalSection(&mutex);
        hooks_enabled = false;
        if (original_functions) {
            for (uint32_t i = 0; game_server_handlers && i < game_server_handlers->size(); ++i)
                game_server_handlers->at(i).handler_func = original_functions[i].handler_func;
            delete[] original_functions;
            original_functions = nullptr;
        }
        LeaveCriticalSection(&mutex);
    }
    // GW doesn't have the StoC array in RDATA; its built from the different StoC modules in DATA, so trying to find it before GW is fully loaded will cause undefined behaviour
    void InitOnGameThread() {
        EnterCriticalSection(&mutex);
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

        stoc_handler_count = game_server_handlers->size();
        // Because GW registers new handlers module by module, we may have caught it too soon; sanity check to make sure the header count is over 300
        GWCA_ASSERT(stoc_handler_count == STOC_HEADER_COUNT);
        original_functions = new StoCHandler[stoc_handler_count];
        packet_entries.resize(stoc_handler_count);

        // Hook any packet entries that have already been registered
        for (uint32_t i = 0; i < stoc_handler_count; ++i) {
            original_functions[i] = game_server_handlers->at(i);
            if (packet_entries[i].size()) {
                game_server_handlers->at(i).handler_func = StoCHandler_Func;
            }
        }
        LeaveCriticalSection(&mutex);
    }
    void Init() {
        InitializeCriticalSection(&mutex);
        // Requires GameThread module to be hooked!
        GameThread::Enqueue([]() {
            InitOnGameThread();
            });
    }
    void Exit() {
        DisableHooks();
        DeleteCriticalSection(&mutex);
    }
}

namespace GW {

    Module StoCModule = {
        "StoCModule",   // name
        NULL,           // param
        ::Init,         // init_module
        ::Exit,           // exit_module
        ::EnableHooks,  // enable_hooks
        ::DisableHooks, // disable_hooks
    };

    bool StoC::RegisterPacketCallback(
        HookEntry *entry,
        uint32_t header,
        const PacketCallback& callback,
        int altitude)
    {
        bool success = false;
        EnterCriticalSection(&mutex);
        if (packet_entries.size() <= header) {
            packet_entries.resize(header + 1);
        }
        RemoveCallback(header, entry);
        auto it = packet_entries[header].begin();
        while (it != packet_entries[header].end()) {
            if (it->altitude > altitude)
                break;
            it++;
        }
        packet_entries[header].insert(it, { altitude,entry, callback});
        if (game_server_handlers && game_server_handlers->size() > header) {
            game_server_handlers->at(header).handler_func = StoCHandler_Func;
            success = true;
        }
        LeaveCriticalSection(&mutex);
        return success;
    }

    bool StoC::RegisterPostPacketCallback(HookEntry* entry, uint32_t header, const PacketCallback& callback)
    {
        return RegisterPacketCallback(entry, header, callback, 0x8000);
    }

    void StoC::RemoveCallback(uint32_t header, HookEntry *entry) {
        EnterCriticalSection(&mutex);
        if (packet_entries.size() <= header) {
            packet_entries.resize(header + 1);
        }
        auto it = packet_entries[header].begin();
        while (it != packet_entries[header].end()) {
            if (it->entry == entry) {
                packet_entries[header].erase(it);
                break;
            }
            it++;
        }
        LeaveCriticalSection(&mutex);
    }

    void StoC::RemovePostCallback(uint32_t header, HookEntry* entry)
    {
        RemoveCallback(header, entry);
    }

    bool StoC::EmulatePacket(Packet::StoC::PacketBase *packet) {
        return OriginalHandler(packet);
    }

} // namespace GW
