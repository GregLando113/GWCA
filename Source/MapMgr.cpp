#include "stdafx.h"

#include <GWCA/Packets/Opcodes.h>
#include <GWCA/Constants/Constants.h>

#include <GWCA/Utilities/Debug.h>
#include <GWCA/Utilities/Export.h>
#include <GWCA/Utilities/Macros.h>
#include <GWCA/Utilities/Scanner.h>

#include <GWCA/GameContainers/GamePos.h>

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

namespace {
    using namespace GW;

    uintptr_t map_id_addr;
    uintptr_t map_info_addr;
    uintptr_t area_info_addr;
    uintptr_t instance_type_addr;

    void Init() {
        {
            uintptr_t address = GW::Scanner::Find("\x8B\xF0\xEB\x03\x8B\x75\x0C\x3B", "xxxxxxxx", +0xA);
            GWCA_INFO("[SCAN] map_info_addr = %p\n", (void *)address);
            if (Verify(address))
                map_info_addr = *(uintptr_t *)(address);
        }

        {
            uintptr_t address = Scanner::Find(
                "\x6B\xC6\x7C\x5E\x05", "xxxxx", 5);
            GWCA_INFO("[SCAN] area_info_addr = %p\n", (void *)address);
            area_info_addr = *(uintptr_t *)address;
        }

        {
            uintptr_t address = Scanner::Find("\xE8\x00\x00\x00\x00\x6A\x3D\x57\xE8", "x????xxxx", -4);
            GWCA_INFO("[SCAN] map_id_addr = %p\n", (void *)address);
            if (Verify(address))
                map_id_addr = *(uintptr_t *)address;
        }

        {
            uintptr_t address = Scanner::Find(
                "\x6A\x2C\x50\xE8\x00\x00\x00\x00\x83\xC4\x08\xC7", "xxxx????xxxx", +0x17);
            GWCA_INFO("[SCAN] instance_type_addr = %p\n", (void *)address);
            if (Verify(address))
                instance_type_addr = *(uintptr_t *)address;
        }
    }
}

namespace GW {

    Module MapModule {
        "MapModule",    // name
        NULL,           // param
        ::Init,         // init_module
        NULL,           // exit_module
        NULL,           // enable_hooks
        NULL,           // disable_hooks
    };

    bool Map::GetIsMapLoaded() {
        return GameContext::instance()->map != nullptr;
    }

    void Map::Travel(Constants::MapID map_id,
        int district, int region, int language) {

        struct ZoneMap {
            uint32_t header = GAME_CMSG_PARTY_TRAVEL;
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

    bool Map::GetIsMapUnlocked(Constants::MapID map_id) {
        const Array<uint32_t>& unlocked_map = GW::GameContext::instance()->world->unlocked_map;
        if (!unlocked_map.valid())
            return false;
        uint32_t real_index = (uint32_t)map_id / 32;
        if (real_index >= unlocked_map.size())
            return false;
        uint32_t shift = (uint32_t)map_id % 32;
        uint32_t flag = 1u << shift;
        return (unlocked_map[real_index] & flag) != 0;
    }

    int Map::GetLanguage() {
        return *(int32_t *)(map_info_addr + 0xC);
    }

    bool Map::GetIsObserving() {
        CharContext *ctx = GameContext::instance()->character;
        if (!ctx) return false;
        return ctx->current_map_id != ctx->observe_map_id;
    }

    int Map::GetDistrict() {
        CharContext *ctx = GameContext::instance()->character;
        if (!ctx) return 0;
        return static_cast<int>(ctx->district_number);
    }

    Constants::InstanceType Map::GetInstanceType() {
        return *(Constants::InstanceType *)instance_type_addr;
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
        CtoS::SendPacket(4, GAME_CMSG_SKIP_CINEMATIC);
    }

    void Map::EnterChallenge() {
        CtoS::SendPacket(8, GAME_CMSG_PARTY_ENTER_CHALLENGE, 0);
    }
} // namespace GW
