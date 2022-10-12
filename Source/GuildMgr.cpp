#include "stdafx.h"


#include <GWCA/GameEntities/Guild.h>
#include <GWCA/GameEntities/Map.h>

#include <GWCA/Context/GuildContext.h>

#include <GWCA/Managers/Module.h>
#include <GWCA/Managers/UIMgr.h>
#include <GWCA/Managers/MapMgr.h>

#include <GWCA/Managers/GuildMgr.h>

namespace GW {

    Module GuildModule{
        "GuildModule",  // name
        NULL,           // param
        NULL,           // init_module
        NULL,           // exit_module
        NULL,           // exit_module
        NULL,           // remove_hooks
    };
    namespace GuildMgr {
        wchar_t* GetPlayerGuildAnnouncer() {
            auto* g = GetGuildContext();
            return g ? g->announcement_author : nullptr;
        }

        wchar_t* GetPlayerGuildAnnouncement() {
            auto* g = GetGuildContext();
            return g ? g->announcement : nullptr;
        }

        uint32_t GetPlayerGuildIndex() {
            auto* g = GetGuildContext();
            return g ? g->player_guild_index : 0;
        }

        GuildArray* GetGuildArray() {
            auto* g = GetGuildContext();
            return g && g->guilds.valid() ? &g->guilds : nullptr;
        }
        Guild* GetPlayerGuild() {
            return GetGuildInfo(GetPlayerGuildIndex());
        }

        Guild* GetCurrentGH() {
            AreaInfo* m = Map::GetCurrentMapInfo();
            if (!m || m->type != GW::RegionType::GuildHall) return nullptr;
            GW::Array<GW::Guild*>* guilds = GW::GuildMgr::GetGuildArray();
            if (!guilds) return nullptr;
            for (auto* guild : *guilds) {
                if (guild) return guild;
            }
            return nullptr;
        }

        Guild* GetGuildInfo(uint32_t guild_id) {
            auto* g = GetGuildArray();
            return g && guild_id < g->size() ? g->at(guild_id) : nullptr;
        }

        bool TravelGH() {
            auto* g = GetGuildContext();
            return g ? TravelGH(g->player_gh_key) : false;

        }

        bool TravelGH(GHKey key) {
            return UI::SendUIMessage(UI::UIMessage::kGuildHall, &key);
        }

        bool LeaveGH() {
            return UI::SendUIMessage(UI::UIMessage::kLeaveGuildHall);
        }
    }
} // namespace GW
