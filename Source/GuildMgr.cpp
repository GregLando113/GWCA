#include "stdafx.h"

#include <GWCA/Packets/Opcodes.h>
#include <GWCA/Utilities/Export.h>
#include <GWCA/Utilities/Scanner.h>

#include <GWCA/GameEntities/Guild.h>
#include <GWCA/Context/GameContext.h>
#include <GWCA/Context/GuildContext.h>

#include <GWCA/Managers/Module.h>

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
        GHKey guild_uuid = GameContext::instance()->guild->player_gh_key;
        CtoS::SendPacket(0x18, GAME_CMSG_PARTY_ENTER_GUILD_HALL,
            guild_uuid.k[0], guild_uuid.k[1], guild_uuid.k[2], guild_uuid.k[3]);
    }

    void GuildMgr::TravelGH(GHKey key) {
        CtoS::SendPacket(0x18, GAME_CMSG_PARTY_ENTER_GUILD_HALL, key.k[0], key.k[1], key.k[2], key.k[3]);
    }

    void GuildMgr::LeaveGH() {
        CtoS::SendPacket(0x8, GAME_CMSG_PARTY_LEAVE_GUILD_HALL, 0x1);
    }
} // namespace GW
