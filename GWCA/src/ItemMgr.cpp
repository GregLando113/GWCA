#include <GWCA\Managers\ItemMgr.h>

#include <GWCA\Context\GameContext.h>
#include <GWCA\Managers\StoCMgr.h>
#include <GWCA\Managers\CtoSMgr.h>

void GW::Items::OpenXunlaiWindow() {
	static DWORD ecxbuf[4] = { 119, 0, 0, 3 };
	StoC::EmulatePacket((Packet::StoC::PacketBase*)ecxbuf);
}

void GW::Items::PickUpItem(GW::Item* item, DWORD CallTarget /*= 0*/) {
	CtoS::SendPacket(0xC, 0x39, item->AgentId, CallTarget);
}

void GW::Items::DropItem(GW::Item* item, DWORD quantity) {
	CtoS::SendPacket(0xC, 0x26, item->ItemId, quantity);
}

void GW::Items::EquipItem(GW::Item* item) {
	CtoS::SendPacket(0x8, 0x2A, item->ItemId);
}

void GW::Items::UseItem(GW::Item* item) {
	CtoS::SendPacket(0x8, 0x78, item->ItemId);
}

GW::Bag** GW::Items::GetBagArray() {
	return GameContext::instance()->items->inventory->bags;
}

GW::ItemArray GW::Items::GetItemArray() {
	return GameContext::instance()->items->itemarray;
}

void GW::Items::DropGold(DWORD Amount /*= 1*/) {
	CtoS::SendPacket(0x8, 0x29, Amount);
}

DWORD GW::Items::GetGoldAmountOnCharacter() {
	return GameContext::instance()->items->inventory->gold_character;
}

DWORD GW::Items::GetGoldAmountInStorage() {
	return GameContext::instance()->items->inventory->gold_storage;
}

void GW::Items::OpenLockedChest() {
	CtoS::SendPacket(0x8, 0x4D, 0x2);
}

bool GW::Items::UseItemByModelId(DWORD modelid, BYTE bagStart /*= 1*/, const BYTE bagEnd /*= 4*/) {
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

DWORD GW::Items::CountItemByModelId(DWORD modelid, BYTE bagStart /*= 1*/, const BYTE bagEnd /*= 4*/) {
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

GW::Item* GW::Items::GetItemByModelId(DWORD modelid, BYTE bagStart /*= 1*/, const BYTE bagEnd /*= 4*/) {
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
