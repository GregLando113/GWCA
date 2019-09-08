#pragma once

#include <GWCA/GameContainers/Array.h>

namespace GW {
    struct ObserverMatch;

    struct CharContext { // total: 0x400
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
        /* +h00FC */ uint8_t h00FC[148]; // 40
        /* +h0190 */ uint32_t h0190;
        /* +h0194 */ uint32_t token1; // world id
        /* +h0198 */ uint32_t map_id_again;
        /* +h019C */ uint32_t is_explorable;
        /* +h01A0 */ uint8_t host[24];
        /* +h01B8 */ uint32_t token2; // player id
        /* +h01BC */ uint8_t h01BC[64];
        /* +h01FC */ uint32_t current_map_id_again;
        /* +h0200 */ uint32_t h0200[8];
        /* +h0208 */ uint32_t district_number;
        /* +h020C */ uint32_t language;
        /* +h0210 */ uint32_t observe_map_id;
        /* +h0214 */ uint32_t current_map_id;
        /* +h0218 */ uint8_t h0218[8];
        /* +h0220 */ Array<ObserverMatch *> observer_matchs;
        /* +h0230 */ uint8_t h0230[240];
        /* +h0320 */ uint32_t *progress_bar;
        /* +h0324 */ uint8_t h0324[108];
        /* +h0380 */ wchar_t player_email[64];
    };
}
