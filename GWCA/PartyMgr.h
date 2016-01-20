#pragma once

#include "GWCAManager.h"
#include "GameContext.h"
#include "Hooker.h"

namespace GWCA {
	class PartyMgr : public GWCAManager<PartyMgr> {
		friend class GWCAManager<PartyMgr>;
		
	public:
		GW::PartyInfo* GetPartyInfo() { 
			return GameContext::instance()->party->partyinfo;
		}

		GW::PlayerPartyMemberArray& GetPlayerPartyArray() {
			return GetPartyInfo()->players;
		}
		GW::HeroPartyMemberArray& GetHeroPartyArray() {
			return GetPartyInfo()->heroes;
		}
		GW::HenchmanPartyMemberArray& GetHenchPartyArray() {
			return GetPartyInfo()->henchmen;
		}

		DWORD GetPartySize() {
			GW::PartyInfo* info = GetPartyInfo();
			return info->players.size() + info->heroes.size() + info->henchmen.size();
		}

		DWORD GetPartyPlayerCount() {
			return GetPlayerPartyArray().size();
		}
		DWORD GetPartyHeroCount() {
			return GetHeroPartyArray().size();
		}
		DWORD GetPartyHenchmanCount() {
			return GetHenchPartyArray().size();
		}

		bool GetIsPartyInHardMode(){
			return GameContext::instance()->party->partystate.InHardMode();
		}
		bool GetIsPartyDefeated(){
			return GameContext::instance()->party->partystate.IsDefeated();
		}

		// Set party ready status.
		void Tick(bool flag);

		// Ticks
		inline void Tick() { Tick(true); }

		// check if the whole party is ticked
		bool GetIsPartyTicked();

		// check if selected party member is ticked
		bool GetIsTicked(DWORD player_index);

		// check if the player is ticked
		bool GetIsPlayerTicked();

		// check if the whole party is loaded
		bool GetIsPartyLoaded();

	private:
		PartyMgr();

		void RestoreHooks() override;

		typedef DWORD(__stdcall *Tick_t)(DWORD unk1);

		Tick_t ori_tick_;

		Hook hk_tick_;

		// Parameter is always 1 or 2 creating "Ready" or "Not ready"
		static DWORD __stdcall DetourTick(DWORD unk1);
	};
}
