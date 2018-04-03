#include <GWCA\Managers\ItemMgr.h>

#include <GWCA\Context\GameContext.h>
#include <GWCA\Managers\StoCMgr.h>
#include <GWCA\Managers\CtoSMgr.h>

#include <GWCA\Utilities\Hooker.h>

void GW::Items::OpenXunlaiWindow() {
	GW::Packet::StoC::DataWindow pack;
	pack.agent = 0;
	pack.type = 0;
	pack.data = 3;
	StoC::EmulatePacket(&pack);
}

void GW::Items::PickUpItem(GW::Item* item, DWORD CallTarget /*= 0*/) {
	CtoS::SendPacket(0xC, 0x46, item->AgentId, CallTarget);
}

void GW::Items::DropItem(GW::Item* item, DWORD quantity) {
	CtoS::SendPacket(0xC, 0x33, item->ItemId, quantity);
}

void GW::Items::EquipItem(GW::Item* item) {
	CtoS::SendPacket(0x8, 0x37, item->ItemId);
}

void GW::Items::UseItem(GW::Item* item) {
	CtoS::SendPacket(0x8, 0x84, item->ItemId);
}

GW::Bag** GW::Items::GetBagArray() {
	return GameContext::instance()->items->inventory->Bags;
}

GW::Bag *GW::Items::GetBag(GW::Constants::Bag bag_id) {
	GW::Bag **bags = GetBagArray();
	if (!bags) return nullptr;
	return bags[(unsigned)bag_id];
}

GW::Bag *GW::Items::GetBag(DWORD bag_id) {
	if (bag_id >= GW::Constants::BagMax)
		return nullptr;
	GW::Bag **bags = GetBagArray();
	if (!bags) return nullptr;
	return bags[bag_id];
}

GW::Item *GW::Items::GetItemBySlot(GW::Bag *bag, DWORD slot) {
	if (!bag || slot == 0) return nullptr;
	if (!bag->Items.valid()) return nullptr;
	if (slot > bag->Items.size()) return nullptr;
	return bag->Items[slot - 1];
}

GW::Item *GW::Items::GetItemBySlot(GW::Constants::Bag bag, DWORD slot) {
	GW::Bag *bag_ptr = GetBag(bag);
	return GetItemBySlot(bag_ptr, slot);
}

GW::Item *GW::Items::GetItemBySlot(DWORD bag, DWORD slot) {
	if (bag < 0 || Constants::BagMax <= bag) return nullptr;
	return GetItemBySlot((Constants::Bag)bag, slot);
}

GW::ItemArray GW::Items::GetItemArray() {
	return GameContext::instance()->items->itemarray;
}

void GW::Items::DropGold(DWORD Amount /*= 1*/) {
	CtoS::SendPacket(0x8, 0x36, Amount);
}

DWORD GW::Items::GetGoldAmountOnCharacter() {
	return GameContext::instance()->items->inventory->GoldCharacter;
}

DWORD GW::Items::GetGoldAmountInStorage() {
	return GameContext::instance()->items->inventory->GoldStorage;
}

void GW::Items::OpenLockedChest() {
	CtoS::SendPacket(0x8, 0x5A, 0x2);
}

void GW::Items::MoveItem(GW::Item *item, GW::Bag *bag, int slot, int quantity) {
	if (slot < 0) return;
	if (!item || !bag) return;
	if (bag->Items.size() < (unsigned)slot) return;
	// @Robustness: Check if there is enough space at the destination.
	CtoS::SendPacket(0x10, 0x78, item->ItemId, bag->BagId, slot);
}

void GW::Items::MoveItem(GW::Item *from, GW::Item *to, int quantity) {
	if (!from || !to) return;
	if (!from->Bag || !to->Bag) return;
	if (quantity <= 0) quantity = from->Quantity;
	if (quantity + to->Quantity > 250) return;
	CtoS::SendPacket(0x10, 0x78, from->ItemId, to->Bag->BagId, to->Slot);
}

bool GW::Items::UseItemByModelId(DWORD modelid, int bagStart, int bagEnd) {
	GW::Bag** bags = GetBagArray();
	if (bags == NULL) return false;

	GW::Bag* bag = NULL;
	GW::Item* item = NULL;

	for (int bagIndex = bagStart; bagIndex <= bagEnd; ++bagIndex) {
		bag = bags[bagIndex];
		if (bag != NULL) {
			GW::ItemArray items = bag->Items;
			if (!items.valid()) return false;
			for (size_t i = 0; i < items.size(); i++) {
				item = items[i];
				if (item != NULL) {
					if (item->ModelId == modelid) {
						UseItem(item);
						return true;
					}
				}
			}
		}
	}

	return false;
}

DWORD GW::Items::CountItemByModelId(DWORD modelid, int bagStart, int bagEnd) {
	DWORD itemcount = 0;
	GW::Bag** bags = GetBagArray();
	GW::Bag* bag = NULL;

	for (int bagIndex = bagStart; bagIndex <= bagEnd; ++bagIndex) {
		bag = bags[bagIndex];
		if (bag != NULL) {
			GW::ItemArray items = bag->Items;
			for (size_t i = 0; i < items.size(); i++) {
				if (items[i]) {
					if (items[i]->ModelId == modelid) {
						itemcount += items[i]->Quantity;
					}
				}
			}
		}
	}

	return itemcount;
}

GW::Item* GW::Items::GetItemByModelId(DWORD modelid, int bagStart, int bagEnd) {
	GW::Bag** bags = GetBagArray();
	GW::Bag* bag = NULL;

	for (int bagIndex = bagStart; bagIndex <= bagEnd; ++bagIndex) {
		bag = bags[bagIndex];
		if (bag != NULL) {
			GW::ItemArray items = bag->Items;
			for (size_t i = 0; i < items.size(); i++) {
				if (items[i]) {
					if (items[i]->ModelId == modelid) {
						return items[i];
					}
				}
			}
		}
	}

	return NULL;
}

int GW::Items::GetCurrentStoragePannel(void) {
	static DWORD *addr;
	if (!addr) {
		DWORD **tmp = (DWORD **)Scanner::Find("\x0F\x84\xFC\x01\x00\x00\x8B\x43\x14", "xxxxxxxxx", -4);
		assert(tmp);
		addr = *tmp;
	}
	// @Cleanup: 20 being the position for the storage, but this array hold way more, for instance the current chat channel
	return addr[20];
}

typedef void (__fastcall *ItemClick_t)(uint32_t *bag_id, uint32_t edx, uint32_t *info);
static GW::THook<ItemClick_t> ItemClickHook;
static std::function<void(uint32_t type, uint32_t slot, GW::Bag *bag)> ItemClickCallback;

static void __fastcall OnItemClick(uint32_t *bag_id, uint32_t edx, uint32_t *info)
{
	// click type:
	//  2  add (when you load / open chest)
	//  5  click
	//  7  release
	//  8  double click
	//  9  move
	//  10 drag-add
	//  12 drag-remove
	uint32_t type = info[2];
	uint32_t slot = info[1] - 2; // for some reason the slot is offset by 2

	GW::Bag *bag = GW::Items::GetBag(*bag_id + 1);
	assert(bag);

	if (ItemClickCallback) {
		ItemClickCallback(type, slot, bag);
	}

	ItemClickHook.Original()(bag_id, edx, info);
}

void GW::Items::SetOnItemClick(std::function<void(uint32_t type, uint32_t slot, GW::Bag *bag)> callback) {
	if (ItemClickHook.Empty()) {
		ItemClick_t ItemClick = (ItemClick_t)Scanner::Find("\x74\x73\x8B\x50\x08", "xxxxx", -25);
		printf("[SCAN] ItemClick = %p\n", ItemClick);
		ItemClickHook.Detour(ItemClick, OnItemClick);
	}
	ItemClickCallback = callback;
}

void GW::Items::RestoreHooks() {
	HookBase::DisableHooks(&ItemClickHook);
}