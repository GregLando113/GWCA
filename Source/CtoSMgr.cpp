#include "stdafx.h"

#include <GWCA/Utilities/Export.h>
#include <GWCA/Utilities/Macros.h>
#include <GWCA/Utilities/Scanner.h>

#include <GWCA/Managers/Module.h>
#include <GWCA/Managers/CtoSMgr.h>

namespace {
    using namespace GW;

    typedef void(__cdecl *SendPacket_pt)(
        uint32_t context, uint32_t size, void* packet);
    SendPacket_pt SendPacket_Func;

    uintptr_t game_srv_object_addr;

    void Init() {
        SendPacket_Func = (SendPacket_pt)Scanner::Find(
            "\xF7\xD9\xC7\x47\x54\x01\x00\x00\x00\x1B\xC9\x81", "xxxxxxxxxxxx", -0xC3);
        printf("[SCAN] SendPacket = %p\n", SendPacket_Func);

        {
            uintptr_t address = Scanner::Find(
                "\xC3\xA1\x00\x00\x00\x00\x85\xC0\x74\xF1", "xx????xxxx", +2);
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
        NULL,           // enable_hooks
        NULL,           // disable_hooks
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
