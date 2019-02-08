#ifndef _ENTITIE_ITEM_INC
#define _ENTITIE_ITEM_INC

#include <GWCA/GameContainers/Array.h>
#include <GWCA/Constants/ItemIDs.h>

namespace GW {

    using wchar = wchar_t;
    using ItemID = DWORD;

    struct Bag;
    struct Item;

    using ItemArray = Array<Item*>;

    struct Bag { // total: 0x24/36
        /* +h0000 */ DWORD BagType; // Bag 1, Equipped 2, NotCollected 3, Storage 4, MaterialStorage 5
        /* +h0004 */ DWORD Index;
        /* +h0008 */ DWORD BagId;
        /* +h000C */ DWORD ContainerItem;
        /* +h0010 */ DWORD ItemsCount;
        /* +h0014 */ Bag  *BagArray;
        /* +h0018 */ ItemArray Items;
		/* +h001C */ BYTE h001C[4];
		/* +h0020 */ DWORD Slots;

        inline bool IsInventoryBag() { return (BagType == 1); }
        inline bool IsStorageBag()   { return (BagType == 4); }

        static const size_t npos = (size_t)-1;

        size_t find_dye(DWORD model_id, DWORD ExtraId, size_t pos = 0);

        size_t find1(DWORD model_id, size_t pos = 0);
        size_t find2(Item *item, size_t pos = 0);
    };

    struct ItemModifier {
        DWORD mod;

        DWORD identifier() { return (mod & 0x3FF00000) >> 20; }
        DWORD arg1() { return (mod & 0x0001FF00) >> 8; }
        DWORD arg2() { return (mod & 0x000000FE); }
        DWORD arg3() { return (mod & 0x0003FFFF); }
        DWORD arg4() { return (mod & 0x00040000) >> 17; }
        DWORD arg5() { return (mod & 0x0001FFFE); }
        DWORD arg6() { return (mod & 0x00000001); }
    };

    struct Item { // total: 0x50/80
        /* +h0000 */ DWORD ItemId;
        /* +h0004 */ DWORD AgentId;
        /* +h0008 */ Bag  *BagEquiped; // Only valid if Item is a equipped Bag
        /* +h000C */ Bag  *Bag;
        /* +h0010 */ ItemModifier *ModStruct; // Pointer to an array of mods.
        /* +h0014 */ DWORD ModStructSize; // Size of this array.
        /* +h0018 */ wchar *Customized;
        /* +h001C */ DWORD ModelFileID;
        /* +h0020 */ BYTE Type;
        /* +h0021 */ BYTE h0021;
        /* +h0022 */ WORD ExtraId;
        /* +h0024 */ WORD Value;
        /* +h0026 */ BYTE h0026[4];
        /* +h002A */ WORD Interaction;
        /* +h002C */ DWORD ModelId;
        /* +h0030 */ wchar *InfoString;
        /* +h0034 */ wchar *NameString;
        /* +h0038 */ wchar *CompleteName; // with color, quantity, etc.
        /* +h003C */ wchar *SingleItemName; // with color, w/o quantity, named as single item
        /* +h0040 */ BYTE h003C[10];
        /* +h004A */ BYTE IsMaterialSalvageable; // Only valid for type 11 (Materials)
		/* +h004B */ BYTE Unknown001; // probably used for quantity extension for new material storage
		/* +h004C */ WORD Quantity;
		/* +h004E */ BYTE Equipped;
		/* +h004F */ BYTE Profession;
		/* +h0050 */ BYTE Slot;

        bool GetIsStackable();
        bool GetIsMaterial();
        bool GetIsZcoin();
    };

    struct WeapondSet {
        Item *Weapon;
        Item *Offhand;
    };

    struct Inventory { // total: 0x98/152
        union {
        /* +h0000 */ Bag *Bags[23];
            struct {
        /* +h0000 */ Bag *UnusedBag;
        /* +h0004 */ Bag *Backpack;
        /* +h0008 */ Bag *BeltPouch;
        /* +h000C */ Bag *Bag1;
        /* +h0010 */ Bag *Bag2;
        /* +h0014 */ Bag *EquipmentPack;
        /* +h0018 */ Bag *MaterialStorage;
        /* +h001C */ Bag *UnclaimedItems;
        /* +h0020 */ Bag *Storage1;
        /* +h0024 */ Bag *Storage2;
        /* +h0028 */ Bag *Storage3;
        /* +h002C */ Bag *Storage4;
        /* +h0030 */ Bag *Storage5;
        /* +h0034 */ Bag *Storage6;
        /* +h0038 */ Bag *Storage7;
        /* +h003C */ Bag *Storage8;
        /* +h0040 */ Bag *Storage9;
        /* +h0044 */ Bag *Storage10;
        /* +h0048 */ Bag *Storage11;
        /* +h004C */ Bag *Storage12;
        /* +h0050 */ Bag *Storage13;
        /* +h0054 */ Bag *Storage14;
        /* +h0058 */ Bag *EquippedItems;
            };
        };
        /* +h005C */ Item *Bundle;
        /* +h0060 */ DWORD h004C;
        union {
        /* +h0064 */ WeapondSet WeaponSets[4];
            struct {
        /* +h0064 */ Item *WeaponSet0;
        /* +h0068 */ Item *OffhandSet0;
        /* +h006C */ Item *WeaponSet1;
        /* +h0070 */ Item *OffhandSet1;
        /* +h0074 */ Item *WeaponSet2;
        /* +h0078 */ Item *OffhandSet2;
        /* +h007C */ Item *WeaponSet3;
        /* +h0080 */ Item *OffhandSet3;
            };
        };
        /* +h0084 */ DWORD ActiveWeaponSet;
        /* +h0088 */ DWORD h0074[2];
        /* +h0090 */ DWORD GoldCharacter;
        /* +h0094 */ DWORD GoldStorage;
    };

    using MerchItemArray = Array<ItemID>;

    inline size_t Bag::find1(DWORD model_id, size_t pos) {
        for (size_t i = pos; i < Items.size(); i++) {
            Item *item = Items[i];
            if (!item && model_id == 0) return i;
            if (!item) continue;
            if (item->ModelId == model_id)
                return i;
        }
        return npos;
    }

    inline size_t Bag::find_dye(DWORD model_id, DWORD extra_id, size_t pos) {
        for (size_t i = pos; i < Items.size(); i++) {
            Item *item = Items[i];
            if (!item && model_id == 0) return i;
            if (!item) continue;
            if (item->ModelId == model_id && item->ExtraId == extra_id)
                return i;
        }
        return npos;
    }

    // Find a similar item
    inline size_t Bag::find2(Item *item, size_t pos) {
        if (item->ModelId == Constants::ItemID::Dye)
            return find_dye(item->ModelId, item->ExtraId, pos);
        else
            return find1(item->ModelId, pos);
    }
}

#endif // _ENTITIE_ITEM_INC
