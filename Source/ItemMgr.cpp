#include "stdafx.h"

#include <GWCA/Packets/Opcodes.h>
#include <GWCA/Constants/Constants.h>

#include <GWCA/Utilities/Debug.h>
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
#include <GWCA/Context/WorldContext.h>

#include <GWCA/Managers/Module.h>

#include <GWCA/Managers/UIMgr.h>
#include <GWCA/Managers/ItemMgr.h>
#include <GWCA/Managers/CtoSMgr.h>
#include <GWCA/Managers/StoCMgr.h>

namespace {
    using namespace GW;

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

    typedef void (__fastcall *ItemClick_pt)(Constants::Bag *bag_id, void *edx, ItemClickParam *param);
    ItemClick_pt RetItemClick;
    ItemClick_pt ItemClick_Func;

    std::unordered_map<HookEntry *, Items::ItemClickCallback> ItemClick_callbacks;
    void __fastcall OnItemClick(Constants::Bag* bag_id, void *edx, ItemClickParam *param) {
        HookBase::EnterHook();
        if (!(bag_id && param)) {
            RetItemClick(bag_id, edx, param);
            HookBase::LeaveHook();
            return;
        }

        uint32_t slot = param->slot - 2; // for some reason the slot is offset by 2
        GW::HookStatus status;
        Bag* bag = Items::GetBag((Constants::Bag)(((uint32_t)*bag_id) + 1));
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
                "\xC7\x00\x0F\x00\x00\x00\x89\x48\x14", "xxxxxxxxx", -0x28);
            GWCA_INFO("[SCAN] StorageOpen = %p\n", (void*)address);
            if (Verify(address))
                storage_open_addr = *(uintptr_t*)address;
        }

        ItemClick_Func = (ItemClick_pt)Scanner::Find(
            "\x8B\x48\x08\x83\xEA\x00\x0F\x84", "xxxxxxxx", -0x1C);
        GWCA_INFO("[SCAN] ItemClick = %p\n", ItemClick_Func);

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
    bool Item::GetIsZcoin() const {
        if (model_file_id == 31202) return true; // Copper
        if (model_file_id == 31203) return true; // Gold
        if (model_file_id == 31204) return true; // Silver
        return false;
    }

    bool Item::GetIsMaterial() const {
        if (type() == Constants::ItemType::Materials_Zcoins
            && !GetIsZcoin()) {
            return true;
        }
        return false;
    }
    namespace Items {
        void OpenXunlaiWindow() {
            Packet::StoC::DataWindow pack;
            pack.agent = 0;
            pack.type = 0;
            pack.data = 3;
            StoC::EmulatePacket(&pack);
        }

        void PickUpItem(const Item* item, uint32_t CallTarget /*= 0*/) {
            CtoS::SendPacket(0xC, GAME_CMSG_INTERACT_ITEM, item->agent_id, CallTarget);
        }

        void DropItem(const Item* item, uint32_t quantity) {
            CtoS::SendPacket(0xC, GAME_CMSG_DROP_ITEM, item->item_id, quantity);
        }

        void EquipItem(const Item* item) {
            CtoS::SendPacket(0x8, GAME_CMSG_EQUIP_ITEM, item->item_id);
        }

        void UseItem(const Item* item) {
            CtoS::SendPacket(0x8, GAME_CMSG_ITEM_USE, item->item_id);
        }

        Bag** GetBagArray() {
            auto* i = GetInventory();
            return i ? i->bags : nullptr;
        }

        Bag* GetBag(Constants::Bag bag_id) {
            Bag** bags = bag_id < Constants::Bag::Max ? GetBagArray() : nullptr;
            return bags ? bags[(uint32_t)bag_id] : nullptr;
        }

        Item* GetItemBySlot(const Bag* bag, uint32_t slot) {
            if (!bag || slot == 0) return nullptr;
            if (!bag->items.valid()) return nullptr;
            if (slot > bag->items.size()) return nullptr;
            return bag->items[slot - 1];
        }

        Item* GetItemBySlot(Constants::Bag bag, uint32_t slot) {
            return GetItemBySlot(GetBag(bag), slot);
        }

        Item* GetHoveredItem() {
            UI::TooltipInfo* tooltip = UI::GetCurrentTooltip();
            if (!(tooltip && (tooltip->type() == UI::TooltipType::Item || tooltip->type() == UI::TooltipType::WeaponSet)))
                return nullptr;
            return GetItemById(*(ItemID*)tooltip->payload);
        }

        Item* GetItemById(ItemID item_id) {
            auto* items = item_id != ItemID::None ? GetItemArray() : nullptr;
            return items && (uint32_t)item_id < items->size() ? items->at((uint32_t)item_id) : nullptr;
        }

        ItemArray* GetItemArray() {
            auto* i = ItemContext::instance();
            return i && i->item_array.valid() ? &i->item_array : nullptr;
        }

        void DropGold(uint32_t Amount /*= 1*/) {
            CtoS::SendPacket(0x8, GAME_CMSG_DROP_GOLD, Amount);
        }

        Inventory* GetInventory() {
            auto* i = ItemContext::instance();
            return i ? i->inventory : nullptr;
        }

        uint32_t GetGoldAmountOnCharacter() {
            auto* i = GetInventory();
            return i ? i->gold_character : 0;
        }

        uint32_t GetGoldAmountInStorage() {
            auto* i = GetInventory();
            return i ? i->gold_storage : 0;
        }

        static void ChangeGold(uint32_t character_gold, uint32_t storage_gold) {
            CtoS::SendPacket(0xC, GAME_CMSG_ITEM_CHANGE_GOLD, character_gold, storage_gold);
        }

        uint32_t DepositGold(uint32_t amount) {
            uint32_t gold_storage = GetGoldAmountInStorage();
            uint32_t gold_character = GetGoldAmountOnCharacter();
            uint32_t will_move = 0;
            if (amount == 0) {
                will_move = std::min(1000000 - gold_storage, gold_character);
            }
            else {
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

        uint32_t WithdrawGold(uint32_t amount) {
            uint32_t gold_storage = GetGoldAmountInStorage();
            uint32_t gold_character = GetGoldAmountOnCharacter();
            uint32_t will_move = 0;
            if (amount == 0) {
                will_move = std::min(gold_storage, 100000 - gold_character);
            }
            else {
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

        void OpenLockedChest() {
            CtoS::SendPacket(0x8, GAME_CMSG_OPEN_CHEST, 0x2);
        }

        void MoveItem(const Item* from, const Bag* bag, uint32_t slot, uint32_t quantity) {
            if (!from || !bag) return;
            if (bag->items.size() < (unsigned)slot) return;
            if (quantity <= 0) quantity = from->quantity;
            if (quantity > from->quantity) quantity = from->quantity;
            if (quantity == from->quantity)
                CtoS::SendPacket(0x10, GAME_CMSG_ITEM_MOVE, from->item_id, bag->bag_id, slot);
            else
                CtoS::SendPacket(0x14, GAME_CMSG_ITEM_SPLIT_STACK, from->item_id, quantity, bag->bag_id, slot);
        }

        void MoveItem(const Item* item, Constants::Bag bag_id, uint32_t slot, uint32_t quantity)
        {
            MoveItem(item, GetBag(bag_id), slot, quantity);
        }

        void MoveItem(const Item* from, const Item* to, uint32_t quantity) {
            if (!from || !to) return;
            if (!from->bag || !to->bag) return;
            MoveItem(from, to->bag, to->slot, quantity);
        }

        Constants::MaterialSlot GetMaterialSlot(Constants::ModelID model_id) {
            using namespace Constants;
            switch (model_id) {
            case ModelID::Bone:                 return MaterialSlot::Bone;
            case ModelID::IronIngot:            return MaterialSlot::IronIngot;
            case ModelID::TannedHideSquare:     return MaterialSlot::TannedHideSquare;
            case ModelID::Scale:                return MaterialSlot::Scale;
            case ModelID::ChitinFragment:       return MaterialSlot::ChitinFragment;
            case ModelID::BoltofCloth:          return MaterialSlot::BoltofCloth;
            case ModelID::WoodPlank:            return MaterialSlot::WoodPlank;
            case ModelID::GraniteSlab:          return MaterialSlot::GraniteSlab;
            case ModelID::PileofGlitteringDust: return MaterialSlot::PileofGlitteringDust;
            case ModelID::PlantFiber:           return MaterialSlot::PlantFiber;
            case ModelID::Feather:              return MaterialSlot::Feather;
            case ModelID::FurSquare:            return MaterialSlot::FurSquare;
            case ModelID::BoltofLinen:          return MaterialSlot::BoltofLinen;
            case ModelID::BoltofDamask:         return MaterialSlot::BoltofDamask;
            case ModelID::BoltofSilk:           return MaterialSlot::BoltofSilk;
            case ModelID::GlobofEctoplasm:      return MaterialSlot::GlobofEctoplasm;
            case ModelID::SteelIngot:           return MaterialSlot::SteelIngot;
            case ModelID::DeldrimorSteelIngot:  return MaterialSlot::DeldrimorSteelIngot;
            case ModelID::MonstrousClaw:        return MaterialSlot::MonstrousClaw;
            case ModelID::MonstrousEye:         return MaterialSlot::MonstrousEye;
            case ModelID::MonstrousFang:        return MaterialSlot::MonstrousFang;
            case ModelID::Ruby:                 return MaterialSlot::Ruby;
            case ModelID::Sapphire:             return MaterialSlot::Sapphire;
            case ModelID::Diamond:              return MaterialSlot::Diamond;
            case ModelID::OnyxGemstone:         return MaterialSlot::OnyxGemstone;
            case ModelID::LumpofCharcoal:       return MaterialSlot::LumpofCharcoal;
            case ModelID::ObsidianShard:        return MaterialSlot::ObsidianShard;
            case ModelID::TemperedGlassVial:    return MaterialSlot::TemperedGlassVial;
            case ModelID::LeatherSquare:        return MaterialSlot::LeatherSquare;
            case ModelID::ElonianLeatherSquare: return MaterialSlot::ElonianLeatherSquare;
            case ModelID::VialofInk:            return MaterialSlot::VialofInk;
            case ModelID::RollofParchment:      return MaterialSlot::RollofParchment;
            case ModelID::RollofVellum:         return MaterialSlot::RollofVellum;
            case ModelID::SpiritwoodPlank:      return MaterialSlot::SpiritwoodPlank;
            case ModelID::AmberChunk:           return MaterialSlot::AmberChunk;
            case ModelID::JadeiteShard:         return MaterialSlot::JadeiteShard;
            };
            return (MaterialSlot)-1;
        }

        Constants::MaterialSlot GetMaterialSlot(const Item* item) {
            return item && item->GetIsMaterial() ? GetMaterialSlot(item->model_id) : (Constants::MaterialSlot)-1;
        }

        bool UseItemByModelId(Constants::ModelID modelid, int bagStart, int bagEnd) {
            Bag** bags = GetBagArray();
            Bag* bag = NULL;
            Item* item = NULL;

            for (int bagIndex = bagStart; bags && bagIndex <= bagEnd; ++bagIndex) {
                bag = bags[bagIndex];
                if (!bag) continue;
                ItemArray items = bag->items;
                if (!items.valid()) continue;
                for (size_t i = 0; i < items.size(); i++) {
                    item = items[i];
                    if (!item) continue;
                    if (item->model_id != modelid) continue;
                    UseItem(item);
                    return true;
                }
            }

            return false;
        }

        uint32_t CountItemByModelId(Constants::ModelID modelid, int bagStart, int bagEnd) {
            uint32_t itemcount = 0;
            Bag** bags = GetBagArray();
            Bag* bag = NULL;
            Item* item = NULL;

            for (int bagIndex = bagStart; bags && bagIndex <= bagEnd; ++bagIndex) {
                bag = bags[bagIndex];
                if (!bag) continue;
                ItemArray items = bag->items;
                for (size_t i = 0; i < items.size(); i++) {
                    item = items[i];
                    if (!item) continue;
                    if (item->model_id != modelid) continue;
                    UseItem(item);
                    return true;
                    if (!items[i]) continue;
                    if (items[i]) {
                        if (items[i]->model_id == modelid) {
                            itemcount += items[i]->quantity;
                        }
                    }
                }
            }

            return itemcount;
        }

        Item* GetItemByModelId(Constants::ModelID modelid, int bagStart, int bagEnd) {
            Bag** bags = GetBagArray();
            Bag* bag = NULL;
            Item* item = NULL;

            for (int bagIndex = bagStart; bagIndex <= bagEnd; ++bagIndex) {
                bag = bags[bagIndex];
                if (!bag) continue;
                ItemArray items = bag->items;
                for (size_t i = 0; i < items.size(); i++) {
                    item = items[i];
                    if (!item) continue;
                    if (item->model_id != modelid) continue;
                    return item;
                }
            }

            return NULL;
        }

        Constants::StoragePane GetStoragePage(void) {
            return (Constants::StoragePane)UI::GetPreference(UI::Preference::StorageBagPage);
        }

        bool GetIsStorageOpen(void) {
            if (Verify(storage_open_addr))
                return *(uint32_t*)storage_open_addr != 0;
            else
                return false;
        }

        void RegisterItemClickCallback(
            HookEntry* entry,
            ItemClickCallback callback)
        {
            ItemClick_callbacks.insert({ entry, callback });
        }

        void RemoveItemClickCallback(
            HookEntry* entry)
        {
            auto it = ItemClick_callbacks.find(entry);
            if (it != ItemClick_callbacks.end())
                ItemClick_callbacks.erase(it);
        }

        void AsyncGetItemByName(const Item* item, std::wstring& res) {
            if (!item) return;
            if (!item || !item->complete_name_enc) return;
            wchar_t* str = item->complete_name_enc;
            UI::AsyncDecodeStr(str, &res);
        }

        Constants::EquipmentStatus GetCapeStatus() {
            return (Constants::EquipmentStatus)(WorldContext::instance()->equipment_status & 0x3);
        }
        void SetCapeStatus(Constants::EquipmentStatus s) {
            if (GetCapeStatus() != s)
                CtoS::SendPacket(0xC, GAME_CMSG_EQUIP_VISIBILITY, s, 0x3);
        }
        Constants::EquipmentStatus GetHelmStatus() {
            return (Constants::EquipmentStatus)((WorldContext::instance()->equipment_status & 0xC) >> 2);
        }
        void SetHelmStatus(Constants::EquipmentStatus s) {
            if (GetHelmStatus() != s)
                CtoS::SendPacket(0xC, GAME_CMSG_EQUIP_VISIBILITY, (uint32_t)s << 2, 0xC);
        }
        Constants::EquipmentStatus GetCostumeBodyStatus() {
            return (Constants::EquipmentStatus)((WorldContext::instance()->equipment_status & 0x30) >> 4);
        }
        void SetCostumeBodyStatus(Constants::EquipmentStatus s) {
            if (GetCostumeBodyStatus() != s)
                CtoS::SendPacket(0xC, GAME_CMSG_EQUIP_VISIBILITY, (uint32_t)s << 4, 0x30);
        }
        Constants::EquipmentStatus GetCostumeHeadpieceStatus() {
            return (Constants::EquipmentStatus)((WorldContext::instance()->equipment_status & 0xC0) >> 6);
        }
        void SetCostumeHeadpieceStatus(Constants::EquipmentStatus s) {
            if (GetCostumeHeadpieceStatus() != s)
                CtoS::SendPacket(0xC, GAME_CMSG_EQUIP_VISIBILITY, (uint32_t)s << 6, 0xC0);
        }
    }
   

} // namespace GW
