#pragma once
#include <Windows.h>
#include "..\GameContainers\gw_array.h"

namespace GWCA {
	using namespace GW;

	class CharContext {
	public:
		gw_array<void*> unk1_arr; //0x0000
		char pad_0x0010[0x4]; //0x0010
		gw_array<void*> unk2_arr; //0x0014
		char pad_0x0024[0x10]; //0x0024
		gw_array<void*> unk3_arr; //0x0034
		gw_array<void*> unk4_arr; //0x0044
		char pad_0x0054[0x20]; //0x0054
		wchar_t playername[20]; //0x0074
		char pad_0x009C[0x50]; //0x009C
		gw_array<void*> unk6_arr; //0x00EC
		char pad_0x00FC[0x30]; //0x00FC
		DWORD mapidagain; //0x012C
		char pad_0x0130[0x60]; //0x0130
		DWORD currentmapid_again; //0x0190
		char pad_0x0194[0x28]; //0x0194
		DWORD currentmapid; //0x01BC
		DWORD currentmapid_dupe; //0x01C0
		char pad_0x01C4[0x8]; //0x01C4
		gw_array<void*> unk5_arr; //0x01CC
		char pad_0x01DC[0x14C]; //0x01DC
		wchar_t playeremail[64]; //0x0328
	};//Size=0x03A8
}
