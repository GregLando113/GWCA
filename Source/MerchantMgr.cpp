#include "stdafx.h"

#include <GWCA/Constants/Constants.h>

#include <GWCA/Utilities/Debug.h>
#include <GWCA/Utilities/Export.h>
#include <GWCA/Utilities/Macros.h>
#include <GWCA/Utilities/Scanner.h>

#include <GWCA/GameContainers/GamePos.h>

#include <GWCA/GameEntities/Item.h>
#include <GWCA/Context/GameContext.h>
#include <GWCA/Context/WorldContext.h>

#include <GWCA/Managers/Module.h>
#include <GWCA/Managers/MerchantMgr.h>

namespace {
    using namespace GW;

    typedef void (__cdecl *TransactItem_pt)(
        uint32_t type,
        uint32_t gold_give,
        Merchant::TransactionInfo give,
        uint32_t gold_recv,
        Merchant::TransactionInfo recv
    );

    typedef void (__cdecl *RequestQuote_pt)(
        uint32_t type,
        uint32_t unknown,
        Merchant::QuoteInfo give,
        Merchant::QuoteInfo recv
    );

    TransactItem_pt TransactItem_Func;
    RequestQuote_pt RequestQuote_func;

    void Init() {
        TransactItem_Func = (TransactItem_pt )Scanner::Find(
            "\x85\xFF\x74\x1D\x8B\x4D\x14\xEB\x08", "xxxxxxxxx", -0x7F);
        GWCA_INFO("[SCAN] TransactItem = %p\n", TransactItem_Func);

        RequestQuote_func = (RequestQuote_pt )Scanner::Find(
            "\x8B\x75\x20\x83\xFE\x10\x76\x14", "xxxxxxxx", -0x35);
        GWCA_INFO("[SCAN] RequestQuote = %p\n", RequestQuote_func);
    }
}

namespace GW {

    Module MerchantModule = {
        "MerchantModule",   // name
        NULL,               // param
        ::Init,             // init_module
        NULL,               // exit_module
        NULL,               // enable_hooks
        NULL,               // disable_hooks
    };

    void Merchant::TransactItems(TransactionType type,
        uint32_t gold_give, TransactionInfo give,
        uint32_t gold_recv, TransactionInfo recv) {

        if (Verify(TransactItem_Func))
            TransactItem_Func(type, gold_give, give, gold_recv, recv);
    }

    void Merchant::RequestQuote(TransactionType type,
        QuoteInfo give, QuoteInfo recv) {

        if (Verify(RequestQuote_func))
            RequestQuote_func(type, 0, give, recv);
    }

    MerchItemArray Merchant::GetMerchantItemsArray() {
        return GameContext::instance()->world->merch_items;
    }
} // namespace GW