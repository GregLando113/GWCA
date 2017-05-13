#pragma once

#include <GWCA\Constants\Constants.h>
#include <GWCA\GameEntities\Player.h>

namespace GW {
	namespace PlayerMgr {

		void SetActiveTitle(GW::Constants::TitleID id);

		void RemoveActiveTitle();

		GW::PlayerArray& GetPlayerArray();

		GW::Player& GetPlayerByID(PlayerID id);

		wchar_t* GetPlayerName(PlayerID id);

		void SetPlayerName(PlayerID id, const wchar_t* replacename);

		void ChangeSecondProfession(GW::Constants::Profession prof, DWORD heroindex = 0);
	};
}
