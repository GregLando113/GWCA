#include "stdafx.h"

#include <GWCA/Utilities/Debug.h>
#include <GWCA/Utilities/Hooker.h>
#include <GWCA/Utilities/Macros.h>
#include <GWCA/Utilities/Scanner.h>

#include <GWCA/GameEntities/FriendsList.h>

#include <GWCA/Managers/Module.h>
#include <GWCA/Managers/FriendListMgr.h>

namespace {
    using namespace GW;

    typedef void(__cdecl *FriendStatusHandler_pt)(
        FriendStatus status, const uint8_t *uuid, const wchar_t *alias, const wchar_t *charname);
    FriendStatusHandler_pt RetFriendStatusHandler;
    FriendStatusHandler_pt FriendStatusHandler_Func;

    HookCallback<Friend *, FriendStatus, const wchar_t *, const wchar_t *> OnFriendStatus_callback;
    std::unordered_map<HookEntry *, FriendListMgr::FriendStatusCallback> FriendStatus_callbacks;
    void __cdecl OnFriendStatusHandler(FriendStatus status,
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

    typedef void(__cdecl *SetOnlineStatus_pt)(FriendStatus status);
    SetOnlineStatus_pt SetOnlineStatus_Func;

    // type:
    //  1 = Friend
    //  2 = Ignore
    typedef void (__cdecl *AddFriend_pt)(const wchar_t *name, const wchar_t *alias, FriendType type);
    AddFriend_pt AddFriend_Func;

    typedef void (__cdecl *RemoveFriend_pt)(const uint8_t *uuid, const wchar_t *name, uint32_t arg8);
    RemoveFriend_pt RemoveFriend_Func;

    uintptr_t FriendList_Addr;

    void Init() {

        uintptr_t address = Scanner::Find("\x74\x30\x8D\x47\xFF\x83\xF8\x01", "xxxxxxxx", -0xB);
        if (Verify(address))
            FriendList_Addr = *(uintptr_t*)address;

        FriendStatusHandler_Func = (FriendStatusHandler_pt)Scanner::Find("\x8B\x75\x14\x8B\x01\x89\x45\x98", "xxxxxxxx", -0x17);
        SetOnlineStatus_Func = (SetOnlineStatus_pt)Scanner::Find("\x83\xFE\x03\x77\x40\xFF\x24\xB5\x00\x00\x00\x00\x33\xC0", "xxxxxxxx????xx", -0x26);
        AddFriend_Func = (AddFriend_pt)Scanner::Find("\x8B\x75\x10\x83\xFE\x03\x74\x65", "xxxxxxxx", -0x48);
        RemoveFriend_Func = (RemoveFriend_pt)Scanner::Find("\x8B\x4D\x10\x89\x4E\x28\x8B\x4D\x08\xC7\x06", "xxxxxxxxxxx", -0x2D);

        GWCA_INFO("[SCAN] FriendList_Addr = %p", FriendList_Addr);
        GWCA_INFO("[SCAN] FriendStatusHandler = %p", FriendStatusHandler_Func);
        GWCA_INFO("[SCAN] SetOnlineStatus = %p", SetOnlineStatus_Func);
        GWCA_INFO("[SCAN] AddFriend_Func = %p", AddFriend_Func);
        GWCA_INFO("[SCAN] RemoveFriend_Func = %p", RemoveFriend_Func);

#if _DEBUG
        GWCA_ASSERT(FriendList_Addr);
        GWCA_ASSERT(FriendStatusHandler_Func);
        GWCA_ASSERT(SetOnlineStatus_Func);
        GWCA_ASSERT(AddFriend_Func);
        GWCA_ASSERT(RemoveFriend_Func);
#endif

        HookBase::CreateHook(FriendStatusHandler_Func, OnFriendStatusHandler, (void**)&RetFriendStatusHandler);
    }

    void EnableHooks() {
        if (FriendStatusHandler_Func)
            HookBase::EnableHooks(FriendStatusHandler_Func);
    }

    void DisableHooks() {
        if(FriendStatusHandler_Func)
            HookBase::DisableHooks(FriendStatusHandler_Func);
    }

    void Exit() {
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

    bool FriendListMgr::SetFriendListStatus(FriendStatus status) {
        if (!SetOnlineStatus_Func) return false;
        SetOnlineStatus_Func(status);
        return true;
    }

    void FriendListMgr::RegisterFriendStatusCallback(
        HookEntry *entry,
        const FriendStatusCallback& callback)
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

    Friend *FriendListMgr::GetFriend(const wchar_t *alias, const wchar_t *playing, FriendType type = FriendType::Friend) {
        if (!(alias || playing)) return NULL;
        FriendList *fl = GetFriendList();
        if (!fl) return NULL;
        uint32_t n_friends = fl->number_of_friend;
        FriendsListArray &friends = fl->friends;
        for (Friend *it : friends) {
            if (!it) continue;
            if (it->type != type) continue;
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

    FriendStatus FriendListMgr::GetMyStatus() {
        FriendList *fl = GetFriendList();
        return fl ? fl->player_status : FriendStatus::Offline;
    }

    static bool InternalAddFriend(FriendType type, const wchar_t *name, const wchar_t *alias)
    {
        if (!AddFriend_Func) return false;
        wchar_t buffer[32];
        if (!alias) {
            wcsncpy(buffer, name, 32);
            alias = buffer;
        }
        AddFriend_Func(name, alias, type);
        return true;
    }

    bool FriendListMgr::AddFriend(const wchar_t *name, const wchar_t *alias)
    {
        return InternalAddFriend(FriendType::Friend, name, alias);
    }

    bool FriendListMgr::AddIgnore(const wchar_t *name, const wchar_t *alias)
    {
        return InternalAddFriend(FriendType::Ignore, name, alias);
    }

    bool FriendListMgr::RemoveFriend(Friend *_friend)
    {
        if (!(_friend && RemoveFriend_Func))
            return false;
        RemoveFriend_Func(_friend->uuid, _friend->alias, 0);
        return true;
    }
} // namespace GW
