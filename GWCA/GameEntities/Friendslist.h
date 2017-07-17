#ifndef _ENTITIE_FRIEND_LIST_H
#define _ENTITIE_FRIEND_LIST_H

#include <GWCA\GameContainers\Array.h>

namespace GW {
    using wchar = wchar_t;

    struct Friend {
        /* +h0000 */ DWORD Type; // 0 = Friend, 1 = Ignore, 2 = Played, 3 = Trade
        /* +h0004 */ DWORD Status; // 0 = Offline, 1 = Online, 2 = Do not disturb, 3 = Away
        /* +h0008 */ wchar Account[20];
        /* +h001C */ wchar Name[20];
        /* +h0030 */ DWORD FriendID;
        /* +h0034 */ DWORD ZoneId;
    };

    using FriendsListArray = Array<Friend*>;

    struct FriendList {
        /* +h0000 */ FriendsListArray Friends;
        /* +h0010 */ DWORD h0010[5];
        /* +h0024 */ DWORD NumberOfFriend;
        /* +h0028 */ DWORD NumberOfIgnore;
        /* +h002C */ DWORD NumberOfPartner;
        /* +h0030 */ DWORD NumberOfTrade;
        /* +h0034 */ DWORD h0034[18];
        /* +h007C */ DWORD PlayerStatus;
    };
}

#endif // _ENTITIE_FRIEND_LIST_H
