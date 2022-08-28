#pragma once

#include <GWCA/Utilities/Hook.h>
#include <GWCA/Utilities/Export.h>
#include <GWCA/GameContainers/Array.h>

namespace GW {

    struct Bag;
    struct Item;
    struct Inventory;

    typedef Array<Item *> ItemArray;

    namespace Constants {
        enum class Bag;
    }

    enum class EquipmentType : uint32_t {
        Cape = 0x0, Helm = 0x2, CostumeBody = 0x4, CostumeHeadpiece = 0x6, Unknown = 0xff
    };
    enum class EquipmentStatus : uint32_t {
        AlwaysHide, HideInTownsAndOutposts, HideInCombatAreas, AlwaysShow
    };

    struct Module;
    extern Module ItemModule;

    namespace Items {
        // Get full array of items sorted by ItemID.
        GWCA_API ItemArray* GetItemArray();

        GWCA_API Inventory* GetInventory();

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
        GWCA_API bool UseItem(const Item *item);

        // Equip item if equippable. Pass agent id of the hero you'd like to put the item on, or 0 for current player
        GWCA_API bool EquipItem(const Item *item, uint32_t agent_id = 0);

        // Drop item if droppable.
        GWCA_API bool DropItem(const Item *item, uint32_t quantity);

        // Ping weapon/offhand for an agent
        GWCA_API bool PingWeaponSet(uint32_t agent_id, uint32_t weapon_item_id, uint32_t offhand_item_id);

        // Pick up selected item off the ground.
        GWCA_API bool PickUpItem(const Item *item, uint32_t call_target = 0);

        // Opens the storage window from anywhere.
        // Can only interact with the storage in an outpost with a xunlai chest inside, sorry no exploiting.
        GWCA_API void OpenXunlaiWindow();

        // Drop amount gold on ground.
        GWCA_API bool DropGold(uint32_t amount = 1);

        // Begin the salvage process for an item
        GWCA_API bool SalvageStart(uint32_t salvage_kit_id, uint32_t item_id);

        // Identify an item
        GWCA_API bool IdentifyItem(uint32_t identification_kit_id, uint32_t item_id);

        // Cancel the current salvage session
        GWCA_API bool SalvageSessionCancel();

        // Acknowledge that the salvage session is complete
        GWCA_API bool SalvageSessionDone();

        // Choose to salvage materials from a prompted item.
        GWCA_API bool SalvageMaterials();

        // Get amount of gold on character.
        GWCA_API uint32_t GetGoldAmountOnCharacter();

        // Get amount of gold in storage.
        GWCA_API uint32_t GetGoldAmountInStorage();

        // Deposit to the storage the given amount of gold. "0" is all you can.
        GWCA_API uint32_t DepositGold(uint32_t amount = 0);

        // Withdraw from the storage the given amount of gold. "0" is all you can.
        GWCA_API uint32_t WithdrawGold(uint32_t amount = 0);

        // Open locked chest; requires valid target. use_key = true to use a key if available rather than lockpick
        GWCA_API bool OpenLockedChest(bool use_key = true);
        
        // Move item to a new position (bag, slot). Used to merge stacks
        // slot start at 0
        GWCA_API bool MoveItem(const Item *from, Constants::Bag bag_id, uint32_t slot, uint32_t quantity = 0);
        GWCA_API bool MoveItem(const Item *item, const Bag *bag, uint32_t slot, uint32_t quantity = 0);
        GWCA_API bool MoveItem(const Item *from, const Item *to, uint32_t quantity = 0);

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
            const ItemClickCallback& callback);

        GWCA_API void RemoveItemClickCallback(
            HookEntry *entry);

        // Returns the slot of the materials in the storage page. (-1 if not found)
        GWCA_API int GetMaterialSlot(uint32_t model_id);
        GWCA_API int GetMaterialSlot(const Item *item);

        GWCA_API void AsyncGetItemByName(const Item *item, std::wstring& name);

        GWCA_API EquipmentStatus GetEquipmentVisibility(EquipmentType type);
        GWCA_API bool SetEquipmentVisibility(EquipmentType type, EquipmentStatus state);
    };
}
