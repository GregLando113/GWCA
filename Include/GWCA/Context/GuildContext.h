#pragma once
#include <GWCA/GameEntities/Guild.h>

namespace GW {

    typedef Array<Guild *> GuildArray;
    typedef Array<GuildPlayer *> GuildRoster;
    typedef Array<GuildHistoryEvent *> GuildHistory;

    struct GuildContext {

        static GuildContext* instance();

        /* +h0000 */ uint32_t h0000;
        /* +h0004 */ uint32_t h0004;
        /* +h0008 */ uint32_t h0008;
        /* +h000C */ uint32_t h000C;
        /* +h0010 */ uint32_t h0010;
        /* +h0014 */ uint32_t h0014;
        /* +h0018 */ uint32_t h0018;
        /* +h001C */ uint32_t h001C;
        /* +h0020 */ Array<void *> h0020;
        /* +h0030 */ uint32_t h0030;
        /* +h0034 */ wchar_t player_name[20];
        /* +h005C */ uint32_t h005C;
        /* +h0060 */ uint32_t player_guild_index;
        /* +h0064 */ GHKey player_gh_key;
        /* +h0074 */ uint32_t h0074;
        /* +h0078 */ wchar_t announcement[256];
        /* +h0278 */ wchar_t announcement_author[20];
        /* +h02A0 */ uint32_t h02A0;
        /* +h02A4 */ uint32_t h02A4;
        /* +h02A8 */ Array<void *> factions_outpost_guilds;
        /* +h02B8 */ Array<void *> h02B8;
        /* +h02C8 */ uint32_t h02C8;
        /* +h02CC */ GuildHistory player_guild_history;
        /* +h02DC */ uint32_t h02DC[7];
        /* +h02F8 */ GuildArray guilds;
        /* +h0308 */ uint32_t h0308[4];
        /* +h0318 */ Array<void *> h0318;
        /* +h0328 */ uint32_t h0328;
        /* +h032C */ Array<void *> h032C;
        /* +h033C */ uint32_t h033C[7];
        /* +h0358 */ GuildRoster player_roster;
        //... end of what i care about


    };
}
