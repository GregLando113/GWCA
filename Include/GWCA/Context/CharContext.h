#pragma once
#include <GWCA/Constants/Maps.h>

#include <GWCA/GameContainers/Array.h>


namespace GW {
    struct ObserverMatch { // total: 0x4C/76
        /* +h0000 */ uint32_t match_id;
        /* +h0004 */ uint32_t match_id_dup;
        /* +h0008 */ uint32_t map_id;
        /* +h000C */ uint32_t age;
        /* +h0010 */ uint32_t h0010[14];
        /* +h0048 */ wchar_t* team_names;
    };
    static_assert(sizeof(ObserverMatch) == 76, "struct ObserverMatch has incorect size");

    struct ProgressBar {
        int     pips;
        uint8_t color[4];      // RGBA
        uint8_t background[4]; // RGBA
        int     unk[7];
        float   progress;      // 0 ... 1
        // possibly more
    };

    struct CharContext { // total: 0x42C

        static CharContext* instance();

        /* +h0000 */ Array<void *> h0000;
        /* +h0010 */ uint32_t h0010;
        /* +h0014 */ Array<void *> h0014;
        /* +h0024 */ uint32_t h0024[4];
        /* +h0034 */ Array<void *> h0034;
        /* +h0044 */ Array<void *> h0044;
        /* +h0054 */ uint32_t h0054[8];
        /* +h0074 */ wchar_t player_name[0x14];
        /* +h009C */ uint32_t h009C[20];
        /* +h00EC */ Array<void *> h00EC;
        /* +h00FC */ uint32_t h00FC[37]; // 40
        /* +h0190 */ uint32_t world_flags;
        /* +h0194 */ uint32_t token1; // world id
        /* +h0198 */ Constants::MapID map_id;
        /* +h019C */ uint32_t is_explorable;
        /* +h01A0 */ uint8_t host[0x18];
        /* +h01B8 */ uint32_t token2; // player id
        /* +h01BC */ uint32_t h01BC[25];
        /* +h0220 */ uint32_t district_number;
        /* +h0224 */ Constants::MapLanguage language;
        /* +h0228 */ Constants::MapID observe_map_id;
        /* +h022C */ Constants::MapID current_map_id;
        /* +h0230 */ uint32_t observe_map_type;
        /* +h0234 */ uint32_t current_map_type;
        /* +h0238 */ Array<ObserverMatch *> observer_matchs;
        /* +h0248 */ uint32_t h0248[22];
        /* +h02a0 */ uint32_t player_flags; // bitwise something
        /* +h02A4 */ PlayerID player_id;
        /* +h02A8 */ uint32_t h02A8[40];
        /* +h0348 */ ProgressBar *progress_bar; // seems to never be nullptr
        /* +h034C */ uint32_t h034C[27];
        /* +h03B8 */ wchar_t player_email[0x40];
    };
    static_assert(sizeof(CharContext) == 0x438, "struct CharContext has incorrect size");
}
