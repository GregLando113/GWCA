#pragma once

namespace GW {

    namespace GuildMgr {

        GWCA_API GW::GuildContext* GetGuildContext();

        // Array of guilds, holds basically everything about a guild. Can get structs of all players in outpost ;)
        GWCA_API GW::GuildArray GetGuildArray();

        // Index in guild array of player guild.
        GWCA_API DWORD GetPlayerGuildIndex();

        // Announcement in guild at the moment.
        GWCA_API wchar_t* GetPlayerGuildAnnouncement();

        // Name of player who last edited the announcement.
        GWCA_API wchar_t* GetPlayerGuildAnnouncer();

        GWCA_API void TravelGH();

        GWCA_API void TravelGH(GW::GHKey key);

        GWCA_API void LeaveGH();
    };
}
