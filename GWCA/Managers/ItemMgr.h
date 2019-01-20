#pragma once

namespace GW {

    namespace Items {
        GWCA_API void RestoreHooks();

        // Get full array of items sorted by ItemID.
        GWCA_API ItemArray GetItemArray();

        // Get bag array [1-4] = inventory bags, [5]=Equip pack, [6]=Materials storage,
        // [7] = Unclaimed items, [8-16] = storage, [17] = Equipped
        // Note: bag->index of each bag is one less than its index in the array
        GWCA_API Bag **GetBagArray();
        GWCA_API Bag  *GetBag(Constants::Bag bag_id);
        GWCA_API Bag  *GetBag(DWORD bag_id);

        // bag & slot are 1 based
        GWCA_API Item *GetItemBySlot(Bag *bag, DWORD slot);
        GWCA_API Item *GetItemBySlot(Constants::Bag bag, DWORD slot);
        GWCA_API Item *GetItemBySlot(DWORD bag, DWORD slot);

        // Use given item if usable.
        GWCA_API void UseItem(Item* item);

        // Equip item if equippable.
        GWCA_API void EquipItem(Item* item);

        // Drop item if droppable.
        GWCA_API void DropItem(Item* item, DWORD quantity);

        // Pick up selected item off the ground.
        GWCA_API void PickUpItem(Item* item, DWORD CallTarget = 0);

        // Opens the storage window from anywhere.
        // Can only interact with the storage in an outpost with a xunlai chest inside, sorry no exploiting.
        GWCA_API void OpenXunlaiWindow();

        // Drop amount gold on ground.
        GWCA_API void DropGold(DWORD Amount = 1);

        // Get amount of gold on character.
        GWCA_API DWORD GetGoldAmountOnCharacter();

        // Get amount of gold in storage.
        GWCA_API DWORD GetGoldAmountInStorage();

        // Deposit to the storage the given amount of gold. "0" is all you can.
        GWCA_API DWORD DepositGold(DWORD amount = 0);

        // Withdraw from the storage the given amount of gold. "0" is all you can.
        GWCA_API DWORD WithdrawGold(DWORD amount = 0);

        // Open locked chest, raw packet, first send a GoSignpost packet to select chest.
        GWCA_API void OpenLockedChest();
        
        // Move item to a new position (bag, slot). Used to merge stacks
        // slot start at 0
        GWCA_API void MoveItem(Item *from, Constants::Bag bag_id, int slot, int quantity = 0);
        GWCA_API void MoveItem(Item *item, Bag *bag, int slot, int quantity = 0);
        GWCA_API void MoveItem(Item *from, Item *to, int quantity = 0);

        // Split a given amount into a new position (bag, slot).
        // GWCA_API void SplitStack(GW::Item *item, GW::Bag *bag, int slot, int quantity);

        // === Complex functions ===
        // Find item in selected bags with said modelid, then use it.
        // return: True if found and used, false if not.
        GWCA_API bool UseItemByModelId(DWORD modelid, int bagStart = 1, int bagEnd = 4);

        // Returns the amount of item with said modelid in given bags.
        GWCA_API DWORD CountItemByModelId(DWORD modelid, int bagStart = 1, int bagEnd = 4);

        // Returns item struct of item with given modelid.
        GWCA_API Item* GetItemByModelId(DWORD modelid, int bagStart = 1, int bagEnd = 4);

        // Returns the current storage pannel (0 based). Note that if material storage is on focus, 0 is returned.
        GWCA_API int GetStoragePage(void);

        GWCA_API bool IsStorageOpen(void);

        // The callback should return false if it want to foward the info to Gw.
        GWCA_API void SetOnItemClick(std::function<void(uint32_t type, uint32_t slot, Bag *bag)> callback);

        // Returns the slot of the materials in the storage page. (-1 if not found)
        GWCA_API int GetMaterialSlot(DWORD model_id);
        GWCA_API int GetMaterialSlot(Item *item);

        GWCA_API void AsyncGetItemByName(GW::Item *item, std::wstring& name);
    };
}
