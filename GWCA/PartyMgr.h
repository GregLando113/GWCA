#pragma once

#include "GWCAManager.h"
#include "GameContext.h"

namespace GWCA {
	class PartyMgr : public GWCAManager<PartyMgr> {
		friend class GWCAManager<PartyMgr>;
		
	public:
		GW::PartyInfo* GetPartyInfo() { 
			return GameContext::instance()->party->partyinfo;
		}

		GW::PartyMemberArray& GetPlayerPartyArray() {
			return GetPartyInfo()->players;
		}
		GW::HeroPartyMemberArray& GetHeroPartyArray() {
			return GetPartyInfo()->heroes;
		}
		GW::HenchmanPartyMemberArray& GetHenchPartyArray() {
			return GetPartyInfo()->henchmen;
		}

		DWORD GetPartyMemberCount() {
			GW::PartyInfo* info = GetPartyInfo();
			return info->players.size() + info->heroes.size() + info->henchmen.size();
		}

		DWORD GetPlayerCountInParty() {
			return GetPlayerPartyArray().size();
		}
		DWORD GetHeroCountInParty() {
			return GetHeroPartyArray().size();
		}
		DWORD GetHenchmanCountInParty() {
			return GetHenchPartyArray().size();
		}

		bool GetIsPartyInHardMode(){
			return GameContext::instance()->party->partystate.InHardMode();
		}
		bool GetIsPartyDefeated(){
			return GameContext::instance()->party->partystate.IsDefeated();
		}

		bool GetPartyIsTicked(){
			GW::PartyMemberArray& party = GetPlayerPartyArray();
			bool ticked = true;
			for (GW::PartyMember member : party)
			{
				if (!member.ticked()) ticked = false;
			}
			return ticked;
		}

	private:
		PartyMgr() : GWCAManager() {}
	};
}