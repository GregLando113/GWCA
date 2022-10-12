#include "stdafx.h"

#include <GWCA/Constants/Constants.h>
#include <GWCA/Constants/ItemIDs.h>

#include <GWCA/Packets/StoC.h>

#include <GWCA/Context/ItemContext.h>
#include <GWCA/Context/WorldContext.h>

#include <GWCA/Utilities/Hooker.h>
#include <GWCA/Utilities/Hook.h>
#include <GWCA/Utilities/Scanner.h>

#include <GWCA/GameEntities/Item.h>
#include <GWCA/GameEntities/Agent.h>

#include <GWCA/Managers/ItemMgr.h>
#include <GWCA/Managers/Module.h>
#include <GWCA/Managers/StoCMgr.h>
#include <GWCA/Managers/AgentMgr.h>
#include <GWCA/Managers/UIMgr.h>

namespace {
    using namespace GW;
    using namespace Items;

    uint32_t* storage_open_addr = 0;
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
    ItemClick_pt RetItemClick = 0;
    ItemClick_pt ItemClick_Func = 0;

    // General purpose "void function that does something with this id"
    typedef void(__cdecl* DoAction_pt)(uint32_t identifier);

    DoAction_pt DropGold_Func = 0;
    DoAction_pt OpenLockedChest_Func = 0;

    typedef void(__cdecl* Void_pt)();
    Void_pt SalvageSessionCancel_Func = 0;
    Void_pt SalvageSessionComplete_Func = 0;
    Void_pt SalvageMaterials_Func = 0;

    typedef void(__cdecl* SalvageStart_pt)(uint32_t salvage_kit_id, uint32_t salvage_session_id, uint32_t item_id);
    SalvageStart_pt SalvageStart_Func = 0;

    typedef void(__cdecl* IdentifyItem_pt)(uint32_t identification_kit_id, uint32_t item_id);
    IdentifyItem_pt IdentifyItem_Func = 0;

    HookEntry OnUseItem_Entry;
    DoAction_pt UseItem_Func = 0;
    DoAction_pt UseItem_Ret = 0;
    void OnUseItem(uint32_t item_id) {
        GW::Hook::EnterHook();
        UI::SendUIMessage(UI::UIMessage::kSendUseItem, (void*)item_id);
        GW::Hook::LeaveHook();
    };
    void OnUseItem_UIMessage(GW::HookStatus* status, UI::UIMessage message_id, void* wparam, void*) {
        GWCA_ASSERT(message_id == UI::UIMessage::kSendUseItem && wparam);
        if (!status->blocked) {
            UseItem_Ret((uint32_t)wparam);
        }
    }

    HookEntry OnPingWeaponSet_Entry;
    typedef void(__cdecl* PingWeaponSet_pt)(uint32_t agent_id, uint32_t weapon_item_id, uint32_t offhand_item_id);
    PingWeaponSet_pt PingWeaponSet_Func = 0;
    PingWeaponSet_pt PingWeaponSet_Ret = 0;

    void OnPingWeaponSet(uint32_t agent_id, uint32_t weapon_item_id, uint32_t offhand_item_id) {
        GW::Hook::EnterHook();
        uint32_t pack[3] = { agent_id,weapon_item_id,offhand_item_id };
        // Pass this through UI, we'll pick it up in OnSendDialog_UIMessage
        UI::SendUIMessage(UI::UIMessage::kSendPingWeaponSet, (void*)&pack);
        GW::Hook::LeaveHook();
    };
    void OnPingWeaponSet_UIMessage(GW::HookStatus* status, UI::UIMessage message_id, void* wparam, void*) {
        GWCA_ASSERT(message_id == UI::UIMessage::kSendPingWeaponSet && wparam);
        uint32_t* pack = (uint32_t*)wparam;
        if (!status->blocked) {
            PingWeaponSet_Ret(pack[0], pack[1], pack[2]);
        }
    }

    HookEntry OnMoveItem_Entry;
    typedef void(__cdecl* MoveItem_pt)(uint32_t item_id, uint32_t quantity, uint32_t bag_id, uint32_t slot);
    MoveItem_pt MoveItem_Func = 0;
    MoveItem_pt MoveItem_Ret = 0;
    void OnMoveItem(uint32_t item_id, uint32_t quantity, uint32_t bag_id, uint32_t slot) {
        GW::Hook::EnterHook();
        uint32_t pack[] = { item_id,quantity,bag_id, slot };
        UI::SendUIMessage(UI::UIMessage::kSendMoveItem, (void*)pack);
        GW::Hook::LeaveHook();
    };
    void OnMoveItem_UIMessage(GW::HookStatus* status, UI::UIMessage message_id, void* wparam, void*) {
        GWCA_ASSERT(message_id == UI::UIMessage::kSendMoveItem && wparam);
        uint32_t* pack = (uint32_t*)wparam;
        if (!status->blocked) {
            MoveItem_Ret(pack[0], pack[1], pack[2], pack[3]);
        }
    }

    typedef void(__cdecl* EquipItem_pt)(uint32_t item_id, uint32_t agent_id);
    EquipItem_pt EquipItem_Func = 0;

    typedef void(__cdecl* DropItem_pt)(uint32_t item_id, uint32_t quantity);
    DropItem_pt DropItem_Func = 0;

    typedef void(__cdecl* ChangeEquipmentVisibility_pt)(uint32_t equipment_state, uint32_t equip_type);
    ChangeEquipmentVisibility_pt ChangeEquipmentVisibility_Func = 0;

    typedef void(__cdecl* ChangeGold_pt)(uint32_t character_gold, uint32_t storage_gold);
    ChangeGold_pt ChangeGold_Func = 0;

    std::unordered_map<HookEntry *, ItemClickCallback> ItemClick_callbacks;
    void __fastcall OnItemClick(uint32_t* bag_id, void *edx, ItemClickParam *param) {
        HookBase::EnterHook();
        if (!(bag_id && param)) {
            RetItemClick(bag_id, edx, param);
            HookBase::LeaveHook();
            return;
        }

        uint32_t slot = param->slot - 2; // for some reason the slot is offset by 2
        GW::HookStatus status;
        Bag* bag = GetBag(*bag_id + 1);
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

        DWORD address = 0;

        address = Scanner::Find("\xC7\x00\x0F\x00\x00\x00\x89\x48\x14", "xxxxxxxxx", -0x28);
        if (Scanner::IsValidPtr(*(uintptr_t*)address))
            storage_open_addr = *(uint32_t**)address;

        ItemClick_Func = (ItemClick_pt)Scanner::Find( "\x8B\x48\x08\x83\xEA\x00\x0F\x84", "xxxxxxxx", -0x1C);

        address = Scanner::Find("\x0f\xb6\x40\x04\x83\xc0\xf8","xxxxxxx",0x1f);
        UseItem_Func = (DoAction_pt)Scanner::FunctionFromNearCall(address);

        address = Scanner::Find("\x83\xc4\x04\x85\xc0\x0f?????\x8d\x45\x0c", "xxxxxx?????xxx");
        EquipItem_Func = (EquipItem_pt)Scanner::FunctionFromNearCall(address + 0x1e);
        MoveItem_Func = (MoveItem_pt)Scanner::FunctionFromNearCall(address + 0x6e);

        address = Scanner::FindAssertion("p:\\code\\gw\\ui\\game\\gmview.cpp", "param.notifyData");
        DropGold_Func = (DoAction_pt)Scanner::FunctionFromNearCall(address + 0x13);

        SalvageSessionCancel_Func = (Void_pt)Scanner::FunctionFromNearCall(address - 0x22);
        SalvageSessionComplete_Func = (Void_pt)Scanner::FunctionFromNearCall(address - 0x37);
        SalvageMaterials_Func = (Void_pt)Scanner::FunctionFromNearCall(address - 0x4b);

        address = Scanner::Find("\x6a\x00\x50\x68\x00\x01\x00\x10", "xxxxxxxx");
        SalvageStart_Func = (SalvageStart_pt)Scanner::FunctionFromNearCall(address + 0x22);

        IdentifyItem_Func = (IdentifyItem_pt)Scanner::FunctionFromNearCall(address + 0x4a);

        address = Scanner::Find("\x83\xc4\x40\x6a\x00\x6a\x19", "xxxxxxx", -0x4e);
        DropItem_Func = (DropItem_pt)Scanner::FunctionFromNearCall(address);

        address = Scanner::Find("\x8b\x42\x04\x51\x23\xc1","xxxxxx",0x7);
        ChangeEquipmentVisibility_Func = (ChangeEquipmentVisibility_pt)Scanner::FunctionFromNearCall(address);

        address = Scanner::Find("\x68\x21\x03\x00\x00\x89\x45\xfc", "xxxxxxxx", 0x3a);
        ChangeGold_Func = (ChangeGold_pt)Scanner::FunctionFromNearCall(address);

        address = Scanner::Find("\x83\xc9\x01\x89\x4b\x24", "xxxxxx", 0x28);
        OpenLockedChest_Func = (DoAction_pt)Scanner::FunctionFromNearCall(address);

        address = Scanner::FindAssertion("p:\\code\\gw\\ui\\game\\gmweaponbar.cpp", "slotIndex < ITEM_PLAYER_EQUIP_SETS", 0x128);
        PingWeaponSet_Func = (PingWeaponSet_pt)Scanner::FunctionFromNearCall(address);

        GWCA_INFO("[SCAN] StorageOpenPtr = %p", storage_open_addr);
        GWCA_INFO("[SCAN] OnItemClick Function = %p", ItemClick_Func);
        GWCA_INFO("[SCAN] UseItem Function = %p", UseItem_Func);
        GWCA_INFO("[SCAN] EquipItem Function = %p", EquipItem_Func);
        GWCA_INFO("[SCAN] MoveItem Function = %p", MoveItem_Func);
        GWCA_INFO("[SCAN] DropGold Function = %p", DropGold_Func);
        GWCA_INFO("[SCAN] DropItem Function = %p", DropItem_Func);
        GWCA_INFO("[SCAN] ChangeEquipmentVisibility Function = %p", ChangeEquipmentVisibility_Func);
        GWCA_INFO("[SCAN] ChangeGold Function = %p", ChangeGold_Func);
        GWCA_INFO("[SCAN] OpenLockedChest Function = %p", OpenLockedChest_Func);
        GWCA_INFO("[SCAN] PingWeaponSet_Func = %p", PingWeaponSet_Func);
        GWCA_INFO("[SCAN] SalvageSessionCancel_Func = %p", SalvageSessionCancel_Func);
        GWCA_INFO("[SCAN] SalvageSessionComplete_Func = %p", SalvageSessionComplete_Func);
        GWCA_INFO("[SCAN] SalvageMaterials_Func = %p", SalvageMaterials_Func);
        GWCA_INFO("[SCAN] SalvageStart_Func = %p", SalvageStart_Func);
#ifdef _DEBUG
        GWCA_ASSERT(storage_open_addr);
        GWCA_ASSERT(ItemClick_Func);
        GWCA_ASSERT(EquipItem_Func);
        GWCA_ASSERT(UseItem_Func);
        GWCA_ASSERT(MoveItem_Func);
        GWCA_ASSERT(DropGold_Func);
        GWCA_ASSERT(DropItem_Func);
        GWCA_ASSERT(ChangeEquipmentVisibility_Func);
        GWCA_ASSERT(ChangeGold_Func);
        GWCA_ASSERT(OpenLockedChest_Func);
        GWCA_ASSERT(PingWeaponSet_Func);
        GWCA_ASSERT(SalvageSessionCancel_Func);
        GWCA_ASSERT(SalvageSessionComplete_Func);
        GWCA_ASSERT(SalvageMaterials_Func);
        GWCA_ASSERT(SalvageStart_Func);
#endif
        HookBase::CreateHook(ItemClick_Func, OnItemClick, (void**)&RetItemClick);
        if (PingWeaponSet_Func) {
            HookBase::CreateHook(PingWeaponSet_Func, OnPingWeaponSet, (void**)&PingWeaponSet_Ret);
            UI::RegisterUIMessageCallback(&OnPingWeaponSet_Entry, UI::UIMessage::kSendPingWeaponSet, OnPingWeaponSet_UIMessage, 0x1);
        }
        if (MoveItem_Func) {
            HookBase::CreateHook(MoveItem_Func, OnMoveItem, (void**)&MoveItem_Ret);
            UI::RegisterUIMessageCallback(&OnMoveItem_Entry, UI::UIMessage::kSendMoveItem, OnMoveItem_UIMessage, 0x1);
        }
        if (UseItem_Func) {
            HookBase::CreateHook(UseItem_Func, OnUseItem, (void**)&UseItem_Ret);
            UI::RegisterUIMessageCallback(&OnUseItem_Entry, UI::UIMessage::kSendUseItem, OnUseItem_UIMessage, 0x1);
        }
    }

    void EnableHooks() {
        if (ItemClick_Func)
            HookBase::EnableHooks(ItemClick_Func);
        if (PingWeaponSet_Func)
            HookBase::EnableHooks(PingWeaponSet_Func);
        if (MoveItem_Func)
            HookBase::EnableHooks(MoveItem_Func);
        if (UseItem_Func)
            HookBase::EnableHooks(UseItem_Func);
    }

    void DisableHooks() {
        if (ItemClick_Func)
            HookBase::DisableHooks(ItemClick_Func);
        if (PingWeaponSet_Func)
            HookBase::DisableHooks(PingWeaponSet_Func);
        if (MoveItem_Func)
            HookBase::DisableHooks(MoveItem_Func);
        if (UseItem_Func)
            HookBase::DisableHooks(UseItem_Func);
    }

    void Exit() {
        HookBase::RemoveHook(ItemClick_Func);
        HookBase::RemoveHook(PingWeaponSet_Func);
        HookBase::RemoveHook(MoveItem_Func);
        HookBase::RemoveHook(UseItem_Func);
    }
}

namespace GW {

    Module ItemModule = {
        "ItemModule",   // name
        NULL,           // param
        ::Init,         // init_module
        ::Exit,         // exit_module
        ::EnableHooks,           // enable_hooks
        ::DisableHooks,           // disable_hooks
    };
    bool Item::GetIsZcoin() const {
        if (model_file_id == 31202) return true; // Copper
        if (model_file_id == 31203) return true; // Gold
        if (model_file_id == 31204) return true; // Silver
        return false;
    }

    bool Item::GetIsMaterial() const {
        if (type == static_cast<uint8_t>(Constants::ItemType::Materials_Zcoins)
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

        bool PickUpItem(const Item* item, uint32_t CallTarget /*= 0*/) {
            return Agents::PickUpItem(Agents::GetAgentByID(item->agent_id), CallTarget);
        }

        bool DropItem(const Item* item, uint32_t quantity) {
            if (!(DropItem_Func && item))
                return false;
            DropItem_Func(item->item_id, quantity);
            return true;
        }

        bool PingWeaponSet(uint32_t agent_id, uint32_t weapon_item_id, uint32_t offhand_item_id) {
            if (!(PingWeaponSet_Func && agent_id))
                return false;
            PingWeaponSet_Func(agent_id,weapon_item_id,offhand_item_id);
            return true;
        }

        bool EquipItem(const Item* item, uint32_t agent_id) {
            if (!(item && EquipItem_Func))
                return false;
            if (!agent_id)
                agent_id = Agents::GetPlayerId();
            if (!agent_id)
                return false;
            EquipItem_Func(item->item_id, agent_id);
            return true;
        }

        bool UseItem(const Item* item) {
            if (!(UseItem_Func && item))
                return false;
            UseItem_Func(item->item_id);
            return true;
        }

        Bag** GetBagArray() {
            auto* i = GetInventory();
            return i ? i->bags : nullptr;
        }

        Bag* GetBag(Constants::Bag bag_id) {
            if ((uint32_t)bag_id >= (uint32_t)Constants::BagMax)
                return nullptr;
            Bag** bags = GetBagArray();
            return bags ? bags[(unsigned)bag_id] : nullptr;
        }

        Bag* GetBag(uint32_t bag_id) {
            return GetBag((Constants::Bag)bag_id);
        }

        Item* GetItemBySlot(const Bag* bag, uint32_t slot) {
            if (!bag || slot == 0) return nullptr;
            if (!bag->items.valid()) return nullptr;
            if (slot > bag->items.size()) return nullptr;
            return bag->items[slot - 1];
        }

        Item* GetItemBySlot(Constants::Bag bag, uint32_t slot) {
            Bag* bag_ptr = GetBag(bag);
            return bag_ptr ? GetItemBySlot(bag_ptr, slot) : nullptr;
        }

        Item* GetHoveredItem() {
            UI::TooltipInfo* tooltip = UI::GetCurrentTooltip();
            if (!(tooltip && (tooltip->type() == UI::TooltipType::Item || tooltip->type() == UI::TooltipType::WeaponSet)))
                return nullptr;
            return GetItemById(*(uint32_t*)tooltip->payload);
        }

        Item* GetItemBySlot(uint32_t bag, uint32_t slot) {
            if (Constants::BagMax <= bag) return nullptr;
            return GetItemBySlot((Constants::Bag)bag, slot);
        }

        Item* GetItemById(uint32_t item_id) {
            GW::ItemArray* items = item_id ? GetItemArray() : nullptr;
            return items && item_id < items->size() ? items->at(item_id) : nullptr;
        }

        ItemArray* GetItemArray() {
            auto* i = GetItemContext();
            return i && i->item_array.valid() ? &i->item_array : nullptr;
        }
        Inventory* GetInventory() {
            auto* i = GetItemContext();
            return i ? i->inventory : nullptr;
        }

        bool DropGold(uint32_t Amount /*= 1*/) {
            if (!(DropGold_Func && GetGoldAmountOnCharacter() >= Amount))
                return false;
            DropGold_Func(Amount);
            return true;
        }

        bool SalvageSessionCancel() {
            return SalvageSessionCancel_Func ? SalvageSessionCancel_Func(), true : false;
        }

        bool SalvageSessionDone() {
            return SalvageSessionComplete_Func ? SalvageSessionComplete_Func(), true : false;
        }

        bool SalvageMaterials() {
            return SalvageMaterials_Func ? SalvageMaterials_Func(), true : false;
        }

        bool SalvageStart(uint32_t salvage_kit_id, uint32_t item_id) {
            return SalvageStart_Func ? SalvageStart_Func(salvage_kit_id, GetWorldContext()->salvage_session_id, item_id), true : false;
        }

        bool IdentifyItem(uint32_t identification_kit_id, uint32_t item_id) {
            return IdentifyItem_Func ? IdentifyItem_Func(identification_kit_id, item_id), true : false;
        }

        uint32_t GetGoldAmountOnCharacter() {
            auto* i = GetInventory();
            return i ? i->gold_character : 0;
        }

        uint32_t GetGoldAmountInStorage() {
            auto* i = GetInventory();
            return i ? i->gold_storage : 0;
        }

        bool ChangeGold(uint32_t character_gold, uint32_t storage_gold) {
            if (!(ChangeGold_Func && (GetGoldAmountInStorage() + GetGoldAmountOnCharacter()) == (character_gold + storage_gold)))
                return false;
            ChangeGold_Func(character_gold, storage_gold);
            return true;
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
            return ChangeGold(gold_character, gold_storage) ? will_move : 0;
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
            return ChangeGold(gold_character, gold_storage) ? will_move : 0;
        }

        bool OpenLockedChest(bool use_key) {
            auto* target = Agents::GetTarget();
            if (!(OpenLockedChest_Func && target && target->GetIsGadgetType()))
                return false;
            auto* me = Agents::GetPlayer();
            if (!(me && GetDistance(me->pos, target->pos) < Constants::Range::Area))
                return false;
            if (use_key) {
                // TODO: Find matching key for chest to allow GWCA to use keys; how does the game know what dialog buttons to show?
                use_key = false;
            }
            if (!use_key && !GetItemByModelId(Constants::ItemID::Lockpick))
                return false;
            OpenLockedChest_Func(use_key ? 0x1U : 0x2U);
            return true;
        }

        bool MoveItem(const Item* from, const Bag* bag, uint32_t slot, uint32_t quantity) {
            if (!(MoveItem_Func && from && bag)) return false;
            if (bag->items.size() < (unsigned)slot) return false;
            if (quantity <= 0) quantity = from->quantity;
            if (quantity > from->quantity) quantity = from->quantity;
            MoveItem_Func(from->item_id, quantity, bag->index, slot);
            return true;
        }

        bool MoveItem(const Item* item, Constants::Bag bag_id, uint32_t slot, uint32_t quantity)
        {
            return MoveItem(item, GetBag(bag_id), slot, quantity);
        }

        bool MoveItem(const Item* from, const Item* to, uint32_t quantity) {
            return MoveItem(from, to->bag, to->slot, quantity);
        }


        int GetMaterialSlot(uint32_t model_id) {
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

        int GetMaterialSlot(const Item* item) {
            if (!item) return -1;
            if (!item->GetIsMaterial()) return -1;
            return GetMaterialSlot(item->model_id);
        }

        bool UseItemByModelId(uint32_t modelid, int bagStart, int bagEnd) {
            return UseItem(GetItemByModelId(modelid, bagStart, bagEnd));
        }

        uint32_t CountItemByModelId(uint32_t modelid, int bagStart, int bagEnd) {
            uint32_t itemcount = 0;
            Bag** bags = GetBagArray();
            Bag* bag = NULL;

            for (int bagIndex = bagStart; bagIndex <= bagEnd; ++bagIndex) {
                bag = bags[bagIndex];
                if (!(bag && bag->items.valid())) continue;
                for (GW::Item* item : bag->items) {
                    if (item && item->model_id == modelid) {
                        itemcount += item->quantity;
                    }
                }
            }

            return itemcount;
        }

        Item* GetItemByModelId(uint32_t modelid, int bagStart, int bagEnd) {
            Bag** bags = GetBagArray();
            Bag* bag = NULL;

            for (int bagIndex = bagStart; bagIndex <= bagEnd; ++bagIndex) {
                bag = bags[bagIndex];
                if (!(bag && bag->items.valid())) continue;
                for (GW::Item* item : bag->items) {
                    if (item && item->model_id == modelid) {
                        return item;
                    }
                }
            }

            return NULL;
        }

        uint32_t GetStoragePage(void) {
            return UI::GetPreference(UI::NumberPreference::StorageBagPage);
        }

        bool GetIsStorageOpen(void) {
            return storage_open_addr && *(uint32_t*)storage_open_addr != 0;
        }

        void RegisterItemClickCallback(
            HookEntry* entry,
            const ItemClickCallback& callback)
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

        uint32_t GetEquipmentVisibilityState() {
            auto* w = GetWorldContext();
            return w ? w->equipment_status : 0;
        }
        EquipmentStatus GetEquipmentVisibility(EquipmentType type) {
            return (EquipmentStatus)((GetEquipmentVisibilityState() >> (uint32_t)type) & 0x3);
        }
        bool SetEquipmentVisibility(EquipmentType type, EquipmentStatus state) {
            if (GetEquipmentVisibility(type) == state)
                return true;
            if (!ChangeEquipmentVisibility_Func)
                return false;
            ChangeEquipmentVisibility_Func((uint32_t)((uint32_t)state << (uint32_t)type), (uint32_t)(0x3 << (uint32_t)type));
            return true;
        }
    }

} // namespace GW
