#pragma once

#include <Windows.h>
#include <functional>

#include <GWCA\Utilities\Export.h>
#include <GWCA\Constants\Constants.h>
#include <GWCA\GameEntities\Friendslist.h>

namespace GW {

	namespace FriendListMgr {

		GWCA_API FriendList *GetFriendList();

		GWCA_API Friend *GetFriend(wchar_t *account, wchar_t *playing);

		inline Friend *GetFriend(DWORD index) { 
			return GetFriendList()->Friends[index + 1]; }

		inline DWORD GetNumberOfFriends() { return GetFriendList()->NumberOfFriend; }
		inline DWORD GetNumberOfIgnores() { return GetFriendList()->NumberOfIgnore; }
		inline DWORD GetNumberOfPartners() { return GetFriendList()->NumberOfPartner; }
		inline DWORD GetNumberOfTraders() { return GetFriendList()->NumberOfTrade; }

		inline DWORD GetMyStatus() { return GetFriendList()->PlayerStatus; }

		GWCA_API void SetFriendListStatus(Constants::OnlineStatus status);

		GWCA_API void SetOnFriendStatusCallback(
			std::function<void (Friend *f, GW::FriendStatus status)>);
	};
}
