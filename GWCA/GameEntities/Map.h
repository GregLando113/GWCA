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
        /* +h0000 */ DWORD h0000;
        /* +h0004 */ DWORD Continent;
        /* +h0008 */ Region Region;
        /* +h000C */ RegionType Type;
        /* +h0010 */ DWORD Flags;
        /* +h0014 */ DWORD ThumbmailID;
        /* +h0018 */ DWORD PartySize;
        /* +h001C */ DWORD h001C[9];
        /* +h0040 */ DWORD X; // icon position on map.
        /* +h0044 */ DWORD Y;
        /* +h0048 */ DWORD h0048[11];
        /* +h0074 */ DWORD NameID;
        /* +h0078 */ DWORD DescriptionID;

        inline bool GetHasEnterButton() { return (Flags & 0x1000000) != 0; }
    };
}

#endif // _MAP_INC
