#ifndef _ENTITIE_MAP_INC
#define _ENTITIE_MAP_INC

#include <GWCA\GameContainers\Array.h>

namespace GW {
    struct MissionMapIcon { // total: 0x28/40
        /* +h0000 */ DWORD index;
        /* +h0004 */ float X;
        /* +h0008 */ float Y;
        /* +h000C */ DWORD h000C; // = 0
        /* +h0010 */ DWORD h0010; // = 0
        /* +h0014 */ DWORD option; // Affilitation/color. gray = 0, blue, red, yellow, teal, purple, green, gray
        /* +h0018 */ DWORD h0018; // = 0
        /* +h001C */ DWORD modelId; // Model of the displayed icon in the Minimap
        /* +h0020 */ DWORD h0020; // = 0
        /* +h0024 */ DWORD h0024; // May concern the name
    };

    using MissionMapIconArray = Array<MissionMapIcon>;

    enum class RegionType {
        AllianceBattle,
        Arena,
        ExplorableZone,
        GuildBattleArea,
        GuildHall,
        MissionOutpost,
        CooperativeMission,
        CompetitiveMission,
        EliteMission,
        Challenge,
        Outpost,
        ZaishenBattle,
        HeroesAscent,
        City,
        MissionArea,
        HeroBattleOutpost,
        HeroBattleArea,
        EotnMission,
        Dungeon,
        Marketplace,
        Unknown,
        DevRegion
    };

    enum class Region {
        Kryta,
        Maguuma,
        Ascalon,
        NorthernShiverpeaks,
        HeroesAscent,
        CrystalDesert,
        FissureOfWoe,
        Presearing,
        Kaineng,
        Kurzick,
        Luxon,
        ShingJea,
        Kourna,
        Vaabi,
        Desolation,
        Istan,
        DomainOfAnguish,
        TarnishedCoast,
        DepthsOfTyria,
        FarShiverpeaks,
        CharrHomelands,
        BattleIslands,
        TheBattleOfJahai,
        TheFlightNorth,
        TheTenguAccords,
        TheRiseOfTheWhiteMantle,
        Swat,
        DevRegion
    };

    // https://github.com/entice/gw-interface/blob/master/GuildWarsInterface/Datastructures/Const/AreaInfo.cs
    struct AreaInfo { // total: 0x7C/120
        /* +h0000 */ DWORD Campaign;
        /* +h0004 */ DWORD Continent;
        /* +h0008 */ Region Region;
        /* +h000C */ RegionType Type;
        /* +h0010 */ DWORD Flags;
        /* +h0014 */ DWORD ThumbmailID;
        /* +h0018 */ DWORD MinPartySize;
        /* +h001C */ DWORD MaxPartySize;
        /* +h0020 */ DWORD MinPlayerSize;
        /* +h0024 */ DWORD MaxPlayerSize;
        /* +h0028 */ DWORD ControlledOutpostID;
        /* +h002C */ DWORD FractionMission;
        /* +h0030 */ DWORD MinLevel;
        /* +h0034 */ DWORD MaxLevel;
        /* +h0038 */ DWORD NeededPQ;
        /* +h003C */ DWORD MissionMapsTo;
        /* +h0040 */ DWORD X; // icon position on map.
        /* +h0044 */ DWORD Y;
        /* +h0048 */ DWORD IconStart_X;
        /* +h004C */ DWORD IconStart_Y;
        /* +h0050 */ DWORD IconEnd_X;
        /* +h0054 */ DWORD IconEnd_Y;
        /* +h0058 */ DWORD IconStart_X_dupe;
        /* +h005C */ DWORD IconStart_Y_dupe;
        /* +h0060 */ DWORD IconEnd_X_dupe;
        /* +h0064 */ DWORD IconEnd_Y_dupe;
        /* +h0068 */ DWORD h0068;
        /* +h006C */ DWORD MissionChronology;
        /* +h0070 */ DWORD HAMapChronology;
        /* +h0074 */ DWORD NameID;
        /* +h0078 */ DWORD DescriptionID;

        inline bool GetHasEnterButton() { return (Flags & 0x1000000) != 0; }
    };
}

#endif // _MAP_INC
