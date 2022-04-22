#pragma once

#include <stdint.h>

#include <GWCA/GameContainers/Array.h>

namespace GW {
    struct Title { // total: 0x28/40
        /* +h0000 */ uint32_t h0000;
        /* +h0004 */ uint32_t current_points;
        /* +h0008 */ uint32_t title_tier_index;
        /* +h000C */ uint32_t points_needed_current_rank;
        /* +h0010 */ uint32_t h0010;
        /* +h0014 */ uint32_t points_needed_next_rank;
        /* +h0018 */ uint32_t max_title_rank;
        /* +h001C */ uint32_t h001C;
        /* +h0020 */ wchar_t* h0020; // Pretty sure these are ptrs to title hash strings
        /* +h0024 */ wchar_t* h0024; // Pretty sure these are ptrs to title hash strings
    };
    static_assert(sizeof(Title) == 40, "struct Title has incorect size");

    struct TitleTier {
        uint32_t title_id;
        uint32_t tier_number;
        wchar_t* tier_name_enc;
    };
    static_assert(sizeof(TitleTier) == 0xc, "struct TitleTier has incorect size");

    typedef Array<Title> TitleArray;
}
