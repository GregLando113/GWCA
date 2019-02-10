#include "stdafx.h"

#include <GWCA/Utilities/Export.h>
#include <GWCA/Utilities/Macros.h>
#include <GWCA/Utilities/Scanner.h>

#include <GWCA/Managers/Module.h>
#include <GWCA/Managers/CtoSMgr.h>

namespace {
    using namespace GW;

    typedef void(__fastcall *SendPacket_pt)(
        uint32_t context, uint32_t size, void* packet);
    SendPacket_pt SendPacket_Func;

    uintptr_t game_srv_object_addr;

    void Init() {
        SendPacket_Func = (SendPacket_pt)Scanner::Find(
            "\x55\x8B\xEC\x83\xEC\x2C\x53\x56\x57\x8B\xF9\x85", "xxxxxxxxxxxx", 0);
        printf("[SCAN] SendPacket = %p\n", SendPacket_Func);

        {
            uintptr_t address = Scanner::Find(
                "\x56\x33\xF6\x3B\xCE\x74\x0E\x56\x33\xD2", "xxxxxxxxxx", -4);
            printf("[SCAN] CtoGSObjectPtr = %p\n", (void *)address);
            if (Verify(address))
                game_srv_object_addr = *(uintptr_t *)address;
        }
    }
}

namespace GW {

    Module CtoSModule = {
        "CtoSModule",   // name
        NULL,           // param
        ::Init,         // init_module
        NULL,           // exit_module
        NULL,           // exit_module
        NULL,           // remove_hooks
    };

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
