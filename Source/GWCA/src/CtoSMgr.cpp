#include "stdafx.h"

#include <GWCA/Utilities/Export.h>
#include <GWCA/Utilities/Scanner.h>

#include <GWCA/Managers/CtoSMgr.h>

namespace GW {
    void CtoS::SendPacket(uint32_t size, void *buffer) {
        typedef void(__fastcall *SendCtoGSPacket_t)(
            uint32_t ctogsobj, uint32_t size, void* packet);

        static SendCtoGSPacket_t gs_send_func = nullptr;
        static uint32_t **game_srv_object = nullptr;
        if (!game_srv_object) {
            game_srv_object = (uint32_t **)Scanner::Find(
                "\x56\x33\xF6\x3B\xCE\x74\x0E\x56\x33\xD2", "xxxxxxxxxx", -4);
            printf("[SCAN] CtoGSObjectPtr = %p\n", game_srv_object);
        }
        if (!gs_send_func) {
            gs_send_func = (SendCtoGSPacket_t)Scanner::Find(
                "\x55\x8B\xEC\x83\xEC\x2C\x53\x56\x57\x8B\xF9\x85", "xxxxxxxxxxxx", 0);
            printf("[SCAN] CtoGSSendFunction = %p\n", gs_send_func);
        }
        if (gs_send_func && game_srv_object) {
            gs_send_func(**game_srv_object, size, buffer);
        }
    }

    void CtoS::SendPacket(uint32_t size, ...) {
        uint32_t *pak = &size + 1;
        SendPacket(size, pak);
    }
} // namespace GW
