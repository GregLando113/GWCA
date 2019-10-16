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
        FriendStatus status, const uint8_t *uuid, const wchar_t *alias, const wchar_t *charname);
    FriendStatusHandler_pt RetFriendStatusHandler;
    FriendStatusHandler_pt FriendStatusHandler_Func;

    HookCallback<Friend *, FriendStatus, const wchar_t *, const wchar_t *> OnFriendStatus_callback;
    std::unordered_map<HookEntry *, FriendListMgr::FriendStatusCallback> FriendStatus_callbacks;
    void __fastcall OnFriendStatusHandler(FriendStatus status, 
        const uint8_t *uuid, const wchar_t *alias, const wchar_t *charname)
    {
        HookBase::EnterHook();
        Friend *_friend = FriendListMgr::GetFriend(uuid);
        HookStatus hook_status;
        if (_friend) {
            for (auto& it : FriendStatus_callbacks) {
                it.second(&hook_status, _friend, status, alias, charname);
                ++hook_status.altitude;
            }
        }
        if (!hook_status.blocked)
            RetFriendStatusHandler(status, uuid, alias, charname);
        HookBase::LeaveHook();
    }

    typedef void(__fastcall *SetOnlineStatus_pt)(uint32_t status);
    SetOnlineStatus_pt SetOnlineStatus_Func;

    // type:
    //  1 = Friend
    //  2 = Ignore
    typedef void (__fastcall *AddFriend_pt)(const wchar_t *name, const wchar_t *alias, uint32_t type);
    AddFriend_pt AddFriend_Func;

    typedef void (__fastcall *RemoveFriend_pt)(const uint8_t *uuid, const wchar_t *name, uint32_t arg8);
    RemoveFriend_pt RemoveFriend_Func;

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
            "\x89\x45\x9C\x8B\x4A\x04\x8B\x42", "xxxxxxxx", -0xB);
        printf("[SCAN] FriendStatusHandler = %p\n", FriendStatusHandler_Func);

        SetOnlineStatus_Func = (SetOnlineStatus_pt)Scanner::Find(
            "\x8B\xCE\x89\x35\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x8B\xC8", "xxxx????x????xx", -0x1C);
        printf("[SCAN] SetOnlineStatus = %p\n", SetOnlineStatus_Func);

        if (Verify(FriendStatusHandler_Func)) {
            HookBase::CreateHook(FriendStatusHandler_Func,
                OnFriendStatusHandler, (void **)&RetFriendStatusHandler);
        }

        AddFriend_Func = (AddFriend_pt)Scanner::Find(
            "\x8B\x5D\x08\x83\xFB\x03\x74\x00\x83", "xxxxxxx?x", -0x46);
        printf("[SCAN] AddFriend_Func = %p\n", AddFriend_Func);

        RemoveFriend_Func = (RemoveFriend_pt)Scanner::Find(
            "\x8B\x4D\x08\x8D\x50\x2C\x89\x48\x28", "xxxxxxxxx", -0x29);
        printf("[SCAN] RemoveFriend_Func = %p\n", RemoveFriend_Func);
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

    void FriendListMgr::RegisterFriendStatusCallback(
        HookEntry *entry,
        FriendStatusCallback callback)
    {
        FriendStatus_callbacks.insert({entry, callback});
    }

    void FriendListMgr::RemoveFriendStatusCallback(
            HookEntry *entry)
    {
        auto it = FriendStatus_callbacks.find(entry);
        if (it != FriendStatus_callbacks.end())
            FriendStatus_callbacks.erase(it);
    }

    Friend *FriendListMgr::GetFriend(wchar_t *alias, wchar_t *playing) {
        if (!(alias || playing)) return NULL;
        FriendList *fl = GetFriendList();
        if (!fl) return NULL;
        uint32_t n_friends = fl->number_of_friend;
        FriendsListArray &friends = fl->friends;
        for (Friend *it : friends) {
            if (!it) continue;
            if (it->type != FriendType_Friend) continue;
            if (n_friends == 0) break;
            --n_friends;
            if (alias && !wcsncmp(it->alias, alias, 20))
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

    Friend *FriendListMgr::GetFriend(const uint8_t *uuid) {
        FriendList *fl = GetFriendList();
        FriendsListArray& friends = fl->friends;
        for (size_t i = 0; i < friends.size(); i++) {
            Friend *it = friends[i];
            if (!it) continue;
            if (!memcmp(it->uuid, uuid, 16))
                return it;
        }
        return nullptr;
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

    static void InternalAddFriend(uint32_t type, const wchar_t *name, const wchar_t *alias)
    {
        wchar_t buffer[32];
        if (!alias) {
            wcsncpy(buffer, name, 32);
            alias = buffer;
        }
        AddFriend_Func(name, alias, type);
    }

    void FriendListMgr::AddFriend(const wchar_t *name, const wchar_t *alias)
    {
        InternalAddFriend(1, name, alias);
    }

    void FriendListMgr::AddIgnore(const wchar_t *name, const wchar_t *alias)
    {
        InternalAddFriend(2, name, alias);
    }

    void FriendListMgr::RemoveFriend(Friend *_friend)
    {
        if (!_friend)
            return;
        RemoveFriend_Func(_friend->uuid, _friend->alias, 0);
    }
} // namespace GW
