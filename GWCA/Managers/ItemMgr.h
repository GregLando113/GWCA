#pragma once

#include <Windows.h>

#include <GWCA\GameEntities\Item.h>
#include <functional>

namespace GW {

	namespace Items {

		// Get full array of items sorted by ItemID.
		GWCA_API GW::ItemArray GetItemArray();

		// Get bag array [1-4] = inventory bags, [5]=Equip pack, [6]=Materials storage,
		// [7] = Unclaimed items, [8-16] = storage, [17] = Equipped
		// Note: bag->index of each bag is one less than its index in the array
		GWCA_API GW::Bag** GetBagArray();

		// Use given item if usable.
		GWCA_API void UseItem(GW::Item* item);

		// Equip item if equippable.
		GWCA_API void EquipItem(GW::Item* item);

		// Drop item if droppable.
		GWCA_API void DropItem(GW::Item* item, DWORD quantity);

		// Pick up selected item off the ground.
		GWCA_API void PickUpItem(GW::Item* item, DWORD CallTarget = 0);

		// Opens the storage window from anywhere.
		// Can only interact with the storage in an outpost with a xunlai chest inside, sorry no exploiting.
		GWCA_API void OpenXunlaiWindow();

		// Drop amount gold on ground.
		GWCA_API void DropGold(DWORD Amount = 1);

		// Get amount of gold on character.
		GWCA_API DWORD GetGoldAmountOnCharacter();

		// Get amount of gold in storage.
		GWCA_API DWORD GetGoldAmountInStorage();

		// Open locked chest, raw packet, first send a GoSignpost packet to select chest.
		GWCA_API void OpenLockedChest();

		// === Complex functions ===
		// Find item in selected bags with said modelid, then use it.
		// return: True if found and used, false if not.
		GWCA_API bool UseItemByModelId(DWORD modelid, int bagStart = 1, int bagEnd = 4);

		// Returns the amount of item with said modelid in given bags.
		GWCA_API DWORD CountItemByModelId(DWORD modelid, int bagStart = 1, int bagEnd = 4);

		// Returns item struct of item with given modelid.
		GWCA_API GW::Item* GetItemByModelId(DWORD modelid, int bagStart = 1, int bagEnd = 4);

		// The callback should return false if it want to foward the info to Gw.
		GWCA_API void SetOnItemClick(std::function<bool(GW::Item*, GW::Bag*)> callback);
	};
}
