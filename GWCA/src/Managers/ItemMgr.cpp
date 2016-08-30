#include "..\..\Managers\ItemMgr.h"

#include "..\..\Constants\Constants.h"

#include "..\..\Managers\GameThreadMgr.h"
#include "..\..\Managers\StoCMgr.h"
#include "..\..\Managers\CtoSMgr.h"
#include "..\..\Managers\MapMgr.h"
#include "..\..\Managers\MemoryMgr.h"

void GWCA::ItemMgr::OpenXunlaiWindow() {
	static DWORD ecxbuf[4] = { 119, 0, 0, 3 };
	StoCMgr::Instance().EmulatePacket((Packet::StoC::PacketBase*)ecxbuf);
}

void GWCA::ItemMgr::PickUpItem(GW::Item* item, DWORD CallTarget /*= 0*/) {
	CtoSMgr::Instance().SendPacket(0xC, 0x39, item->AgentId, CallTarget);
}

void GWCA::ItemMgr::DropItem(GW::Item* item, DWORD quantity) {
	CtoSMgr::Instance().SendPacket(0xC, 0x26, item->ItemId, quantity);
}

void GWCA::ItemMgr::EquipItem(GW::Item* item) {
	CtoSMgr::Instance().SendPacket(0x8, 0x2A, item->ItemId);
}

void GWCA::ItemMgr::UseItem(GW::Item* item) {
	CtoSMgr::Instance().SendPacket(0x8, 0x78, item->ItemId);
}

GWCA::GW::Bag** GWCA::ItemMgr::GetBagArray() {
	return *MemoryMgr::ReadPtrChain<GW::Bag***>(MemoryMgr::GetContextPtr(), 2, 0x40, 0xF8);
}

GWCA::GW::ItemArray GWCA::ItemMgr::GetItemArray() {
	return *MemoryMgr::ReadPtrChain<GW::ItemArray*>(MemoryMgr::GetContextPtr(), 2, 0x40, 0xB8);
}

GWCA::ItemMgr::ItemMgr() {
}

bool GWCA::ItemMgr::UseItemByModelId(DWORD modelid, BYTE bagStart /*= 1*/, const BYTE bagEnd /*= 4*/) {

	if (MapMgr::Instance().GetInstanceType() == GwConstants::InstanceType::Loading) return false;

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

void GWCA::ItemMgr::DropGold(DWORD Amount /*= 1*/) {
	CtoSMgr::Instance().SendPacket(0x8, 0x29, Amount);
}

DWORD GWCA::ItemMgr::CountItemByModelId(DWORD modelid, BYTE bagStart /*= 1*/, const BYTE bagEnd /*= 4*/) {
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

GWCA::GW::Item* GWCA::ItemMgr::GetItemByModelId(DWORD modelid, BYTE bagStart /*= 1*/, const BYTE bagEnd /*= 4*/) {
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

DWORD GWCA::ItemMgr::GetGoldAmountOnCharacter() {
	return *MemoryMgr::ReadPtrChain<DWORD*>(MemoryMgr::GetContextPtr(), 3, 0x40, 0xF8, 0x7C);
}

DWORD GWCA::ItemMgr::GetGoldAmountInStorage() {
	return *MemoryMgr::ReadPtrChain<DWORD*>(MemoryMgr::GetContextPtr(), 3, 0x40, 0xF8, 0x80);
}

void GWCA::ItemMgr::OpenLockedChest() {
	return CtoSMgr::Instance().SendPacket(0x8, 0x4D, 0x2);
}
