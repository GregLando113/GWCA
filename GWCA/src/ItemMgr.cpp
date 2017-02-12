#include <GWCA\Managers\ItemMgr.h>

#include <GWCA\Constants\Constants.h>

#include <GWCA\Context\GameContext.h>
#include <GWCA\Context\ItemContext.h>
#include <GWCA\Managers\GameThreadMgr.h>
#include <GWCA\Managers\StoCMgr.h>
#include <GWCA\Managers\CtoSMgr.h>
#include <GWCA\Managers\MapMgr.h>
#include <GWCA\Managers\MemoryMgr.h>

void GW::ItemMgr::OpenXunlaiWindow() {
	static DWORD ecxbuf[4] = { 119, 0, 0, 3 };
	StoCMgr::Instance().EmulatePacket((Packet::StoC::PacketBase*)ecxbuf);
}

void GW::ItemMgr::PickUpItem(GW::Item* item, DWORD CallTarget /*= 0*/) {
	CtoSMgr::Instance().SendPacket(0xC, 0x39, item->AgentId, CallTarget);
}

void GW::ItemMgr::DropItem(GW::Item* item, DWORD quantity) {
	CtoSMgr::Instance().SendPacket(0xC, 0x26, item->ItemId, quantity);
}

void GW::ItemMgr::EquipItem(GW::Item* item) {
	CtoSMgr::Instance().SendPacket(0x8, 0x2A, item->ItemId);
}

void GW::ItemMgr::UseItem(GW::Item* item) {
	CtoSMgr::Instance().SendPacket(0x8, 0x78, item->ItemId);
}

GW::Bag** GW::ItemMgr::GetBagArray() {
	return GameContext::instance()->items->inventory->bags;
}

GW::ItemArray GW::ItemMgr::GetItemArray() {
	return GameContext::instance()->items->itemarray;
}

GW::ItemMgr::ItemMgr() {}

bool GW::ItemMgr::UseItemByModelId(DWORD modelid, BYTE bagStart /*= 1*/, const BYTE bagEnd /*= 4*/) {

	if (MapMgr::Instance().GetInstanceType() == GW::Constants::InstanceType::Loading) return false;

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

void GW::ItemMgr::DropGold(DWORD Amount /*= 1*/) {
	CtoSMgr::Instance().SendPacket(0x8, 0x29, Amount);
}

DWORD GW::ItemMgr::CountItemByModelId(DWORD modelid, BYTE bagStart /*= 1*/, const BYTE bagEnd /*= 4*/) {
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

GW::Item* GW::ItemMgr::GetItemByModelId(DWORD modelid, BYTE bagStart /*= 1*/, const BYTE bagEnd /*= 4*/) {
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

DWORD GW::ItemMgr::GetGoldAmountOnCharacter() {
	return GameContext::instance()->items->inventory->gold_character;
}

DWORD GW::ItemMgr::GetGoldAmountInStorage() {
	return GameContext::instance()->items->inventory->gold_storage;
}

void GW::ItemMgr::OpenLockedChest() {
	return CtoSMgr::Instance().SendPacket(0x8, 0x4D, 0x2);
}
