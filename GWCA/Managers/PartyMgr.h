#pragma once

#include "GWCAManager.h"
#include <GWCA\Utilities\Hooker.h>
#include <GWCA\Context\GameContext.h>
#include <GWCA\Context\PartyContext.h>


namespace GW {
	class PartyMgr : public GWCAManager<PartyMgr> {
		friend class GWCAManager<PartyMgr>;

	public:
		GW::PartyInfo* GetPartyInfo() {
			return GameContext::instance()->party->partyinfo;
		}

		DWORD GetPartySize() {
			GW::PartyInfo* info = GetPartyInfo();
			if (info == nullptr) return 0;
			return info->players.size() + info->heroes.size() + info->henchmen.size();
		}

		DWORD GetPartyPlayerCount() {
			if (GetPartyInfo()) {
				return GetPartyInfo()->players.size();
			} else {
				return 0;
			}
		}
		DWORD GetPartyHeroCount() {
			if (GetPartyInfo()) {
				return GetPartyInfo()->heroes.size();
			} else {
				return 0;
			}
		}
		DWORD GetPartyHenchmanCount() {
			if (GetPartyInfo()) {
				return GetPartyInfo()->henchmen.size();
			} else {
				return 0;
			}
		}

		bool GetIsPartyInHardMode() {
			return GameContext::instance()->party->partystate.InHardMode();
		}
		bool GetIsPartyDefeated() {
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
