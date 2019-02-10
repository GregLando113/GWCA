#include "stdafx.h"

#include <GWCA/Constants/Constants.h>

#include <GWCA/Utilities/Export.h>
#include <GWCA/Utilities/Macros.h>
#include <GWCA/Utilities/Scanner.h>

#include <GWCA/GameContainers/Vector.h>

#include <GWCA/GameEntities/Item.h>
#include <GWCA/Context/GameContext.h>
#include <GWCA/Context/WorldContext.h>

#include <GWCA/Managers/Module.h>
#include <GWCA/Managers/MerchantMgr.h>

namespace {
    using namespace GW;

    typedef void (__fastcall *TransactItem_pt)(
        uint32_t type,
        uint32_t gold_give,
        Merchant::TransactionInfo give,
        uint32_t gold_recv,
        Merchant::TransactionInfo recv
    );

    typedef void (__fastcall *RequestQuote_pt)(
        uint32_t type,
        uint32_t unknown,
        Merchant::QuoteInfo give,
        Merchant::QuoteInfo recv
    );

    TransactItem_pt TransactItem_Func;
    RequestQuote_pt RequestQuote_func;

    void Init() {
        TransactItem_Func = (TransactItem_pt )Scanner::Find(
            "\x8B\x45\x18\x83\xF8\x10\x76\x17\x68", "xxxxxxxxx", -0x2C);
        printf("[SCAN] TransactItem = %p\n", TransactItem_Func);

        RequestQuote_func = (RequestQuote_pt )Scanner::Find(
            "\x53\x56\x8B\x75\x0C\x57\x83\xFE\x10", "xxxxxxxxx", -0x9);
        printf("[SCAN] RequestQuote = %p\n", RequestQuote_func);
    }
}

namespace GW {

    Module MerchantModule = {
        "MerchantModule",   // name
        NULL,               // param
        ::Init,             // init_module
        NULL,               // exit_module
        NULL,               // exit_module
        NULL,               // remove_hooks
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