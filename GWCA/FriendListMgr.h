#pragma once

#include <Windows.h>

#include "GWStructures.h"
#include "GwConstants.h"

namespace GWAPI {

	class FriendListMgr {
	public:

		inline GWAPI::GW::Friend* get(DWORD index) { return fList_->friends[index + 1]; }
		inline DWORD size() { return fList_->size(); }
		inline DWORD getNumberOfFriends() { return fList_->nFriend; }
		inline DWORD getNumberOfIgnores() { return fList_->nIgnore; }
		inline DWORD getNumberOfPartners() { return fList_->nPlayed; }
		inline DWORD getNumberOfTraders() { return fList_->nTrade; }

		inline DWORD getMyStatus() { return fList_->myStatus; }

		void SetFriendListStatus(GwConstants::OnlineStatus status);

	private:
		typedef void(__fastcall *SetOnlineStatus_t)(DWORD status);
		
		friend class GWAPIMgr;
		const GWAPIMgr* parent_;

		FriendListMgr(GWAPIMgr* parent);

		SetOnlineStatus_t set_online_status_;
		GWAPI::GW::FriendList* fList_;

		void FindList();

	};

}