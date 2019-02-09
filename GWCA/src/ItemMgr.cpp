#include "stdafx.h"

#include <GWCA/CtoSHeaders.h>
#include <GWCA/Constants/Constants.h>

#include <GWCA/Utilities/Export.h>
#include <GWCA/Utilities/Hooker.h>

#include <GWCA/GameContainers/Vector.h>
#include <GWCA/Packets/StoC.h>

#include <GWCA/GameEntities/Item.h>
#include <GWCA/GameEntities/Agent.h>

#include <GWCA/Context/GameContext.h>
#include <GWCA/Context/ItemContext.h>

#include <GWCA/Managers/UIMgr.h>
#include <GWCA/Managers/ItemMgr.h>
#include <GWCA/Managers/CtoSMgr.h>
#include <GWCA/Managers/StoCMgr.h>

namespace GW {
    void Items::OpenXunlaiWindow() {
        Packet::StoC::DataWindow pack;
        pack.agent = 0;
        pack.type = 0;
        pack.data = 3;
        StoC::EmulatePacket(&pack);
    }

    void Items::PickUpItem(Item *item, uint32_t CallTarget /*= 0*/) {
        CtoS::SendPacket(0xC, CtoGS_MSGPickUpItem, item->agent_id, CallTarget);
    }

    void Items::DropItem(Item *item, uint32_t quantity) {
        CtoS::SendPacket(0xC, CtoGS_MSGDropItem, item->item_id, quantity);
    }

    void Items::EquipItem(Item *item) {
        CtoS::SendPacket(0x8, CtoGS_MSGEquipItem, item->item_id);
    }

    void Items::UseItem(Item *item) {
        CtoS::SendPacket(0x8, CtoGS_MSGUseItem, item->item_id);
    }

    Bag **Items::GetBagArray() {
        return GameContext::instance()->items->inventory->bags;
    }

    Bag *Items::GetBag(Constants::Bag bag_id) {
        Bag **bags = GetBagArray();
        if (!bags) return nullptr;
        return bags[(unsigned)bag_id];
    }

    Bag *Items::GetBag(uint32_t bag_id) {
        if (bag_id >= Constants::BagMax)
            return nullptr;
        Bag **bags = GetBagArray();
        if (!bags) return nullptr;
        return bags[bag_id];
    }

    Item *Items::GetItemBySlot(Bag *bag, uint32_t slot) {
        if (!bag || slot == 0) return nullptr;
        if (!bag->items.valid()) return nullptr;
        if (slot > bag->items.size()) return nullptr;
        return bag->items[slot - 1];
    }

    Item *Items::GetItemBySlot(Constants::Bag bag, uint32_t slot) {
        Bag *bag_ptr = GetBag(bag);
        return GetItemBySlot(bag_ptr, slot);
    }

    Item *Items::GetItemBySlot(uint32_t bag, uint32_t slot) {
        if (bag < 0 || Constants::BagMax <= bag) return nullptr;
        return GetItemBySlot((Constants::Bag)bag, slot);
    }

    ItemArray Items::GetItemArray() {
        return GameContext::instance()->items->item_array;
    }

    void Items::DropGold(uint32_t Amount /*= 1*/) {
        CtoS::SendPacket(0x8, CtoGS_MSGDropGold, Amount);
    }

    uint32_t Items::GetGoldAmountOnCharacter() {
        return GameContext::instance()->items->inventory->gold_character;
    }

    uint32_t Items::GetGoldAmountInStorage() {
        return GameContext::instance()->items->inventory->gold_storage;
    }

    static void ChangeGold(uint32_t character_gold, uint32_t storage_gold) {
        CtoS::SendPacket(0xC, CtoGS_MSGChangeGold, character_gold, storage_gold);
    }

    uint32_t Items::DepositGold(uint32_t amount) {
        uint32_t gold_storage = GetGoldAmountInStorage();
        uint32_t gold_character = GetGoldAmountOnCharacter();
        uint32_t will_move = 0;
        if (amount == 0) {
            will_move = std::min(1000000 - gold_storage, gold_character);
        } else {
            if (gold_storage + amount > 1000000)
                return 0;
            if (amount > gold_character)
                return 0;
            will_move = amount;
        }
        gold_storage += will_move;
        gold_character -= will_move;
        ChangeGold(gold_character, gold_storage);
        return will_move;
    }

    uint32_t Items::WithdrawGold(uint32_t amount) {
        uint32_t gold_storage = GetGoldAmountInStorage();
        uint32_t gold_character = GetGoldAmountOnCharacter();
        uint32_t will_move = 0;
        if (amount == 0) {
            will_move = std::min(gold_storage, 100000 - gold_character);
        } else {
            if (gold_character + amount > 100000)
                return 0;
            if (amount > gold_storage)
                return 0;
            will_move = amount;
        }
        gold_storage -= will_move;
        gold_character += will_move;
        ChangeGold(gold_character, gold_storage);
        return will_move;
    }

    void Items::OpenLockedChest() {
        CtoS::SendPacket(0x8, CtoGS_MSGOpenChest, 0x2);
    }

    void Items::MoveItem(Item *item, Bag *bag, int slot, int quantity) {
        if (slot < 0) return;
        if (!item || !bag) return;
        if (bag->items.size() < (unsigned)slot) return;
        // @Robustness: Check if there is enough space at the destination.
        CtoS::SendPacket(0x10, CtoGS_MSGMoveItem, item->item_id, bag->bag_id, slot);
    }

    void Items::MoveItem(Item *item, Constants::Bag bag_id, int slot, int quantity)
    {
        Bag *bag = GetBag(bag_id);
        if (!bag) return;
        MoveItem(item, bag, slot, quantity);
    }

    void Items::MoveItem(Item *from, Item *to, int quantity) {
        if (!from || !to) return;
        if (!from->bag || !to->bag) return;
        if (quantity <= 0) quantity = from->quantity;
        if (quantity + to->quantity > 250) return;
        CtoS::SendPacket(0x10, CtoGS_MSGMoveItem, from->item_id, to->bag->bag_id, to->slot);
    }

    bool Item::GetIsStackable() {
        switch (type) {
        case 0:  return false; // Salvage
        case 1:  return false; // LeadHand
        case 2:  return false; // Axe
        case 3:  return false; // Bag
        case 4:  return false; // Feet
        case 5:  return false; // Bow
        case 6:  return false; // Bundle
        case 7:  return false; // Chest
        case 8:  return false; // Rune
        case 9:  return true;  // Consumable
        case 10: return true;  // Dye
        case 11: return true;  // Material
        case 12: return false; // Focus
        case 13: return false; // Arms
        case 14: return true;  // Sigil
        case 15: return false; // Hammer
        case 16: return false; // Head
        case 17: return false; // SalvageItem
        case 18: return true;  // Key
        case 19: return false; // Legs
        case 20: return true;  // Coins
        case 21: return false; // QuestItem
        case 22: return false; // Wand
        case 24: return false; // Shield
        case 26: return false; // Staff
        case 27: return false; // Sword
        case 29: return false; // Kit
        case 30: return true;  // Trophy
        case 31: return true;  // Scroll
        case 32: return false; // Daggers
        case 33: return true;  // Present
        case 34: return false; // Minipet
        case 35: return false; // Scythe
        case 36: return false; // Spear
        case 43: return false; // Handbook
        case 44: return false; // CostumeBody
        case 45: return false; // CostumeHead
        };
        return false;
    }

    bool Item::GetIsZcoin() {
        if (model_file_id == 31202) return true; // Copper
        if (model_file_id == 31203) return true; // Gold
        if (model_file_id == 31204) return true; // Silver
        return false;
    }

    bool Item::GetIsMaterial() {
        if (type == (uint32_t)Constants::ItemType::Materials_Zcoins
            && !GetIsZcoin()) {
            return true;
        }
        return false;
    }

    int Items::GetMaterialSlot(uint32_t model_id) {
        switch (model_id) {
        case 921: return Constants::Bone;
        case 948: return Constants::IronIngot;
        case 940: return Constants::TannedHideSquare;
        case 953: return Constants::Scale;
        case 954: return Constants::ChitinFragment;
        case 925: return Constants::BoltofCloth;
        case 946: return Constants::WoodPlank;
        case 955: return Constants::GraniteSlab;
        case 929: return Constants::PileofGlitteringDust;
        case 934: return Constants::PlantFiber;
        case 933: return Constants::Feather;
        // rare
        case 941: return Constants::FurSquare;
        case 926: return Constants::BoltofLinen;
        case 927: return Constants::BoltofDamask;
        case 928: return Constants::BoltofSilk;
        case 930: return Constants::GlobofEctoplasm;
        case 949: return Constants::SteelIngot;
        case 950: return Constants::DeldrimorSteelIngot;
        case 923: return Constants::MonstrousClaw;
        case 931: return Constants::MonstrousEye;
        case 932: return Constants::MonstrousFang;
        case 937: return Constants::Ruby;
        case 938: return Constants::Sapphire;
        case 935: return Constants::Diamond;
        case 936: return Constants::OnyxGemstone;
        case 922: return Constants::LumpofCharcoal;
        case 945: return Constants::ObsidianShard;
        case 939: return Constants::TemperedGlassVial;
        case 942: return Constants::LeatherSquare;
        case 943: return Constants::ElonianLeatherSquare;
        case 944: return Constants::VialofInk;
        case 951: return Constants::RollofParchment;
        case 952: return Constants::RollofVellum;
        case 956: return Constants::SpiritwoodPlank;
        case 6532: return Constants::AmberChunk;
        case 6533: return Constants::JadeiteShard;
        };
        return -1;
    }

    int Items::GetMaterialSlot(Item *item) {
        if (!item) return -1;
        if (!item->GetIsMaterial()) return -1;
        return GetMaterialSlot(item->model_id);
    }

    bool Items::UseItemByModelId(uint32_t modelid, int bagStart, int bagEnd) {
        Bag **bags = GetBagArray();
        if (bags == NULL) return false;

        Bag *bag = NULL;
        Item *item = NULL;

        for (int bagIndex = bagStart; bagIndex <= bagEnd; ++bagIndex) {
            bag = bags[bagIndex];
            if (bag != NULL) {
                ItemArray items = bag->items;
                if (!items.valid()) return false;
                for (size_t i = 0; i < items.size(); i++) {
                    item = items[i];
                    if (item != NULL) {
                        if (item->model_id == modelid) {
                            UseItem(item);
                            return true;
                        }
                    }
                }
            }
        }

        return false;
    }

    uint32_t Items::CountItemByModelId(uint32_t modelid, int bagStart, int bagEnd) {
        uint32_t itemcount = 0;
        Bag **bags = GetBagArray();
        Bag  *bag = NULL;

        for (int bagIndex = bagStart; bagIndex <= bagEnd; ++bagIndex) {
            bag = bags[bagIndex];
            if (bag != NULL) {
                ItemArray items = bag->items;
                for (size_t i = 0; i < items.size(); i++) {
                    if (items[i]) {
                        if (items[i]->model_id == modelid) {
                            itemcount += items[i]->quantity;
                        }
                    }
                }
            }
        }

        return itemcount;
    }

    Item* Items::GetItemByModelId(uint32_t modelid, int bagStart, int bagEnd) {
        Bag **bags = GetBagArray();
        Bag  *bag = NULL;

        for (int bagIndex = bagStart; bagIndex <= bagEnd; ++bagIndex) {
            bag = bags[bagIndex];
            if (bag != NULL) {
                ItemArray items = bag->items;
                for (size_t i = 0; i < items.size(); i++) {
                    if (items[i]) {
                        if (items[i]->model_id == modelid) {
                            return items[i];
                        }
                    }
                }
            }
        }

        return NULL;
    }

    int Items::GetStoragePage(void) {
        static uint32_t *addr;
        if (!addr) {
            uintptr_t storage_pannel_addr = Scanner::Find(
                "\x0F\x84\xFC\x01\x00\x00\x8B\x43\x14", "xxxxxxxxx", -4);
            printf("[SCAN] StoragePannel = %08lX\n", storage_pannel_addr);
            if (storage_pannel_addr) addr = *(uint32_t **)storage_pannel_addr;
        }
        assert(addr);
        // @Cleanup: 20 being the position for the storage, but this array hold way more, for instance the current chat channel
        return addr[20];
    }

    bool Items::GetIsStorageOpen(void) {
        static uint32_t *addr;
        if (!addr) {
            uintptr_t storage_open_addr = Scanner::Find(
                "\x40\x85\xD2\xA3\x00\x00\x00\x00\x75\x05", "xxxx????xx", 4);
            printf("[SCAN] StorageOpen = %08lX\n", storage_open_addr);
            assert(storage_open_addr);
            addr = *(uint32_t **)storage_open_addr;
        }
        assert(addr);
        return *addr != 0;
    }

    typedef void (__fastcall *ItemClick_t)(uint32_t *bag_id, uint32_t edx, uint32_t *info);
    static THook<ItemClick_t> ItemClickHook;
    static std::function<void (uint32_t type, uint32_t slot, Bag *bag)> ItemClickCallback;

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

        Bag *bag = Items::GetBag(*bag_id + 1);
        assert(bag);

        if (ItemClickCallback) {
            ItemClickCallback(type, slot, bag);
        }

        ItemClickHook.Original()(bag_id, edx, info);
    }

    void Items::SetOnItemClick(std::function<void(uint32_t type, uint32_t slot, Bag *bag)> callback) {
        if (ItemClickHook.Empty()) {
            ItemClick_t ItemClick = (ItemClick_t)Scanner::Find("\x74\x73\x8B\x50\x08", "xxxxx", -25);
            printf("[SCAN] ItemClick = %p\n", ItemClick);
            ItemClickHook.Detour(ItemClick, OnItemClick);
        }
        ItemClickCallback = callback;
    }

    void Items::RestoreHooks() {
        HookBase::DisableHooks(&ItemClickHook);
    }

    void Items::AsyncGetItemByName(Item *item, std::wstring& res) {
        if (!item) return;
        if (!item || !item->complete_name_enc) return;
        wchar_t *str = item->complete_name_enc;
        UI::AsyncDecodeStr(str, &res);
    }
} // namespace GW