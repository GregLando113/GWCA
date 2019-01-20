#pragma once

#include <GWCA/GameContainers/Array.h>

namespace GW {
    struct ObserverMatch;

    struct CharContext { // total: 0x03A8
        /* +h0000 */ Array<void *> h0000;
        /* +h0010 */ uint8_t h0010[4];
        /* +h0014 */ Array<void *> h0014;
        /* +h0024 */ uint8_t h0024[16];
        /* +h0034 */ Array<void *> h0034;
        /* +h0044 */ Array<void *> h0044;
        /* +h0054 */ uint8_t h0054[32];
        /* +h0074 */ wchar_t player_name[20];
        /* +h009C */ uint8_t h009C[80];
        /* +h00EC */ Array<void *> h00EC;
        /* +h00FC */ uint8_t h00FC[40];
        /* +h0124 */ uint32_t h0124;
        /* +h0128 */ uint32_t token1; // world id
        /* +h012C */ uint32_t map_id_again;
        /* +h0130 */ uint32_t is_explorable;
        /* +h0134 */ uint8_t host[24];
        /* +h014C */ uint32_t token2; // player id
        /* +h0150 */ uint8_t h0130[64];
        /* +h0190 */ uint32_t current_map_id_again;
        /* +h0194 */ uint32_t h0194[8];
        /* +h01B4 */ uint32_t district_number;
        /* +h01B8 */ uint32_t h01B8;
        /* +h01BC */ uint32_t current_map_id;
        /* +h01C0 */ uint32_t current_map_id_dupe;
        /* +h01C4 */ uint8_t h01C4[8];
        /* +h01CC */ Array<ObserverMatch *> observer_matchs;
        /* +h01DC */ uint8_t h001DC[332];
        /* +h0328 */ wchar_t player_email[64];
    };
}
