#pragma once
#include <Windows.h>
#include "..\GameContainers\gw_array.h"

namespace GWCA {
	namespace GW {

		using ItemID = DWORD;

		struct Bag;
		struct Item;

		using ItemArray = gw_array<Item*>;

		struct Bag {							// total : 24 BYTEs
			BYTE unknown1[4];					// 0000	|--4 BYTEs--|
			long index;							// 0004
			DWORD BagId;						// 0008
			DWORD ContainerItem;				// 000C
			DWORD ItemsCount;					// 0010
			Bag* BagArray;						// 0014
			ItemArray Items;						// 0020
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

		struct Item {							// total : 50 BYTEs
			DWORD ItemId;						// 0000
			DWORD AgentId;						// 0004
			BYTE unknown1[4];					// 0008	|--4 BYTEs--|
			Bag* bag;							// 000C
			ItemModifier* ModStruct;						// 0010						pointer to an array of mods
			DWORD ModStructSize;				// 0014						size of this array
			wchar_t* Customized;				// 0018
			DWORD modelfileid;
			short type;
			short extraId;
			short value;
			BYTE unknown4[4];
			short interaction;
			long ModelId;
			BYTE* modString;
			BYTE unknown5[4];
			BYTE* extraItemInfo;
			BYTE unknown6[15];
			BYTE Quantity;
			BYTE equipped;
			BYTE profession;
			BYTE slot;						// 004F
		};

		struct Inventory {
			Bag* bags[0x1F];
			DWORD gold_character;
			DWORD gold_storage;
		};

		using MerchItemArray = gw_array<ItemID>;

	}
}
