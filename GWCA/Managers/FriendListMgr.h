#pragma once

#include <Windows.h>

#include <GWCA\Constants\Constants.h>
#include <GWCA\GameEntities\Friendslist.h>

namespace GW {

	namespace FriendListMgr {

		GW::FriendList* GetFriendList();

		inline GW::Friend* GetFriend(DWORD index) { 
			return GetFriendList()->friends[index + 1]; }
		inline DWORD size() { return GetFriendList()->size(); }
		inline DWORD GetNumberOfFriends() { return GetFriendList()->nFriend; }
		inline DWORD GetNumberOfIgnores() { return GetFriendList()->nIgnore; }
		inline DWORD GetNumberOfPartners() { return GetFriendList()->nPlayed; }
		inline DWORD GetNumberOfTraders() { return GetFriendList()->nTrade; }

		inline DWORD GetMyStatus() { return GetFriendList()->myStatus; }

		void SetFriendListStatus(Constants::OnlineStatus status);
	};
}
