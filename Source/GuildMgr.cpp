#include "stdafx.h"

#include <GWCA/Packets/Opcodes.h>
#include <GWCA/Utilities/Export.h>
#include <GWCA/Utilities/Scanner.h>

#include <GWCA/GameEntities/Guild.h>
#include <GWCA/Context/GameContext.h>
#include <GWCA/Context/GuildContext.h>

#include <GWCA/Managers/Module.h>
#include <GWCA/Managers/UIMgr.h>

#include <GWCA/Managers/CtoSMgr.h>
#include <GWCA/Managers/GuildMgr.h>

namespace GW {

    Module GuildModule {
        "GuildModule",  // name
        NULL,           // param
        NULL,           // init_module
        NULL,           // exit_module
        NULL,           // exit_module
        NULL,           // remove_hooks
    };

    wchar_t *GuildMgr::GetPlayerGuildAnnouncer() {
        auto* g = GuildContext::instance();
        return g ? g->announcement_author : nullptr;
    }

    wchar_t *GuildMgr::GetPlayerGuildAnnouncement() {
        auto* g = GuildContext::instance();
        return g ? g->announcement : nullptr;
    }

    uint32_t GuildMgr::GetPlayerGuildIndex() {
        return GuildContext::instance()->player_guild_index;
    }

    GuildArray* GuildMgr::GetGuildArray() {
        auto* g = GuildContext::instance();
        return g && g->guilds.valid() ? &g->guilds : nullptr;
    }

    void GuildMgr::TravelGH() {
        TravelGH(GuildContext::instance()->player_gh_key);
    }

    void GuildMgr::TravelGH(GHKey key) {
        UI::SendUIMessage(UI::UIMessage::kGuildHall, &key);
    }

    void GuildMgr::LeaveGH() {
        UI::SendUIMessage(UI::UIMessage::kLeaveGuildHall,nullptr);
    }
} // namespace GW
