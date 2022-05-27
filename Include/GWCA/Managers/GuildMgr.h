#pragma once

#include <GWCA/Utilities/Export.h>

namespace GW {
    struct Guild;
    struct GuildContext;

    typedef Array<Guild *> GuildArray;

    // @Cleanup: @Fix: This should be replaced by an UUID type
    struct GHKey;

    struct Module;
    extern Module GuildModule;

    namespace GuildMgr {

        // Array of guilds, holds basically everything about a guild. Can get structs of all players in outpost ;)
        GWCA_API GuildArray* GetGuildArray();

        // Index in guild array of player guild.
        GWCA_API uint32_t GetPlayerGuildIndex();

        // Announcement in guild at the moment.
        GWCA_API wchar_t *GetPlayerGuildAnnouncement();

        // Name of player who last edited the announcement.
        GWCA_API wchar_t *GetPlayerGuildAnnouncer();

        GWCA_API void TravelGH();

        GWCA_API void TravelGH(GHKey key);

        GWCA_API void LeaveGH();
    };
}
