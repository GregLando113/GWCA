#pragma once

#include <GWCA/Utilities/Hook.h>
#include <GWCA/Utilities/Export.h>

namespace GW {

    struct Bag;
    struct Item;

    typedef Array<Item *> ItemArray;

    namespace Constants {
        enum class Bag;
        enum EquipmentStatus : uint32_t;
    }

    struct Module;
    extern Module ItemModule;

    namespace Items {
        // Get full array of items sorted by ItemID.
        GWCA_API ItemArray GetItemArray();

        // Get bag array [1-4] = inventory bags, [5]=Equip pack, [6]=Materials storage,
        // [7] = Unclaimed items, [8-16] = storage, [17] = Equipped
        // Note: bag->index of each bag is one less than its index in the array
        GWCA_API Bag **GetBagArray();
        GWCA_API Bag  *GetBag(Constants::Bag bag_id);
        GWCA_API Bag  *GetBag(uint32_t bag_id);

        // bag & slot are 1 based
        GWCA_API Item *GetItemBySlot(const Bag *bag, uint32_t slot);
        GWCA_API Item *GetItemBySlot(Constants::Bag bag, uint32_t slot);
        GWCA_API Item *GetItemBySlot(uint32_t bag, uint32_t slot);

        GWCA_API Item *GetHoveredItem();

        GWCA_API Item *GetItemById(uint32_t item_id);

        // Use given item if usable.
        GWCA_API void UseItem(const Item *item);

        // Equip item if equippable.
        GWCA_API void EquipItem(const Item *item);

        // Drop item if droppable.
        GWCA_API void DropItem(const Item *item, uint32_t quantity);

        // Pick up selected item off the ground.
        GWCA_API void PickUpItem(const Item *item, uint32_t call_target = 0);

        // Opens the storage window from anywhere.
        // Can only interact with the storage in an outpost with a xunlai chest inside, sorry no exploiting.
        GWCA_API void OpenXunlaiWindow();

        // Drop amount gold on ground.
        GWCA_API void DropGold(uint32_t amount = 1);

        // Get amount of gold on character.
        GWCA_API uint32_t GetGoldAmountOnCharacter();

        // Get amount of gold in storage.
        GWCA_API uint32_t GetGoldAmountInStorage();

        // Deposit to the storage the given amount of gold. "0" is all you can.
        GWCA_API uint32_t DepositGold(uint32_t amount = 0);

        // Withdraw from the storage the given amount of gold. "0" is all you can.
        GWCA_API uint32_t WithdrawGold(uint32_t amount = 0);

        // Open locked chest, raw packet, first send a GoSignpost packet to select chest.
        GWCA_API void OpenLockedChest();
        
        // Move item to a new position (bag, slot). Used to merge stacks
        // slot start at 0
        GWCA_API void MoveItem(const Item *from, Constants::Bag bag_id, uint32_t slot, uint32_t quantity = 0);
        GWCA_API void MoveItem(const Item *item, const Bag *bag, uint32_t slot, uint32_t quantity = 0);
        GWCA_API void MoveItem(const Item *from, const Item *to, uint32_t quantity = 0);

        // Split a given amount into a new position (bag, slot).
        // GWCA_API void SplitStack(Item *item, Bag *bag, int slot, int quantity);

        // === Complex functions ===
        // Find item in selected bags with said modelid, then use it.
        // return: True if found and used, false if not.
        GWCA_API bool UseItemByModelId(uint32_t model_id, int bag_start = 1, int bag_end = 4);

        // Returns the amount of item with said modelid in given bags.
        GWCA_API uint32_t CountItemByModelId(uint32_t model_id, int bag_start = 1, int bag_end = 4);

        // Returns item struct of item with given modelid.
        GWCA_API Item *GetItemByModelId(uint32_t model_id, int bag_start = 1, int bag_end = 4);

        // Returns the current storage pannel (0 based). Note that if material storage is on focus, 0 is returned.
        GWCA_API uint32_t GetStoragePage(void);

        GWCA_API bool GetIsStorageOpen(void);

        // The callback should return false if it want to foward the info to Gw.
        typedef HookCallback<uint32_t, uint32_t, Bag *> ItemClickCallback;
        GWCA_API void RegisterItemClickCallback(
            HookEntry *entry,
            ItemClickCallback callback);

        GWCA_API void RemoveItemClickCallback(
            HookEntry *entry);

        // Returns the slot of the materials in the storage page. (-1 if not found)
        GWCA_API int GetMaterialSlot(uint32_t model_id);
        GWCA_API int GetMaterialSlot(const Item *item);

        GWCA_API void AsyncGetItemByName(const Item *item, std::wstring& name);

        GWCA_API Constants::EquipmentStatus GetCapeStatus();
        GWCA_API Constants::EquipmentStatus GetHelmStatus();
        GWCA_API Constants::EquipmentStatus GetCostumeBodyStatus();
        GWCA_API Constants::EquipmentStatus GetCostumeHeadpieceStatus();
        GWCA_API void SetCapeStatus(Constants::EquipmentStatus);
        GWCA_API void SetHelmStatus(Constants::EquipmentStatus);
        GWCA_API void SetCostumeBodyStatus(Constants::EquipmentStatus);
        GWCA_API void SetCostumeHeadpieceStatus(Constants::EquipmentStatus);

    };
}
