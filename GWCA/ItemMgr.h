#pragma once

#include <Windows.h>

#include "GWCAManager.h"
#include "GWStructures.h"

namespace GWCA {

	class ItemMgr : public GWCAManager<ItemMgr> {
		friend class GWCAManager<ItemMgr>;

	public:

		// Get full array of items sorted by ItemID.
		GW::ItemArray GetItemArray();

		// Get bag array [1-4] = inventory bags, [8-16] = storage, more in gr wiki.
		GW::Bag** GetBagArray();

		// Use given item if usable.
		void UseItem(GW::Item* item);

		// Find item in selected bags with said modelid, then use it.
		// return: True if found and used, false if not.
		bool UseItemByModelId(DWORD modelid, BYTE bagStart = 1, const BYTE bagEnd = 4);

		// Returns the amount of item with said modelid in given bags.
		DWORD CountItemByModelId(DWORD modelid, BYTE bagStart = 1, const BYTE bagEnd = 4);

		// Returns item struct of item with given modelid.
		GW::Item* GetItemByModelId(DWORD modelid, BYTE bagStart = 1, const BYTE bagEnd = 4);

		// Equip item if equippable.
		void EquipItem(GW::Item* item);

		// Drop item if droppable.
		void DropItem(GW::Item* item, DWORD quantity);

		// Pick up selected item off the ground.
		void PickUpItem(GW::Item* item, DWORD CallTarget = 0);

		// Opens the storage window from anywhere.
		// Can only interact with the storage in an outpost with a xunlai chest inside, sorry no exploiting.
		void OpenXunlaiWindow();

		// Drop amount gold on ground.
		void DropGold(DWORD Amount = 1);

		// Get amount of gold on character.
		DWORD GetGoldAmountOnCharacter();

		// Get amount of gold in storage.
		DWORD GetGoldAmountInStorage();

		// Open locked chest, raw packet, first send a GoSignpost packet to select chest.
		void OpenLockedChest();

	private:

		ItemMgr();
		void RestoreHooks() override {};
	};
}