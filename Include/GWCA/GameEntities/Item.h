#pragma once
#include <GWCA/Constants/Items.h>

#include <GWCA/GameContainers/Array.h>


namespace GW {
    namespace Constants {
        enum class Profession;
    }

    struct Item;
    typedef Array<Item *> ItemArray;
    struct DyeInfo {
        uint8_t dye_tint;
        uint8_t dye1 : 4;
        uint8_t dye2 : 4;
        uint8_t dye3 : 4;
        uint8_t dye4 : 4;
    };
    struct Bag { // total: 0x28/40
        /* +h0000 */ uint32_t bag_type; // Bag 1, Equipped 2, NotCollected 3, Storage 4, MaterialStorage 5
        /* +h0004 */ uint32_t index;
        /* +h0008 */ Constants::Bag bag_id;
        /* +h000C */ uint32_t container_item;
        /* +h0010 */ uint32_t items_count;
        /* +h0014 */ Bag  *bag_array;
        /* +h0018 */ ItemArray items;

        inline bool IsInventoryBag() const { return (bag_type == 1); }
        inline bool IsStorageBag()   const { return (bag_type == 4); }
        inline bool IsMaterialStorage()   const { return (bag_type == 5); }

        static const size_t npos = (size_t)-1;

        size_t find_dye(DyeInfo ExtraId, size_t pos = 0) const;

        size_t find1(Constants::ModelID model_id, size_t pos = 0) const;
        size_t find2(const Item *item, size_t pos = 0) const;
    };
    static_assert(sizeof(Bag) == 40, "struct Bag has incorect size");

    struct ItemModifier {
        uint32_t mod = 0;
                          
        uint32_t identifier() const { return mod >> 16; }
        uint32_t arg1() const { return (mod & 0x0000FF00) >> 8; }
        uint32_t arg2() const { return (mod & 0x000000FF); }
        operator bool() const { return mod != 0; }
    };

    struct Item { // total: 0x54/84
        /* +h0000 */ ItemID       item_id;
        /* +h0004 */ AgentID       agent_id;
        /* +h0008 */ Bag           *bag_equipped; // Only valid if Item is a equipped Bag
        /* +h000C */ Bag           *bag;
        /* +h0010 */ ItemModifier  *mod_struct; // Pointer to an array of mods.
        /* +h0014 */ uint32_t       mod_struct_size; // Size of this array.
        /* +h0018 */ wchar_t       *customized;
        /* +h001C */ uint32_t       model_file_id;
        /* +h0020 */ uint8_t        type_uint8;
        /* +h0021 */ DyeInfo        dye;
        /* +h0024 */ uint16_t       value;
        /* +h0026 */ uint16_t       h0026;
        /* +h0028 */ uint32_t       interaction;
        /* +h002C */ Constants::ModelID       model_id;
        /* +h0030 */ wchar_t       *info_string;
        /* +h0034 */ wchar_t       *name_enc;
        /* +h0038 */ wchar_t       *complete_name_enc; // with color, quantity, etc.
        /* +h003C */ wchar_t       *single_item_name; // with color, w/o quantity, named as single item
        /* +h0040 */ uint8_t        h0040[10];
        /* +h004A */ uint8_t        is_material_salvageable; // Only valid for type 11 (Materials)
        /* +h004B */ uint8_t        h004B; // probably used for quantity extension for new material storage
        /* +h004C */ uint16_t       quantity;
        /* +h004E */ uint8_t        equipped;
        /* +h004F */ uint8_t        profession_uint8;
        /* +h0050 */ uint8_t        slot;

        inline Constants::Profession profession() const { return (Constants::Profession)profession_uint8; }
        inline Constants::ItemType type() const { return (Constants::ItemType)type_uint8; }

        inline bool GetIsStackable() const {
            return (interaction & 0x80000) != 0;
        }

        bool GetIsMaterial() const;
        bool GetIsZcoin() const;
    };
    static_assert(sizeof(Item) == 84, "struct Item has incorect size");

    struct WeapondSet { // total: 0x8/8
        /* +h0000 */ Item *weapond;
        /* +h0004 */ Item *offhand;
    };
    static_assert(sizeof(WeapondSet) == 8, "struct WeapondSet has incorect size");

    struct Inventory { // total: 0x98/152
        union {
        /* +h0000 */ Bag *bags[23];
            struct {
        /* +h0000 */ Bag *unused_bag;
        /* +h0004 */ Bag *backpack;
        /* +h0008 */ Bag *belt_pouch;
        /* +h000C */ Bag *bag1;
        /* +h0010 */ Bag *bag2;
        /* +h0014 */ Bag *equipment_pack;
        /* +h0018 */ Bag *material_storage;
        /* +h001C */ Bag *unclaimed_items;
        /* +h0020 */ Bag *storage1;
        /* +h0024 */ Bag *storage2;
        /* +h0028 */ Bag *storage3;
        /* +h002C */ Bag *storage4;
        /* +h0030 */ Bag *storage5;
        /* +h0034 */ Bag *storage6;
        /* +h0038 */ Bag *storage7;
        /* +h003C */ Bag *storage8;
        /* +h0040 */ Bag *storage9;
        /* +h0044 */ Bag *storage10;
        /* +h0048 */ Bag *storage11;
        /* +h004C */ Bag *storage12;
        /* +h0050 */ Bag *storage13;
        /* +h0054 */ Bag *storage14;
        /* +h0058 */ Bag *equipped_items;
            };
        };
        /* +h005C */ Item *bundle;
        /* +h0060 */ uint32_t h0060;
        union {
        /* +h0064 */ WeapondSet weapon_sets[4];
            struct {
        /* +h0064 */ Item *weapon_set0;
        /* +h0068 */ Item *offhand_set0;
        /* +h006C */ Item *weapon_set1;
        /* +h0070 */ Item *offhand_set1;
        /* +h0074 */ Item *weapon_set2;
        /* +h0078 */ Item *offhand_set2;
        /* +h007C */ Item *weapon_set3;
        /* +h0080 */ Item *offhand_set3;
            };
        };
        /* +h0084 */ uint32_t active_weapon_set;
        /* +h0088 */ uint32_t h0088[2];
        /* +h0090 */ uint32_t gold_character;
        /* +h0094 */ uint32_t gold_storage;
    };
    static_assert(sizeof(Inventory) == 152, "struct Inventory has incorect size");

    typedef Array<ItemID> MerchItemArray;

    inline size_t Bag::find1(Constants::ModelID model_id, size_t pos) const {
        for (size_t i = pos; i < items.size(); i++) {
            Item *item = items[i];
            if (!item && model_id == Constants::ModelID::None) return i;
            if (!item) continue;
            if (item->model_id == model_id)
                return i;
        }
        return npos;
    }

    inline size_t Bag::find_dye(DyeInfo extra_id, size_t pos) const {
        for (size_t i = pos; i < items.size(); i++) {
            Item *item = items[i];
            if (!item) continue;
            if (item->model_id == Constants::ModelID::Dye && memcmp(&item->dye,&extra_id,sizeof(item->dye)) == 0)
                return i;
        }
        return npos;
    }

    // Find a similar item
    inline size_t Bag::find2(const Item *item, size_t pos) const {
        if (item->model_id == Constants::ModelID::Dye)
            return find_dye(item->dye, pos);
        else
            return find1(item->model_id, pos);
    }
}
