#include "stdafx.h"

#include <GWCA/Constants/Constants.h>

#include <GWCA/Utilities/Export.h>
#include <GWCA/Utilities/Hooker.h>
#include <GWCA/Utilities/Macros.h>
#include <GWCA/Utilities/Scanner.h>

#include <GWCA/GameEntities/FriendsList.h>

#include <GWCA/Managers/Module.h>
#include <GWCA/Managers/FriendListMgr.h>

namespace {
    using namespace GW;

    typedef void(__fastcall *FriendStatusHandler_pt)(
        FriendList *ctx, uint32_t edx, uint32_t friend_id, FriendStatus status);
    FriendStatusHandler_pt RetFriendStatusHandler;
    FriendStatusHandler_pt FriendStatusHandler_Func;

    std::function<void (Friend *f, FriendStatus status)> OnFriendStatus_callback;
    void __fastcall OnFriendStatusHandler(FriendList *ctx, uint32_t edx, uint32_t friend_id, FriendStatus status) {
        HookBase::EnterHook();
        Friend *_friend = FriendListMgr::GetFriend(friend_id);
        if (_friend && OnFriendStatus_callback)
            OnFriendStatus_callback(_friend, status);
        HookBase::LeaveHook();
        RetFriendStatusHandler(ctx, edx, friend_id, status);
    }

    typedef void(__fastcall *SetOnlineStatus_pt)(uint32_t status);
    SetOnlineStatus_pt SetOnlineStatus_Func;

    uintptr_t FriendList_Addr;

    void Init() {
        {
            uintptr_t address = Scanner::Find(
                "\x85\xC0\x74\x19\x6A\xFF\x8D\x50\x18\x8D\x4E\x18", "xxxxxxxxxxxx", -0x18);
            printf("[SCAN] FriendList_Addr = %p\n", (void *)address);
            if (Verify(address)) {
                address = *(uintptr_t *)address + (address + 5 - 1) + 2;
                FriendList_Addr = *(uintptr_t *)address;
            }
        }

        FriendStatusHandler_Func = (FriendStatusHandler_pt)Scanner::Find(
            "\xC2\x08\x00\x8B\x55\x0C\x89\x56\x04", "xxxxxxxxx", -0x2F);
        printf("[SCAN] FriendStatusHandler = %p\n", FriendStatusHandler_Func);

        SetOnlineStatus_Func = (SetOnlineStatus_pt)Scanner::Find(
            "\x8B\xCE\x89\x35\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x8B\xC8", "xxxx????x????xx", -0x1C);
        printf("[SCAN] SetOnlineStatus = %p\n", SetOnlineStatus_Func);

        if (Verify(FriendStatusHandler_Func)) {
            HookBase::CreateHook(FriendStatusHandler_Func,
                OnFriendStatusHandler, (void **)&RetFriendStatusHandler);
        }
    }

    void Exit() {
        if (FriendStatusHandler_Func)
            HookBase::RemoveHook(FriendStatusHandler_Func);
    }
}

namespace GW {

    Module FriendListModule = {
        "FriendListModule",  // name
        NULL,               // param
        ::Init,             // init_module
        ::Exit,             // exit_module
        NULL,               // enable_hooks
        NULL,               // disable_hooks
    };

    FriendList *FriendListMgr::GetFriendList() {
        return (FriendList *)FriendList_Addr;
    }

    void FriendListMgr::SetFriendListStatus(Constants::OnlineStatus status) {
        if (Verify(SetOnlineStatus_Func))
            SetOnlineStatus_Func((uint32_t)status);
    }

    void FriendListMgr::SetOnFriendStatusCallback(
        std::function<void (Friend *f, FriendStatus status)> callback)
    {
        OnFriendStatus_callback = callback;
    }

    Friend *FriendListMgr::GetFriend(wchar_t *account, wchar_t *playing) {
        if (!(account || playing)) return NULL;
        FriendList *fl = GetFriendList();
        if (!fl) return NULL;
        uint32_t n_friends = fl->number_of_friend;
        FriendsListArray &friends = fl->friends;
        for (Friend *it : friends) {
            if (!it) continue;
            if (it->type != FriendType_Friend) continue;
            if (n_friends == 0) break;
            --n_friends;
            if (account && !wcsncmp(it->account, account, 20))
                return it;
            if (playing && !wcsncmp(it->charname, playing, 20))
                return it;
        }
        return NULL;
    }

    Friend *FriendListMgr::GetFriend(uint32_t index) {
        FriendList *fl = GetFriendList();
        if (!fl || index >= fl->friends.size())
            return NULL;
        return fl->friends[index];
    }

    uint32_t FriendListMgr::GetNumberOfFriends() {
        return GetFriendList()->number_of_friend;
    }

    uint32_t FriendListMgr::GetNumberOfIgnores() {
        return GetFriendList()->number_of_ignore;
    }

    uint32_t FriendListMgr::GetNumberOfPartners(){
        return GetFriendList()->number_of_partner;
    }

    uint32_t FriendListMgr::GetNumberOfTraders() {
        return GetFriendList()->number_of_trade;
    }

    uint32_t FriendListMgr::GetMyStatus() {
        FriendList *fl = GetFriendList();
        if (fl)
            return fl->player_status;
        else
            return 0;
    }

} // namespace GW