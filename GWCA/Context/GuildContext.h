#ifndef _GUILD_CONTEXT_INC
#define _GUILD_CONTEXT_INC

#include <Windows.h>

#include <GWCA\GameEntities\Guild.h>
#include <GWCA\GameContainers\Array.h>

namespace GW {
    using wchar = wchar_t;

    struct GuildContext {
        /* +h0000 */ DWORD h0000;
        /* +h0004 */ DWORD h0004;
        /* +h0008 */ DWORD h0008;
        /* +h000C */ DWORD h000C;
        /* +h0010 */ DWORD h0010;
        /* +h0014 */ DWORD h0014;
        /* +h0018 */ DWORD h0018;
        /* +h001C */ DWORD h001C;
        /* +h0020 */ Array<void*> h0020;
        /* +h0030 */ DWORD h0030;
        /* +h0034 */ wchar PlayerName[20];
        /* +h005C */ DWORD h005C;
        /* +h0060 */ DWORD playerguildindex;
        /* +h0064 */ GHKey playerghkey;
        /* +h0074 */ DWORD h0074;
        /* +h0078 */ wchar Announcement[256];
        /* +h0278 */ wchar AnnouncementAuthor[20];
        /* +h02A0 */ DWORD h02A0;
        /* +h02A4 */ DWORD h02A4;
        /* +h02A8 */ Array<void*> h02A8;
        /* +h02B8 */ Array<void*> h02B8;
        /* +h02C8 */ DWORD h02C8;
        /* +h02CC */ GuildHistory playerguildhistory;
        /* +h02DC */ DWORD h02DC[7];
        /* +h02F8 */ GuildArray guilds;
        /* +h0308 */ DWORD h0308[4];
        /* +h0318 */ Array<void*> h0318;
        /* +h0328 */ DWORD h0328;
        /* +h032C */ Array<void*> h032C;
        /* +h033C */ DWORD h033C[7];
        /* +h0358 */ GuildRoster playerroster;
        //... end of what i care about
    };
}

#endif // _GUILD_CONTEXT_INC
