#pragma once

#include <GWCA/GameContainers/Array.h>

namespace GW {
    namespace Constants {
        enum class MapRegion;
        enum class RegionType;
    }
    struct MissionMapIcon { // total: 0x28/40
        /* +h0000 */ uint32_t index;
        /* +h0004 */ float X;
        /* +h0008 */ float Y;
        /* +h000C */ uint32_t h000C; // = 0
        /* +h0010 */ uint32_t h0010; // = 0
        /* +h0014 */ uint32_t option; // Affilitation/color. gray = 0, blue, red, yellow, teal, purple, green, gray
        /* +h0018 */ uint32_t h0018; // = 0
        /* +h001C */ uint32_t model_id; // Model of the displayed icon in the Minimap
        /* +h0020 */ uint32_t h0020; // = 0
        /* +h0024 */ uint32_t h0024; // May concern the name
    };
    static_assert(sizeof(MissionMapIcon) == 40, "struct MissionMapIcon has incorect size");

    typedef Array<MissionMapIcon> MissionMapIconArray;

    // https://github.com/entice/gw-interface/blob/master/GuildWarsInterface/Datastructures/Const/AreaInfo.cs
    struct AreaInfo { // total: 0x7C/124
        /* +h0000 */ uint32_t campaign;
        /* +h0004 */ uint32_t continent;
        /* +h0008 */ Constants::MapRegion region;
        /* +h000C */ Constants::RegionType type;
        /* +h0010 */ uint32_t flags;
        /* +h0014 */ uint32_t thumbnail_id;
        /* +h0018 */ uint32_t min_party_size;
        /* +h001C */ uint32_t max_party_size;
        /* +h0020 */ uint32_t min_player_size;
        /* +h0024 */ uint32_t max_player_size;
        /* +h0028 */ uint32_t controlled_outpost_id;
        /* +h002C */ uint32_t fraction_mission;
        /* +h0030 */ uint32_t min_level;
        /* +h0034 */ uint32_t max_level;
        /* +h0038 */ uint32_t needed_pq;
        /* +h003C */ uint32_t mission_maps_to;
        /* +h0040 */ uint32_t x; // icon position on map.
        /* +h0044 */ uint32_t y;
        /* +h0048 */ uint32_t icon_start_x;
        /* +h004C */ uint32_t icon_start_y;
        /* +h0050 */ uint32_t icon_end_x;
        /* +h0054 */ uint32_t icon_end_y;
        /* +h0058 */ uint32_t icon_start_x_dupe;
        /* +h005C */ uint32_t icon_start_y_dupe;
        /* +h0060 */ uint32_t icon_end_x_dupe;
        /* +h0064 */ uint32_t icon_end_y_dupe;
        /* +h0068 */ uint32_t h0068;
        /* +h006C */ uint32_t mission_chronology;
        /* +h0070 */ uint32_t ha_map_chronology;
        /* +h0074 */ uint32_t name_id;
        /* +h0078 */ uint32_t description_id;

        inline bool GetHasEnterButton() const { return (flags & 0x100) != 0 || (flags & 0x40000) != 0; }
        inline bool GetIsOnWorldMap()   const { return (flags & 0x20) == 0; }
        inline bool GetIsPvP()          const { return (flags & 0x1) != 0; }
        inline bool GetIsGuildHall()    const { return (flags & 0x800000) != 0; }
    };
    static_assert(sizeof(AreaInfo) == 124, "struct AreaInfo has incorect size");
}
