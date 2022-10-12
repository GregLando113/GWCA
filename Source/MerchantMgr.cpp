#include "stdafx.h"

#include <GWCA/Utilities/Debug.h>
#include <GWCA/Utilities/Macros.h>
#include <GWCA/Utilities/Scanner.h>
#include <GWCA/Utilities/Hook.h>
#include <GWCA/Utilities/Hooker.h>

#include <GWCA/GameEntities/Item.h>
#include <GWCA/Context/WorldContext.h>

#include <GWCA/Managers/Module.h>
#include <GWCA/Managers/MerchantMgr.h>
#include <GWCA/Managers/UIMgr.h>

namespace {
    using namespace GW;

    struct TransactItemStruct {
        Merchant::TransactionType type;
        uint32_t gold_give;
        Merchant::TransactionInfo give;
        uint32_t gold_recv;
        Merchant::TransactionInfo recv;
    };

    HookEntry OnTransactItemEntry;
    typedef void (__cdecl *TransactItem_pt)(
        Merchant::TransactionType type,
        uint32_t gold_give,
        Merchant::TransactionInfo give,
        uint32_t gold_recv,
        Merchant::TransactionInfo recv
    );
    TransactItem_pt TransactItem_Func = 0;
    TransactItem_pt TransactItem_Ret = 0;

    void OnTransactItem(Merchant::TransactionType type,
        uint32_t gold_give,
        Merchant::TransactionInfo give,
        uint32_t gold_recv,
        Merchant::TransactionInfo recv) {
        Hook::EnterHook();
        TransactItemStruct packet = { type, gold_give, give, gold_recv, recv };
        UI::SendUIMessage(UI::UIMessage::kSendMerchantTransactItem, (void*)&packet);
        Hook::LeaveHook();
    };
    void OnTransactItem_UIMessage(GW::HookStatus* status, UI::UIMessage message_id, void* wparam, void*) {
        GWCA_ASSERT(message_id == UI::UIMessage::kSendMerchantTransactItem && wparam);
        if (!status->blocked) {
            TransactItemStruct* packet = (TransactItemStruct*)wparam;
            TransactItem_Ret(packet->type, packet->gold_give, packet->give, packet->gold_recv, packet->recv);
        }
    }

    struct RequestQuoteStruct {
        Merchant::TransactionType type;
        uint32_t unknown;
        Merchant::QuoteInfo give;
        Merchant::QuoteInfo recv;
    };

    HookEntry OnRequestQuoteItemEntry;
    typedef void (__cdecl *RequestQuote_pt)(
        Merchant::TransactionType type,
        uint32_t unknown,
        Merchant::QuoteInfo give,
        Merchant::QuoteInfo recv
    );
    RequestQuote_pt RequestQuote_func;
    RequestQuote_pt RequestQuote_Ret;

    void OnRequestQuote(Merchant::TransactionType type,
        uint32_t unknown,
        Merchant::QuoteInfo give,
        Merchant::QuoteInfo recv) {
        Hook::EnterHook();
        RequestQuoteStruct packet = { type, unknown, give, recv };
        UI::SendUIMessage(UI::UIMessage::kSendMerchantRequestQuote, (void*)&packet);
        Hook::LeaveHook();
    };
    void OnRequestQuote_UIMessage(GW::HookStatus* status, UI::UIMessage message_id, void* wparam, void*) {
        GWCA_ASSERT(message_id == UI::UIMessage::kSendMerchantRequestQuote && wparam);
        if (!status->blocked) {
            RequestQuoteStruct* packet = (RequestQuoteStruct*)wparam;
            RequestQuote_Ret(packet->type, packet->unknown, packet->give, packet->recv);
        }
    }


    void Init() {
        TransactItem_Func = (TransactItem_pt )Scanner::Find("\x85\xFF\x74\x1D\x8B\x4D\x14\xEB\x08", "xxxxxxxxx", -0x7F);
        RequestQuote_func = (RequestQuote_pt )Scanner::Find("\x8B\x75\x20\x83\xFE\x10\x76\x14", "xxxxxxxx", -0x35);


        GWCA_INFO("[SCAN] TransactItem Function = %p", TransactItem_Func);
        GWCA_INFO("[SCAN] RequestQuote Function = %p", RequestQuote_func);

#ifdef _DEBUG
        GWCA_ASSERT(TransactItem_Func);
        GWCA_ASSERT(RequestQuote_func);
#endif
        if (TransactItem_Func) {
            HookBase::CreateHook(TransactItem_Func, OnTransactItem, (void**)&TransactItem_Ret);
            UI::RegisterUIMessageCallback(&OnTransactItemEntry, UI::UIMessage::kSendMerchantTransactItem, OnTransactItem_UIMessage, 0x1);
        }
        if (RequestQuote_func) {
            HookBase::CreateHook(RequestQuote_func, OnRequestQuote, (void**)&RequestQuote_Ret);
            UI::RegisterUIMessageCallback(&OnRequestQuoteItemEntry, UI::UIMessage::kSendMerchantRequestQuote, OnRequestQuote_UIMessage, 0x1);
        }
    }
    void DisableHooks() {
        if (TransactItem_Func)
            HookBase::DisableHooks(TransactItem_Func);
        if (RequestQuote_func)
            HookBase::DisableHooks(RequestQuote_func);
    }
    void EnableHooks() {
        if (TransactItem_Func)
            HookBase::EnableHooks(TransactItem_Func);
        if (RequestQuote_func)
            HookBase::EnableHooks(RequestQuote_func);
    }
    void Exit() {
        HookBase::RemoveHook(TransactItem_Func);
        HookBase::RemoveHook(RequestQuote_func);
    }
}

namespace GW {

    Module MerchantModule = {
        "MerchantModule",   // name
        NULL,               // param
        ::Init,             // init_module
        ::Exit,               // exit_module
        ::EnableHooks,               // enable_hooks
        ::DisableHooks,               // disable_hooks
    };
    namespace Merchant {

        bool TransactItems(TransactionType type,
            uint32_t gold_give, TransactionInfo give,
            uint32_t gold_recv, TransactionInfo recv) {
            if (Verify(TransactItem_Func)) {
                TransactItem_Func(type, gold_give, give, gold_recv, recv);
                return true;
            }
            return false;
        }

        bool RequestQuote(TransactionType type,
            QuoteInfo give, QuoteInfo recv) {
            if (Verify(RequestQuote_func)) {
                RequestQuote_func(type, 0, give, recv);
                return true;
            }
            return false;
        }

        MerchItemArray* GetMerchantItemsArray() {
            auto* w = GetWorldContext();
            return w && w->merch_items.valid() ? &w->merch_items : nullptr;
        }
    }
} // namespace GW
