#pragma once

#include <Windows.h>
#include "..\GameContainers\gw_array.h"

namespace GWCA {
  namespace GW {
    struct Friend {
      DWORD type;				// 0 = Friend, 1 = Ignore, 2 = Played, 3 = Trade
      DWORD status;			// 0 = Offline, 1 = Online, 2 = Do not disturb, 3 = Away
      WCHAR account[20];
      WCHAR name[20];
      DWORD index;			// Order by adding time
      DWORD zoneId;
    };

    using FriendsListArray = gw_array<Friend*>;

    struct FriendList {
      FriendsListArray friends;
      BYTE unknow[20];
      DWORD nFriend;
      DWORD nIgnore;
      DWORD nPlayed;
      DWORD nTrade;
      BYTE unknow2[72];
      DWORD myStatus;

      inline DWORD size() { return (nFriend + nIgnore + nPlayed + nTrade); };
    };
  }
}
