
#pragma once
#include <Windows.h>

#include "..\GameEntities\Guild.h"
#include "..\GameContainers\gw_array.h"

namespace GWCA {
using namespace GW;
class GuildContext
{
public:
  void* unk1_ptr; // 0
  DWORD unk1; // 4
  void* unk2_ptr; // 8
  void* unk3_ptr; // c
  DWORD unk2; // 10
  void* unk4_ptr; // 14
  void* unk5_ptr; // 18
  DWORD unk3; // 1c
  gw_array<void*> unk1_arr; // 20 - wierd fucking graph/binary tree i think
  DWORD unk4; // 30
  wchar_t playername[20]; // 34
  DWORD unk5; // 5c
  DWORD playerguildindex; // 60
  GHKey playerghkey; // 64
  DWORD unk6; // 74
  wchar_t guildannouncement[256]; // 78
  wchar_t lastannouncementwriter[20]; // 278
  DWORD unk7; // 2A0
  void* unk6_ptr; // 2A4
  gw_array<void*> unk2_arr; // 2A8
  gw_array<void*> unk3_arr; // 2B8
  DWORD unk8; //2C8
  GuildHistory playerguildhistory; // 2CC
  BYTE pad1[0x1C]; // 2DC
  GuildArray guilds; // 2F8
  DWORD unk9[4]; // 308
  gw_array<void*> unk4_arr; // 318 - some binary tree pointing to guilds
  DWORD unk10; // 328
  gw_array<void*> unk5_arr; // 32C
  BYTE pad2[0x1C]; // 33C
  GuildRoster playerroster; // 358
  //... end of what i care about
};
}
