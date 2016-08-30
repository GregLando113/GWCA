#pragma once
#include <Windows.h>

namespace GWCA {
  namespace GW {
    struct GHKey { DWORD k[4]; };

		struct GuildPlayer {
			void* vtable;
			wchar_t* name_ptr; // ptr to invitedname, why? dunno
			wchar_t invitedname[20]; // name of character that was invited in
			wchar_t currentname[20]; // name of character currently being played
			wchar_t invitorname[20]; // name of character that invited player
			DWORD invitetime; // time in ms from game creation ??
			wchar_t promotername[20]; // name of player that last modified rank
		};

		using GuildRoster = gw_array<GuildPlayer*>;

		struct GuildHistoryEvent {
			DWORD time1; // Guessing one of these is time in ms
			DWORD time2;
			wchar_t name[0x100]; // Name of added/kicked person, then the adder/kicker, they seem to be in the same array
		};

		using GuildHistory = gw_array<GuildHistoryEvent*>;

		struct Guild {
			GHKey key;

			inline wchar_t* name() const { return (wchar_t*)(this + 0x30); }
			inline wchar_t* tag() const { return (wchar_t*)(this + 0x80); }

			inline DWORD& factioncount() const { return *(DWORD*)(this + 0x78); }

			inline DWORD& rating() const { return *(DWORD*)(this + 0x70); }
			inline DWORD& rank() const { return *(DWORD*)(this + 0x28); }
			inline DWORD& qualifierpoints() const { return *(DWORD*)(this + 0x7C); }
		};

		using GuildArray = gw_array<Guild*>;
  }
}
