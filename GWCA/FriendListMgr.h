#pragma once

#include <Windows.h>
#include "GWAPIMgr.h"

namespace GWAPI {

	class FriendListMgr {

	public:
		FriendListMgr(GWAPIMgr* parent);

		inline GWAPI::GW::Friend* get(DWORD index) { return fList_->friends[index + 1]; }
		inline DWORD size() { return fList_->size(); }
		inline DWORD getNumberOfFriends() { return fList_->nFriend; }
		inline DWORD getNumberOfIgnores() { return fList_->nIgnore; }
		inline DWORD getNumberOfPartners() { return fList_->nPlayed; }
		inline DWORD getNumberOfTraders() { return fList_->nTrade; }

		inline DWORD getMyStatus() { return fList_->myStatus; }

	private:
		friend class GWAPIMgr;
		const GWAPIMgr* parent_;

		GWAPI::GW::FriendList* fList_;

		void FindList();

	};

}