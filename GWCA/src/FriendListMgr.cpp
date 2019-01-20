#include <assert.h>
#include <stdint.h>
#include <Windows.h>

#include <functional>

#include <GWCA/Constants/Constants.h>

#include <GWCA/Utilities/Export.h>
#include <GWCA/Utilities/Hooker.h>
#include <GWCA/Utilities/Scanner.h>

#include <GWCA/GameEntities/Friendslist.h>

#include <GWCA/Managers/FriendListMgr.h>

namespace {
    typedef void(__fastcall *OnFriendStatus_t)(GW::FriendStatus status, wchar_t account[20], wchar_t playing[20]);
    GW::THook<OnFriendStatus_t> OnFriendStatus_hook;
    std::function<void (GW::Friend *f, GW::FriendStatus status)> OnFriendStatus_callback;
    void __fastcall OnFriendStatus_detour(GW::FriendStatus status, wchar_t account[], wchar_t playing[]) {
        OnFriendStatus_hook.Original()(status, account, playing);
        GW::Friend *_friend = GW::FriendListMgr::GetFriend(account, playing);
        if (_friend && OnFriendStatus_callback)
            OnFriendStatus_callback(_friend, status);
    }
}

GW::FriendList *GW::FriendListMgr::GetFriendList() {
    static FriendList *flist = nullptr;
    if (flist == nullptr) {
        /*
        All function that directly use the adresse of friend list are really small or make no sense to scan.
        So we scan a function that call one of this function then follow by transforming rel adr to abs adr.
        */
        DWORD call = Scanner::Find("\x85\xC0\x74\x19\x6A\xFF\x8D\x50\x08\x8D\x4E\x08", "xxxxxxxxxxxx", -0x18);
        call = *(DWORD *)call + (call + 5 - 1) + 2;
        flist = &(*(GW::FriendList *)*(DWORD *)call);
    }
    return flist;
}

void GW::FriendListMgr::SetFriendListStatus(GW::Constants::OnlineStatus status) {
    typedef void(__fastcall *SetOnlineStatus_t)(DWORD status);
    static SetOnlineStatus_t setstatus_func = nullptr;
    if (setstatus_func == nullptr) {
        setstatus_func = (SetOnlineStatus_t)Scanner::Find("\x8B\xCE\x89\x35\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x8B\xC8", "xxxx????x????xx", -0x1C);
    }
    setstatus_func((DWORD)status);
}

void GW::FriendListMgr::SetOnFriendStatusCallback(std::function<void (Friend *f, GW::FriendStatus status)> callback) {
    if (OnFriendStatus_hook.Empty()) {
        OnFriendStatus_t addr = (OnFriendStatus_t)Scanner::Find("\x8B\xF1\x6A\x14\x8D\x4D\xAC\xE8", "xxxxxxxx", -7);
        printf("[SCAN] OnFriendStatus = %p\n", addr);
        OnFriendStatus_hook.Detour(addr, OnFriendStatus_detour);
    }
    OnFriendStatus_callback = callback;
}

GW::Friend *GW::FriendListMgr::GetFriend(wchar_t *account, wchar_t *playing) {
    if (!(account && playing)) return NULL;
    GW::FriendList *fl = GetFriendList();
    if (!fl) return NULL;
    DWORD n_friends = fl->number_of_friend;
    FriendsListArray &friends = fl->friends;
    for (Friend *it : friends) {
        if (!it) continue;
        if (it->type != FriendType_Friend) continue;
        if (n_friends == 0) break;
        --n_friends;
        if (account && !wcsncmp(it->account, account, 20))
            return it;
        if (playing && !wcsncmp(it->name, playing, 20))
            return it;
    }
    return NULL;
}
