#pragma once

#include <GWCA/Utilities/Hook.h>
#include <GWCA/Utilities/Export.h>

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

        GWCA_API Friend *GetFriend(wchar_t *alias, wchar_t *charname);
        GWCA_API Friend *GetFriend(uint32_t index);
        GWCA_API Friend *GetFriend(const uint8_t *uuid);

        GWCA_API uint32_t GetNumberOfFriends();
        GWCA_API uint32_t GetNumberOfIgnores();
        GWCA_API uint32_t GetNumberOfPartners();
        GWCA_API uint32_t GetNumberOfTraders();

        GWCA_API uint32_t GetMyStatus();

        GWCA_API void SetFriendListStatus(Constants::OnlineStatus status);

        typedef HookCallback<Friend *, FriendStatus, const wchar_t *, const wchar_t *> FriendStatusCallback;
        GWCA_API void RegisterFriendStatusCallback(
            HookEntry *entry,
            FriendStatusCallback callback);

        GWCA_API void RemoveFriendStatusCallback(
            HookEntry *entry);

        GWCA_API void AddFriend(const wchar_t *name, const wchar_t *alias = nullptr);
        GWCA_API void AddIgnore(const wchar_t *name, const wchar_t *alias = nullptr);
        GWCA_API void RemoveFriend(Friend *_friend);
    };
}
