#pragma once

#include <GWCA\GameEntities\Position.h>
#include <GWCA\GameEntities\Party.h>

namespace GW {
	namespace PartyMgr {

		// set or unset the fact that ticking will work as a toggle instead
		// of showing a drop-down menu
		void SetTickToggle();
		void RestoreTickToggle();

		inline void RestoreHooks() { RestoreTickToggle(); }

		// Set party ready status.
		void Tick(bool flag = true);

		GW::PartyInfo* GetPartyInfo();

		DWORD GetPartySize();
		DWORD GetPartyPlayerCount();
		DWORD GetPartyHeroCount();
		DWORD GetPartyHenchmanCount();

		bool GetIsPartyDefeated();

		void SetHardMode(bool flag);
		bool GetIsPartyInHardMode();

		// check if the whole party is ticked
		bool GetIsPartyTicked();

		// check if selected party member is ticked
		bool GetIsTicked(DWORD player_index);

		// check if the player is ticked
		bool GetIsPlayerTicked();

		// check if the whole party is loaded
		bool GetIsPartyLoaded();

        void RespondToPartyRequest(bool accept);

		void LeaveParty();

		// hero managment
		void AddHero(DWORD heroid);
		void KickHero(DWORD heroid);
		void KickAllHeroes();

		// hero flagging
		void FlagHero(DWORD hero_index, GW::GamePos pos);
		void UnflagHero(DWORD hero_index);

		void FlagAll(GW::GamePos pos);
		void UnflagAll();
	};
}
