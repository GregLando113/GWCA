#include "stdafx.h"

#include <GWCA/CtoSHeaders.h>
#include <GWCA/Constants/Constants.h>

#include <GWCA/Utilities/Export.h>
#include <GWCA/Utilities/Macros.h>
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

#include <GWCA/Managers/Module.h>

#include <GWCA/Managers/MapMgr.h>
#include <GWCA/Managers/CtoSMgr.h>
#include <GWCA/Managers/MemoryMgr.h>

namespace {
    using namespace GW;

    uintptr_t map_id_addr;
    uintptr_t map_info_addr;
    uintptr_t area_info_addr;

    void Init() {
        {
            uintptr_t address = GW::Scanner::Find("\xC3\x8B\x75\xFC\x8B\x04\xB5", "xxxxxxx", 7);
            printf("[SCAN] map_info_addr = %p\n", (void *)address);
            if (Verify(address))
                map_info_addr = *(uintptr_t *)(address);
        }

        {
            uintptr_t address = Scanner::Find(
                "\x8B\xC6\xC1\xE0\x05\x2B\xC6\x5E\x8D\x04", "xxxxxxxxxx", 11);
            printf("[SCAN] area_info_addr = %p\n", (void *)address);
            area_info_addr = *(uintptr_t *)address;
        }

        {
            // For Map IDs
            uintptr_t address = Scanner::Find("\xB0\x7F\x8D\x55", "xxxx", 0x46);
            printf("[SCAN] MapIDPtr = %p\n", (void *)address);
            if (Verify(address))
                map_id_addr = *(uintptr_t *)address;
        }
    }
}

namespace GW {

    Module MapModule {
        "MapModule",    // name
        NULL,           // param
        ::Init,         // init_module
        NULL,           // exit_module
        NULL,           // exit_module
        NULL,           // remove_hooks
    };

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
        return (Constants::MapID)(*(uint32_t *)map_id_addr);
    }

    int Map::GetRegion() {
        return *(int32_t *)(map_info_addr + 0x10);
    }

    int Map::GetLanguage() {
        return *(int32_t *)(map_info_addr + 0xC);
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
        AreaInfo *infos = (AreaInfo *)area_info_addr;
        if (Verify(infos))
            return &infos[(uint32_t)map_id];
        else
            return NULL;
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
