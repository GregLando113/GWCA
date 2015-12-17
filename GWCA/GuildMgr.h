#pragma once

#include <Windows.h>

#include "GWCAManager.h"
#include "GWStructures.h"

namespace GWAPI {

	class GuildMgr : public GWCAManager {
		friend class GWAPIMgr;

	public:

		// Array of guilds, holds basically everything about a guild. Can get structs of all players in outpost ;)
		GW::GuildArray GetGuildArray();

		// Index in guild array of player guild.
		DWORD GetPlayerGuildIndex();

		// Announcement in guild at the moment.
		wchar_t* GetPlayerGuildAnnouncement();

		// Name of player who last edited the announcement.
		wchar_t* GetPlayerGuildAnnouncer();

		void TravelGH();
		
		void TravelGH(DWORD* key);

		void LeaveGH();

	private:
		GuildMgr(GWAPIMgr& api) : GWCAManager(api) {}
		void RestoreHooks() override {}
	};
}