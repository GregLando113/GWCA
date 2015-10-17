#pragma once

#include <Windows.h>

#include "GWStructures.h"

namespace GWAPI {

	class GuildMgr {

	public:

		// Array of guilds, holds basically everything about a guild. Can get structs of all players in outpost ;)
		GW::GuildArray GetGuildArray();

		// Index in guild array of player guild.
		DWORD GetPlayerGuildIndex();

		// Announcement in guild at the moment.
		wchar_t* GetPlayerGuildAnnouncement();

		// Name of player who last edited the announcement.
		wchar_t* GetPlayerGuildAnnouncer();

	private:
		friend class GWAPIMgr;
		GWAPIMgr* const parent_;
		GuildMgr(GWAPIMgr* obj) : parent_(obj) {}
	};
}