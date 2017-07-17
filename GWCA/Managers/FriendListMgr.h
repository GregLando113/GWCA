#pragma once

#include <Windows.h>

#include <GWCA\Constants\Constants.h>
#include <GWCA\GameEntities\Friendslist.h>

namespace GW {

	namespace FriendListMgr {

		GW::FriendList* GetFriendList();

		inline GW::Friend* GetFriend(DWORD index) { 
			return GetFriendList()->Friends[index + 1]; }

		inline DWORD GetNumberOfFriends() { return GetFriendList()->NumberOfFriend; }
		inline DWORD GetNumberOfIgnores() { return GetFriendList()->NumberOfIgnore; }
		inline DWORD GetNumberOfPartners() { return GetFriendList()->NumberOfPartner; }
		inline DWORD GetNumberOfTraders() { return GetFriendList()->NumberOfTrade; }

		inline DWORD GetMyStatus() { return GetFriendList()->PlayerStatus; }

		void SetFriendListStatus(Constants::OnlineStatus status);
	};
}
