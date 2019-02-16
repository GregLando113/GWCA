#pragma once

namespace GW {

    struct Friend;
    struct FriendList;

    enum FriendStatus : uint32_t;

    namespace Constants {
        enum class OnlineStatus;
    }

    struct Module;
    extern Module FriendListModule;

    namespace FriendListMgr {

        GWCA_API FriendList *GetFriendList();

        GWCA_API Friend *GetFriend(wchar_t *account, wchar_t *playing);

        GWCA_API Friend *GetFriend(uint32_t index);

        GWCA_API uint32_t GetNumberOfFriends();
        GWCA_API uint32_t GetNumberOfIgnores();
        GWCA_API uint32_t GetNumberOfPartners();
        GWCA_API uint32_t GetNumberOfTraders();

        GWCA_API uint32_t GetMyStatus();

        GWCA_API void SetFriendListStatus(Constants::OnlineStatus status);

        GWCA_API void SetOnFriendStatusCallback(
            std::function<void (Friend *f, GW::FriendStatus status)>);
    };
}
