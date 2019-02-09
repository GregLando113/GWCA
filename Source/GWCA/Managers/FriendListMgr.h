#pragma once

namespace GW {

    struct Friend;
    struct FriendList;

    namespace Constants {
        enum class OnlineStatus;
    }

    namespace FriendListMgr {

        GWCA_API FriendList *GetFriendList();

        GWCA_API Friend *GetFriend(wchar_t *account, wchar_t *playing);

        // @Cleanup:
        // move this to cpp
        // inline Friend *GetFriend(uint32_t index) { 
        //     return GetFriendList()->friends[index + 1];
        // }

        // inline uint32_t GetNumberOfFriends() { return GetFriendList()->number_of_friend; }
        // inline uint32_t GetNumberOfIgnores() { return GetFriendList()->number_of_ignore; }
        // inline uint32_t GetNumberOfPartners() { return GetFriendList()->number_of_partner; }
        // inline uint32_t GetNumberOfTraders() { return GetFriendList()->number_of_trade; }

        // inline uint32_t GetMyStatus() { return GetFriendList()->player_status; }

        GWCA_API void SetFriendListStatus(Constants::OnlineStatus status);

        GWCA_API void SetOnFriendStatusCallback(
            std::function<void (Friend *f, GW::FriendStatus status)>);
    };
}
