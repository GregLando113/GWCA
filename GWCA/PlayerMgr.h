#pragma once

#include "GWCAManager.h"
#include "GameContext.h"

namespace GWAPI {
	class PlayerMgr : public GWCAManager {
		friend class GWAPIMgr;
	public:

		GW::PlayerArray& GetPlayerArray() const {
			return GameContext::instance()->world->players;
		}

		GW::Player& GetPlayerByID(PlayerID id){
			return GetPlayerArray()[id];
		}

		wchar_t* GetPlayerName(PlayerID id){
			GetPlayerArray()[id].Name;
		}

		void SetPlayerName(PlayerID id,wchar_t* replacename){
			wcscpy(GetPlayerArray()[id].Name1->Name, replacename);
		}

	private:
		PlayerMgr(GWAPIMgr& mgr) : GWCAManager(mgr) {}
	};
}