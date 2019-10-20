#pragma once

#include <GWCA/GameContainers/Array.h>

namespace GW {
    struct ObserverMatch;

    struct CharContext { // total: 0x400
        /* +h0000 */ Array<void *> h0000;
        /* +h0010 */ uint8_t h0010[4];
        /* +h0014 */ Array<void *> h0014;
        /* +h0024 */ uint8_t h0024[0x10];
        /* +h0034 */ Array<void *> h0034;
        /* +h0044 */ Array<void *> h0044;
        /* +h0054 */ uint8_t h0054[0x20];
        /* +h0074 */ wchar_t player_name[20];
        /* +h009C */ uint8_t h009C[0x50];
        /* +h00EC */ Array<void *> h00EC;
        /* +h00FC */ uint8_t h00FC[0x94]; // 40
        /* +h0190 */ uint32_t h0190;
        /* +h0194 */ uint32_t token1; // world id
        /* +h0198 */ uint32_t map_id;
        /* +h019C */ uint32_t is_explorable;
        /* +h01A0 */ uint8_t host[0x18];
        /* +h01B8 */ uint32_t token2; // player id
        /* +h01BC */ uint8_t h01BC[0x64];
        /* +h0220 */ uint32_t district_number;
        /* +h0224 */ uint32_t language;
        /* +h0228 */ uint32_t observe_map_id;
        /* +h022C */ uint32_t current_map_id;
        /* +h0230 */ uint8_t h0230[8];
        /* +h0238 */ Array<ObserverMatch *> observer_matchs;
        /* +h0248 */ uint8_t h0248[0xF0];
        /* +h0338 */ uint32_t *progress_bar;
        /* +h033C */ uint8_t h033C[0x6C];
        /* +h03A8 */ wchar_t player_email[0x40];
    };
}
