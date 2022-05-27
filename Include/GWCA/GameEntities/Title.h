#pragma once
#include <GWCA/Constants/Constants.h>

#include <GWCA/GameContainers/Array.h>

namespace GW {
    struct Title { // total: 0x28/40
        /* +h0000 */ uint32_t props;
        /* +h0004 */ uint32_t current_points;
        /* +h0008 */ uint32_t current_title_tier_index;
        /* +h000C */ uint32_t points_needed_current_rank;
        /* +h0010 */ uint32_t next_title_tier_index;
        /* +h0014 */ uint32_t points_needed_next_rank;
        /* +h0018 */ uint32_t max_title_rank;
        /* +h001C */ uint32_t max_title_tier_index;
        /* +h0020 */ wchar_t* h0020; // Pretty sure these are ptrs to title hash strings
        /* +h0024 */ wchar_t* h0024; // Pretty sure these are ptrs to title hash strings

        inline bool is_percentage_based() { return (props & 1) != 0; };
        inline bool has_tiers() { return (props & 3) == 2; };

        uint32_t name_id();
        Constants::TitleID title_id();

    };
    static_assert(sizeof(Title) == 40, "struct Title has incorect size");

    struct TitleTier {
        uint32_t props;
        uint32_t tier_number;
        wchar_t* tier_name_enc;
        inline bool is_percentage_based() { return (props & 1) != 0; };
    };
    static_assert(sizeof(TitleTier) == 0xc, "struct TitleTier has incorect size");

    typedef Array<Title> TitleArray;
}
