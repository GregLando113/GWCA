#ifndef _ENTITIE_GUILD_INC
#define _ENTITIE_GUILD_INC

#include <GWCA\GameContainers\Array.h>

namespace GW {
    struct GHKey { DWORD k[4]; };
	using wchar = wchar_t;

    struct GuildPlayer { // total: 0x5C/92
        /* +h0000 */ void* vtable;
        /* +h0004 */ wchar *name_ptr; // ptr to invitedname, why? dunno
        /* +h0008 */ wchar InvitedName[20]; // name of character that was invited in
        /* +h001C */ wchar CurrentName[20]; // name of character currently being played
        /* +h0030 */ wchar IvitorName[20]; // name of character that invited player
        /* +h0044 */ DWORD InviteTime; // time in ms from game creation ??
        /* +h0048 */ wchar PromoterName[20]; // name of player that last modified rank
    };

    using GuildRoster = Array<GuildPlayer*>;

    struct GuildHistoryEvent {
        DWORD time1; // Guessing one of these is time in ms
        DWORD time2;
        wchar name[0x100]; // Name of added/kicked person, then the adder/kicker, they seem to be in the same array
    };

    using GuildHistory = Array<GuildHistoryEvent*>;

    struct Guild {
        GHKey key;

        inline wchar_t* name() const { return (wchar_t*)((BYTE*)this + 0x30); }
        inline wchar_t* tag() const { return (wchar_t*)((BYTE*)this + 0x80); }

        inline DWORD& factioncount() const { return *(DWORD*)((BYTE*)this + 0x78); }

        inline DWORD& rating() const { return *(DWORD*)((BYTE*)this + 0x70); }
        inline DWORD& rank() const { return *(DWORD*)((BYTE*)this + 0x28); }
        inline DWORD& qualifierpoints() const { return *(DWORD*)((BYTE*)this + 0x7C); }
    };

    using GuildArray = Array<Guild*>;
}

#endif // _ENTITIE_GUILD_INC
