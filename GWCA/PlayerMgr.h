#pragma once

#include "GWCAManager.h"
#include "GameContext.h"

namespace GWCA {
	class PlayerMgr : public GWCAManager<PlayerMgr> {
		friend class GWCAManager<PlayerMgr>;
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

		void SetPlayerName(PlayerID id, const wchar_t* replacename){
			wcscpy_s(GetPlayerArray()[id].Name1->Name, replacename);
		}

	private:
		PlayerMgr() {}
	};
}