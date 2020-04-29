#pragma once

#include <GWCA/GameContainers/Array.h>

namespace GW {
    enum FriendType : uint32_t {
        FriendType_Unknow = 0,
        FriendType_Friend = 1,
        FriendType_Ignore = 2,
        FriendType_Player = 3,
        FriendType_Trade  = 4,
    };

    enum FriendStatus : uint32_t {
        FriendStatus_Offline = 0,
        FriendStatus_Online  = 1,
        FriendStatus_DND     = 2,
        FriendStatus_Away    = 3,
    };

    struct Friend {
        /* +h0000 */ FriendType type; // 0 = Friend, 1 = Ignore, 2 = Played, 3 = Trade
        /* +h0004 */ FriendStatus status; // 0 = Offline, 1 = Online, 2 = Do not disturb, 3 = Away
        /* +h0008 */ uint8_t uuid[16];
        /* +h0018 */ wchar_t alias[20];
        /* +h002C */ wchar_t charname[20];
        /* +h0040 */ uint32_t friend_id;
        /* +h0044 */ uint32_t zone_id;
    };

    typedef Array<Friend *> FriendsListArray;

    struct FriendList {
        /* +h0000 */ FriendsListArray friends;
        /* +h0010 */ uint8_t  h0010[20];
        /* +h0024 */ uint32_t number_of_friend;
        /* +h0028 */ uint32_t number_of_ignore;
        /* +h002C */ uint32_t number_of_partner;
        /* +h0030 */ uint32_t number_of_trade;
        /* +h0034 */ uint8_t  h0034[108];
        /* +h00A0 */ FriendStatus player_status;
    };
}
