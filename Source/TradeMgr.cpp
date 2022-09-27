#include "stdafx.h"

#include <GWCA/Utilities/Hooker.h>
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

    typedef void(__cdecl* UpdateTradeCart_pt)(void* eax, void* a1, void* a2);
    UpdateTradeCart_pt UpdateTradeCart_Func = 0;
    UpdateTradeCart_pt RetUpdateTradeCart = 0;

    typedef bool(__cdecl* DoAction_pt)(uint32_t identifier);
    DoAction_pt TradeSendOffer_Func = 0;
    DoAction_pt TradeRemoveItem_Func = 0;

    typedef bool(__cdecl* Void_pt)();
    Void_pt TradeCancel_Func = 0;
    Void_pt TradeCancelOffer_Func = 0;
    Void_pt TradeAcceptOffer_Func = 0;

    static void __cdecl OnUpdateTradeWindow(void* eax, void* a1, void* a2) {
        uintptr_t* address = (uintptr_t*)eax + 2;
        if (address && *address) {
            address = *(uintptr_t**)address;
            trade_window_context = static_cast<TradeWindow*>(*(TradeWindow**)address);
        }
        RetUpdateTradeCart(eax, a1, a2);
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
        /*
            A lot of these function signatures need to update the trade window UI to show the player whats going on.
            So far this has only been done for offering a trade item, but things like submitting an offer currently don't update the UI which is a problem.

            TODO: Move the RVA's for submitting etc further up the UI tree in order to trigger the UI to update.
        */
        OfferTradeItem_Func = (OfferTradeItem_pt)Scanner::Find("\x68\x49\x04\x00\x00\x89\x5D\xE4\xE8", "xxxxxxxxx", -0x6B);
        UpdateTradeCart_Func = (UpdateTradeCart_pt)Scanner::Find("\x57\x8B\x7D\x0C\x3D\xEF\x00\x00\x10", "xxxxxxxxx", -0x24);

        // typedef void(__fastcall*)(void* ecx, void* edx, UI::InteractionMessage*); 06f0
        DWORD address = Scanner::Find("\x8b\x41\x04\x83\xf8\x0e\x0f\x87\x82\x02\x00\x00", "xxxxxxxxxxxx", -0xc);
        TradeCancelOffer_Func = (Void_pt)Scanner::FunctionFromNearCall(address + 0xa6);
        TradeSendOffer_Func = (DoAction_pt)Scanner::FunctionFromNearCall(address + 0x101);

        address = Scanner::Find("\x83\xc4\x04\xf7\x40\x0c\x00\x00\x00\x40", "xxxxxxxxxx", -0x85);
        TradeAcceptOffer_Func = (Void_pt)Scanner::FunctionFromNearCall(address);

        address = Scanner::Find("\x8b\x46\x14\x8b\x00\x85\xc0", "xxxxxxx", 0xa);
        TradeRemoveItem_Func = (DoAction_pt)Scanner::FunctionFromNearCall(address);

        address = Scanner::FindAssertion("p:\\code\\gw\\ui\\game\\gmtrade.cpp", "breakClose", 0x1e);
        TradeCancel_Func = (Void_pt)Scanner::FunctionFromNearCall(address);

        GWCA_INFO("[SCAN] OfferTradeItem_Func = %p", OfferTradeItem_Func);
        GWCA_INFO("[SCAN] UpdateTradeCart_Func = %p", UpdateTradeCart_Func);
        GWCA_INFO("[SCAN] TradeCancelOffer_Func = %p", TradeCancelOffer_Func);
        GWCA_INFO("[SCAN] TradeSendOffer_Func = %p", TradeSendOffer_Func);
        GWCA_INFO("[SCAN] TradeAcceptOffer_Func = %p", TradeAcceptOffer_Func);
        GWCA_INFO("[SCAN] TradeRemoveItem_Func = %p", TradeRemoveItem_Func);
        GWCA_INFO("[SCAN] TradeCancel_Func = %p", TradeCancel_Func);

#ifdef _DEBUG
        GWCA_ASSERT(OfferTradeItem_Func);
        GWCA_ASSERT(TradeCancelOffer_Func);
        GWCA_ASSERT(TradeCancelOffer_Func);
        GWCA_ASSERT(TradeSendOffer_Func);
        GWCA_ASSERT(TradeAcceptOffer_Func);
        GWCA_ASSERT(TradeRemoveItem_Func);
        GWCA_ASSERT(TradeCancel_Func);
#endif

        HookBase::CreateHook(OfferTradeItem_Func, OnOfferTradeItem, (void**)&RetOfferTradeItem);
        HookBase::CreateHook(UpdateTradeCart_Func, OnUpdateTradeWindow, (void**)&RetUpdateTradeCart);
    }
    void EnableHooks() {
        if(OfferTradeItem_Func)
            HookBase::EnableHooks(OfferTradeItem_Func);
        if(UpdateTradeCart_Func)
            HookBase::EnableHooks(UpdateTradeCart_Func);
    }
    void DisableHooks() {
        if (OfferTradeItem_Func)
            HookBase::DisableHooks(OfferTradeItem_Func);
        if (UpdateTradeCart_Func)
            HookBase::DisableHooks(UpdateTradeCart_Func);
    }
    void Exit() {
        HookBase::RemoveHook(OfferTradeItem_Func);
        HookBase::RemoveHook(UpdateTradeCart_Func);
    }
}
namespace GW {
    Module TradeModule = {
        "TradeModule",  // name
        NULL,           // param
        ::Init,           // init_module
        ::Exit,           // exit_module
        ::EnableHooks,           // enable_hooks
        ::DisableHooks,           // disable_hooks
    };

    bool Trade::OpenTradeWindow(uint32_t agent_id) {
        return UI::SendUIMessage(GW::UI::UIMessage::kInitiateTrade, (void*)agent_id);
    }

    bool Trade::AcceptTrade() {
        return TradeCancel_Func ? TradeCancel_Func() : false;
    }

    bool Trade::CancelTrade() {
        return TradeCancel_Func ? TradeCancel_Func() : false;
    }

    bool Trade::ChangeOffer() {
        return TradeCancelOffer_Func ? TradeCancelOffer_Func() : false;
    }

    bool Trade::SubmitOffer(uint32_t gold) {
        return TradeSendOffer_Func ? TradeSendOffer_Func(gold) : false;
    }
    bool Trade::RemoveItem(uint32_t slot) {
        return TradeRemoveItem_Func ? TradeRemoveItem_Func(slot) : false;
    }
    bool Trade::OfferItem(uint32_t item_id, uint32_t quantity) {
        if (!(OfferTradeItem_Func && trade_window_context && !trade_window_context->isDisabled())) {
            return false;
        }
        OfferTradeItem_Func(trade_window_context, 0, item_id, quantity, 1);
        return true;
    }

    void Trade::RegisterOfferItemCallback(HookEntry* entry, const OfferItemCallback& callback) {
        OnOfferItem_callbacks.insert({ entry, callback });
    }
    void Trade::RemoveOfferItemCallback(HookEntry* entry) {
        auto it = OnOfferItem_callbacks.find(entry);
        if (it != OnOfferItem_callbacks.end())
            OnOfferItem_callbacks.erase(it);
    }
} // namespace GW
