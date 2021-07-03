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

    GuildContext *GuildMgr::GetGuildContext() {
        return GameContext::instance()->guild;
    }

    wchar_t *GuildMgr::GetPlayerGuildAnnouncer() {
        return GameContext::instance()->guild->announcement_author;
    }

    wchar_t *GuildMgr::GetPlayerGuildAnnouncement() {
        return GameContext::instance()->guild->announcement;
    }

    uint32_t GuildMgr::GetPlayerGuildIndex() {
        return GameContext::instance()->guild->player_guild_index;
    }

    GuildArray GuildMgr::GetGuildArray() {
        return GameContext::instance()->guild->guilds;
    }

    void GuildMgr::TravelGH() {
        TravelGH(GameContext::instance()->guild->player_gh_key);
    }

    void GuildMgr::TravelGH(GHKey key) {
        UI::SendUIMessage(UI::kGuildHall, &key);
    }

    void GuildMgr::LeaveGH() {
        UI::SendUIMessage(UI::kLeaveGuildHall,nullptr);
    }
} // namespace GW
