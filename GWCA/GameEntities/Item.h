#pragma once

#include <Windows.h>

#include <GWCA\GameContainers\Array.h>

namespace GW {

	using ItemID = DWORD;

	struct Bag;
	struct Item;

	using ItemArray = Array<Item*>;

	struct Bag {							// total : 24 BYTEs
		BYTE unknown1[4];					// 0000	|--4 BYTEs--|
		long index;							// 0004
		DWORD BagId;						// 0008
		DWORD ContainerItem;				// 000C
		DWORD ItemsCount;					// 0010
		Bag* BagArray;						// 0014
		ItemArray Items;					// 0020
	};

	class ItemModifier {
	public:

		DWORD identifier() { return (mod_ & 0x3FF00000) >> 20; }
		DWORD arg1() { return (mod_ & 0x0001FF00) >> 8; }
		DWORD arg2() { return (mod_ & 0x000000FE); }
		DWORD arg3() { return (mod_ & 0x0003FFFF); }
		DWORD arg4() { return (mod_ & 0x00040000) >> 17; }
		DWORD arg5() { return (mod_ & 0x0001FFFE); }
		DWORD arg6() { return (mod_ & 0x00000001); }

	private:
		DWORD mod_;
	};

	struct Item { // total : 0x50/80 BYTEs
		/* +h0000 */ DWORD	ItemId;
		/* +h0004 */ DWORD	AgentId;
		/* +h0008 */ BYTE	h0008[4];
		/* +h000C */ Bag   *Bag;
		/* +h0010 */ ItemModifier* ModStruct; // Pointer to an array of mods.
		/* +h0014 */ DWORD	ModStructSize; // Size of this array.
		/* +h0018 */ WCHAR *Customized;
		/* +h001C */ DWORD	ModelFileID;
		/* +h0020 */ BYTE	Type;
		/* +h0021 */ BYTE	h0021;
		/* +h0022 */ WORD	ExtraId;
		/* +h0024 */ WORD	Value;
		/* +h0026 */ BYTE	h0026[4];
		/* +h002A */ WORD	Interaction;
		/* +h002C */ DWORD	ModelId;
		/* +h0030 */ WCHAR *InfoString;
		/* +h0034 */ BYTE	h0034[4];
		/* +h0038 */ WCHAR *NameString;
		/* +h003C */ BYTE	h003C[15];
		/* +h004B */ BYTE	Quantity;
		/* +h004C */ BYTE	Equipped;
		/* +h004D */ BYTE	Profession;
		/* +h004E */ BYTE	Slot;
	};

	struct Inventory {
		Bag  *bags[0x1F];
		DWORD gold_character;
		DWORD gold_storage;
	};

	using MerchItemArray = Array<ItemID>;
}
