#include "stdafx.h"

#include <GWCA/CtoSHeaders.h>
#include <GWCA/Constants/Constants.h>

#include <GWCA/Utilities/Export.h>
#include <GWCA/Utilities/Scanner.h>

#include <GWCA/GameContainers/Vector.h>

#include <GWCA/GameEntities/Map.h>
#include <GWCA/GameEntities/Pathing.h>

#include <GWCA/Context/Cinematic.h>
#include <GWCA/Context/MapContext.h>
#include <GWCA/Context/CharContext.h>
#include <GWCA/Context/GameContext.h>
#include <GWCA/Context/AgentContext.h>
#include <GWCA/Context/WorldContext.h>

#include <GWCA/Managers/MapMgr.h>
#include <GWCA/Managers/CtoSMgr.h>
#include <GWCA/Managers/MemoryMgr.h>

namespace {
    uintptr_t GetMapInfoPtr() {
        static uintptr_t MapInfoPtr = 0;
        if (!MapInfoPtr) {
            MapInfoPtr = GW::Scanner::Find("\xC3\x8B\x75\xFC\x8B\x04\xB5", "xxxxxxx", 0);
            if (MapInfoPtr) {
                printf("[SCAN] MapInfoPtr = %08lX\n", MapInfoPtr);
                MapInfoPtr = *(uintptr_t *)(MapInfoPtr + 7);
            }
        }
        return MapInfoPtr;
    }
}

namespace GW {
    bool Map::GetIsMapLoaded() {
        return GameContext::instance()->map != nullptr;
    }

    void Map::Travel(Constants::MapID map_id,
        int district, int region, int language) {

        struct ZoneMap {
            uint32_t header = CtoGS_MSGTravelTo;
            uint32_t mapid;
            int region;
            int district;
            int language;
            uint32_t unk;
        };

        if (GetInstanceType() == Constants::InstanceType::Loading)
            return;

        ZoneMap pak;
        pak.mapid = static_cast<uint32_t>(map_id);
        pak.district = district;
        pak.region = region;
        pak.language = language;
        pak.unk = 0;

        CtoS::SendPacket<ZoneMap>(&pak);
    }

    void Map::Travel(Constants::MapID map_id, Constants::District district, int district_number) {
        switch (district) {
        case Constants::District::Current:
            Travel(map_id, district_number, GetRegion(), GetLanguage());
            break;
        case Constants::District::International:
            Travel(map_id, district_number, Constants::Region::International, 0);
            break;
        case Constants::District::American:
            Travel(map_id, district_number, Constants::Region::America, 0);
            break;
        case Constants::District::EuropeEnglish:
            Travel(map_id, district_number, Constants::Region::Europe, Constants::EuropeLanguage::English);
            break;
        case Constants::District::EuropeFrench:
            Travel(map_id, district_number, Constants::Region::Europe, Constants::EuropeLanguage::French);
            break;
        case Constants::District::EuropeGerman:
            Travel(map_id, district_number, Constants::Region::Europe, Constants::EuropeLanguage::German);
            break;
        case Constants::District::EuropeItalian:
            Travel(map_id, district_number, Constants::Region::Europe, Constants::EuropeLanguage::Italian);
            break;
        case Constants::District::EuropeSpanish:
            Travel(map_id, district_number, Constants::Region::Europe, Constants::EuropeLanguage::Spanish);
            break;
        case Constants::District::EuropePolish:
            Travel(map_id, district_number, Constants::Region::Europe, Constants::EuropeLanguage::Polish);
            break;
        case Constants::District::EuropeRussian:
            Travel(map_id, district_number, Constants::Region::Europe, Constants::EuropeLanguage::Russian);
            break;
        case Constants::District::AsiaKorean:
            Travel(map_id, district_number, Constants::Region::Korea, 0);
            break;
        case Constants::District::AsiaChinese:
            Travel(map_id, district_number, Constants::Region::China, 0);
            break;
        case Constants::District::AsiaJapanese:
            Travel(map_id, district_number, Constants::Region::Japan, 0);
            break;
        }
    }

    uint32_t Map::GetInstanceTime() {
        return GameContext::instance()->agent->instance_timer;
    }

    Constants::MapID Map::GetMapID() {
        static uint32_t* map_id_ptr = nullptr;
        if (map_id_ptr == nullptr) {
            // For Map IDs
            uintptr_t addr = Scanner::Find("\xB0\x7F\x8D\x55", "xxxx", 0);
            printf("[SCAN] MapIDPtr = %08lX\n", addr);
            if (addr) {
                map_id_ptr = *(uint32_t **)(addr + 0x46);
            }
        }
        return (Constants::MapID)(*map_id_ptr);
    }

    int Map::GetRegion() {
        return *(int *)(GetMapInfoPtr() + 0x10);
    }

    int Map::GetLanguage() {
        return *(int *)(GetMapInfoPtr() + 0xC);
    }

    int Map::GetDistrict() {
        CharContext *ctx = GameContext::instance()->character;
        if (!ctx) return 0;
        return ctx->district_number;
    }

    Constants::InstanceType Map::GetInstanceType() {
        return *(Constants::InstanceType *)(MemoryMgr::AgentArrayPtr - 0xF0);
    }

    MissionMapIconArray Map::GetMissionMapIconArray() {
        return GameContext::instance()->world->mission_map_icons;
    }

    PathingMapArray Map::GetPathingMap() {
        return GameContext::instance()->map->sub1->sub2->pmaps;
    }

    uint32_t Map::GetFoesKilled() {
        return GameContext::instance()->world->foes_killed;
    }

    uint32_t Map::GetFoesToKill() {
        return GameContext::instance()->world->foes_to_kill;
    }

    AreaInfo *Map::GetMapInfo(Constants::MapID map_id) {
        static AreaInfo *infos = nullptr;
        if (!infos) {
            uintptr_t tmp = Scanner::Find(
                "\x8B\xC6\xC1\xE0\x05\x2B\xC6\x5E\x8D\x04", "xxxxxxxxxx", 11);
            printf("[SCAN] AreaInfoPtr = %08lX\n", tmp);
            infos = *(AreaInfo **)tmp;
        }
        return &infos[(uint32_t)map_id];
    }

    bool Map::GetIsInCinematic(void) {
        GameContext *game_ctx = GameContext().instance();
        if (!(game_ctx && game_ctx->cinematic))
            return false;
        return (game_ctx->cinematic->h0004 != 0);
    }

    void Map::SkipCinematic(void) {
        CtoS::SendPacket(4, CtoGS_MSGSkipCinematic);
    }
} // namespace GW
