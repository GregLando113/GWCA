#include "stdafx.h"

#include <GWCA/Packets/Opcodes.h>
#include <GWCA/Constants/Constants.h>

#include <GWCA/Utilities/Export.h>
#include <GWCA/Utilities/Hooker.h>
#include <GWCA/Utilities/Macros.h>
#include <GWCA/Utilities/Scanner.h>

#include <GWCA/GameContainers/GamePos.h>
#include <GWCA/Packets/StoC.h>

#include <GWCA/GameEntities/Item.h>
#include <GWCA/GameEntities/Agent.h>

#include <GWCA/Context/GameContext.h>
#include <GWCA/Context/ItemContext.h>

#include <GWCA/Managers/Module.h>

#include <GWCA/Managers/UIMgr.h>
#include <GWCA/Managers/ItemMgr.h>
#include <GWCA/Managers/CtoSMgr.h>
#include <GWCA/Managers/StoCMgr.h>

namespace {
    using namespace GW;

    uintptr_t storage_pannel_addr;
    uintptr_t storage_open_addr;

    enum ItemClickType : uint32_t {
        ItemClickType_Add           = 2, // (when you load / open chest)
        ItemClickType_Click         = 5,
        ItemClickType_Release       = 7,
        ItemClickType_DoubleClick   = 8,
        ItemClickType_Move          = 9,
        ItemClickType_DragStart     = 10,
        ItemClickType_DragStop      = 12,
    };

    struct ItemClickParam {
        uint32_t unk0;
        uint32_t slot;
        uint32_t type;
    };

    typedef void (__fastcall *ItemClick_pt)(uint32_t *bag_id, void *edx, ItemClickParam *param);
    ItemClick_pt RetItemClick;
    ItemClick_pt ItemClick_Func;

    std::unordered_map<HookEntry *, Items::ItemClickCallback> ItemClick_callbacks;
    void __fastcall OnItemClick(uint32_t* bag_id, void *edx, ItemClickParam *param) {
        HookBase::EnterHook();
        if (!(bag_id && param)) {
            RetItemClick(bag_id, edx, param);
            HookBase::LeaveHook();
            return;
        }

        uint32_t slot = param->slot - 2; // for some reason the slot is offset by 2
        GW::HookStatus status;
        Bag* bag = Items::GetBag(*bag_id + 1);
        if (bag) {
            for (auto& it : ItemClick_callbacks) {
                it.second(&status, param->type, slot, bag);
                ++status.altitude;
            }
        }
        if (!status.blocked)
            RetItemClick(bag_id, edx, param);
        HookBase::LeaveHook();
    }

    void Init() {
        {
            uintptr_t address = Scanner::Find(
                "\x0F\x84\x5D\x01\x00\x00\x83\x7B\x14", "xxxxxxxxx", -4);
            printf("[SCAN] StoragePannel = %p\n", (void *)address);
            if (Verify(address))
                storage_pannel_addr = *(uintptr_t *)address;
        }

        {
            uintptr_t address = Scanner::Find(
                "\xC7\x00\x0F\x00\x00\x00\x89\x48\x14", "xxxxxxxxx", -0x28);
            printf("[SCAN] StorageOpen = %p\n", (void *)address);
            if (Verify(address))
                storage_open_addr = *(uintptr_t *)address;
        }

        ItemClick_Func = (ItemClick_pt)Scanner::Find(
            "\x8B\x48\x08\x83\xEA\x00\x0F\x84", "xxxxxxxx", -0x1C);
        printf("[SCAN] ItemClick = %p\n", ItemClick_Func);

        if (Verify(ItemClick_Func))
            HookBase::CreateHook(ItemClick_Func, OnItemClick, (void **)&RetItemClick);
    }

    void Exit() {
        if (ItemClick_Func)
            HookBase::RemoveHook(ItemClick_Func);
    }
}

namespace GW {

    Module ItemModule = {
        "ItemModule",   // name
        NULL,           // param
        ::Init,         // init_module
        ::Exit,         // exit_module
        NULL,           // enable_hooks
        NULL,           // disable_hooks
    };

    void Items::OpenXunlaiWindow() {
        Packet::StoC::DataWindow pack;
        pack.agent = 0;
        pack.type = 0;
        pack.data = 3;
        StoC::EmulatePacket(&pack);
    }

    void Items::PickUpItem(Item *item, uint32_t CallTarget /*= 0*/) {
        CtoS::SendPacket(0xC, GAME_CMSG_INTERACT_ITEM, item->agent_id, CallTarget);
    }

    void Items::DropItem(Item *item, uint32_t quantity) {
        CtoS::SendPacket(0xC, GAME_CMSG_DROP_ITEM, item->item_id, quantity);
    }

    void Items::EquipItem(Item *item) {
        CtoS::SendPacket(0x8, GAME_CMSG_EQUIP_ITEM, item->item_id);
    }

    void Items::UseItem(Item *item) {
        CtoS::SendPacket(0x8, GAME_CMSG_ITEM_USE, item->item_id);
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
        CtoS::SendPacket(0x8, GAME_CMSG_DROP_GOLD, Amount);
    }

    uint32_t Items::GetGoldAmountOnCharacter() {
        return GameContext::instance()->items->inventory->gold_character;
    }

    uint32_t Items::GetGoldAmountInStorage() {
        return GameContext::instance()->items->inventory->gold_storage;
    }

    static void ChangeGold(uint32_t character_gold, uint32_t storage_gold) {
        CtoS::SendPacket(0xC, GAME_CMSG_ITEM_CHANGE_GOLD, character_gold, storage_gold);
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
        CtoS::SendPacket(0x8, GAME_CMSG_OPEN_CHEST, 0x2);
    }

    void Items::MoveItem(Item *item, Bag *bag, int slot, int quantity) {
        if (slot < 0) return;
        if (!item || !bag) return;
        if (bag->items.size() < (unsigned)slot) return;
        // @Robustness: Check if there is enough space at the destination.
        CtoS::SendPacket(0x10, GAME_CMSG_ITEM_MOVE, item->item_id, bag->bag_id, slot);
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
        CtoS::SendPacket(0x10, GAME_CMSG_ITEM_MOVE, from->item_id, to->bag->bag_id, to->slot);
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
        if (Verify(storage_pannel_addr)) {
            // @Cleanup: 20 being the position for the storage, but this
            // array hold way more, for instance the current chat channel
            return ((uint32_t *)storage_pannel_addr)[20];
        } else {
            return 0;
        }
    }

    bool Items::GetIsStorageOpen(void) {
        if (Verify(storage_open_addr))
            return *(uint32_t *)storage_open_addr != 0;
        else
            return false;
    }

    void Items::RegisterItemClickCallback(
        HookEntry *entry,
        ItemClickCallback callback)
    {
        ItemClick_callbacks.insert({entry, callback});
    }

    void Items::RemoveItemClickCallback(
        HookEntry *entry)
    {
        auto it = ItemClick_callbacks.find(entry);
        if (it != ItemClick_callbacks.end())
            ItemClick_callbacks.erase(it);
    }

    void Items::AsyncGetItemByName(Item *item, std::wstring& res) {
        if (!item) return;
        if (!item || !item->complete_name_enc) return;
        wchar_t *str = item->complete_name_enc;
        UI::AsyncDecodeStr(str, &res);
    }
} // namespace GW
