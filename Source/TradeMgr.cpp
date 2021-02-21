#include "stdafx.h"

#include <GWCA/Packets/Opcodes.h>

#include <GWCA/Utilities/Export.h>
#include <GWCA/Utilities/Hooker.h>
#include <GWCA/Utilities/Macros.h>
#include <GWCA/Utilities/Scanner.h>

#include <GWCA/GameEntities/Item.h>

#include <GWCA/Managers/ItemMgr.h>
#include <GWCA/Managers/Module.h>
#include <GWCA/Managers/CtoSMgr.h>
#include <GWCA/Managers/TradeMgr.h>
#include <GWCA/Managers/UIMgr.h>

namespace {
    using namespace GW;

    struct TradeWindow {
        uint32_t unk1;
        uint32_t items_max;
        uint32_t frame_id;
        uint32_t state;
        uint32_t items_count;
        inline bool isDisabled() { return state != 0x0;  }
        // ...
    } *trade_window_context = nullptr;

    GW::HookEntry on_trade_action_entry;

    std::unordered_map<HookEntry*, Trade::OfferItemCallback> OnOfferItem_callbacks;
    
    typedef void(__fastcall* OfferTradeItem_pt)(void* ecx, void* edx, uint32_t item_id, uint32_t quantity, uint32_t always_one);
    OfferTradeItem_pt OfferTradeItem_Func = 0;
    OfferTradeItem_pt RetOfferTradeItem = 0;

    typedef void(__fastcall* RemoveTradeItem_pt)(void* ecx, void* edx, uint32_t slot, uint32_t always_one);
    RemoveTradeItem_pt RemoveTradeItem_Func = 0;

    typedef void(__cdecl* UpdateTradeWindow_pt)(void* eax, void* a1, void* a2);
    UpdateTradeWindow_pt UpdateTradeWindow_Func = 0;
    UpdateTradeWindow_pt RetUpdateTradeWindow = 0;

    static void __cdecl OnUpdateTradeWindow(void* eax, void* a1, void* a2) {
        uintptr_t* address = (uintptr_t*)eax + 2;
        if (address && *address) {
            address = *(uintptr_t**)address;
            trade_window_context = static_cast<TradeWindow*>(*(TradeWindow**)address);
        }
        RetUpdateTradeWindow(eax, a1, a2);
    }
    static void OnActionCallback(GW::HookStatus*, void* ecx, uint32_t action_type, void*, void* wParam) {
        if (action_type == 0x4A && !wParam)
            trade_window_context = static_cast<TradeWindow*>(*(TradeWindow**)ecx + 0x4);
    }
    static void __fastcall OnOfferTradeItem(void* ecx, void* edx, uint32_t item_id, uint32_t quantity, uint32_t always_one) {
        HookBase::EnterHook();
        HookStatus status;
        for (auto& it : OnOfferItem_callbacks) {
            it.second(&status, item_id, quantity);
            ++status.altitude;
        }
        if (!status.blocked)
            RetOfferTradeItem(ecx, edx, item_id, quantity, always_one);
        HookBase::LeaveHook();
    }

    void Init() {
        OfferTradeItem_Func = (OfferTradeItem_pt)Scanner::Find("\x68\x49\x04\x00\x00\x89\x5D\xE4\xE8", "xxxxxxxxx", -0x6B);
        GWCA_INFO("[SCAN] OfferTradeItem_Func = %p\n", OfferTradeItem_Func);
        if (Verify(OfferTradeItem_Func))
            HookBase::CreateHook(OfferTradeItem_Func, OnOfferTradeItem, (void**)&RetOfferTradeItem);

        RemoveTradeItem_Func = (RemoveTradeItem_pt)Scanner::Find("\x6A\x01\x57\x89\x5D\xDC\xE8", "xxxxxxx", -0x18);
        GWCA_INFO("[SCAN] RemoveTradeItem_Func = %p\n", OfferTradeItem_Func);

        UpdateTradeWindow_Func = (UpdateTradeWindow_pt)Scanner::Find("\x57\x8B\x7D\x0C\x3D\xEF\x00\x00\x10", "xxxxxxxxx", -0x24);
        if (Verify(UpdateTradeWindow_Func))
            HookBase::CreateHook(UpdateTradeWindow_Func, OnUpdateTradeWindow, (void**)&RetUpdateTradeWindow);
        GWCA_INFO("[SCAN] UpdateTradeWindow_Func = %p\n", UpdateTradeWindow_Func);

    }
}
namespace GW {
    Module TradeModule = {
        "TradeModule",  // name
        NULL,           // param
        ::Init,           // init_module
        NULL,           // exit_module
        NULL,           // enable_hooks
        NULL,           // disable_hooks
    };

    void Trade::OpenTradeWindow(uint32_t agent_id) {
        CtoS::SendPacket(0x8, GAME_CMSG_TRADE_INITIATE, agent_id);
    }

    void Trade::AcceptTrade() {
        CtoS::SendPacket(0x4, GAME_CMSG_TRADE_ACCEPT);
    }

    void Trade::CancelTrade() {
        CtoS::SendPacket(0x4, GAME_CMSG_TRADE_CANCEL);
    }

    void Trade::ChangeOffer() {
        CtoS::SendPacket(0x4, GAME_CMSG_TRADE_CANCEL_OFFER);
    }

    void Trade::SubmitOffer(uint32_t gold) {
        CtoS::SendPacket(0x8, GAME_CMSG_TRADE_SEND_OFFER, gold);
    }
    void Trade::OfferItem(uint32_t item_id, uint32_t quantity) {
        if (OfferTradeItem_Func && trade_window_context && !trade_window_context->isDisabled()) {
            OnOfferTradeItem(trade_window_context, 0, item_id, quantity, 1);
        }
        //CtoS::SendPacket(0xC, GAME_CMSG_TRADE_ADD_ITEM, item_id, quantity);
    }

    void Trade::RegisterOfferItemCallback(HookEntry* entry, OfferItemCallback callback) {
        OnOfferItem_callbacks.insert({ entry, callback });
    }
    void Trade::RemoveOfferItemCallback(HookEntry* entry) {
        auto it = OnOfferItem_callbacks.find(entry);
        if (it != OnOfferItem_callbacks.end())
            OnOfferItem_callbacks.erase(it);
    }
} // namespace GW
