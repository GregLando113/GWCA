#pragma once

#include <Windows.h>

#include <GWCA\GameContainers\Array.h>
#include <GWCA\GameEntities\Item.h>

namespace GW {

	class ItemContext {
	public:
		char pad_0x0000[0x10]; //0x0000
		Array<void*> unk1_arr; //0x0010
		char pad_0x0020[0x4]; //0x0020
		Array<Bag*> bagarray; //0x0024
		char pad_0x0034[0xC]; //0x0034
		Array<void*> unk3_arr; //0x0040
		Array<void*> unk4_arr; //0x0050
		char pad_0x0060[0x58]; //0x0060
		Array<Item*> itemarray; //0x00B8
		char pad_0x00C8[0x30]; //0x00C8
		Inventory* inventory; //0x00F8
		Array<void*> unk5_arr; //0x00FC

	};//Size=0x010C
}
