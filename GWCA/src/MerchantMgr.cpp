#include "stdafx.h"

#include <GWCA/Constants/Constants.h>

#include <GWCA/Utilities/Export.h>
#include <GWCA/Utilities/Scanner.h>

#include <GWCA/GameContainers/Vector.h>

#include <GWCA/GameEntities/Item.h>
#include <GWCA/Context/GameContext.h>
#include <GWCA/Context/WorldContext.h>

#include <GWCA/Managers/MerchantMgr.h>

namespace {
    typedef void __fastcall TransactItem_t(
        uint32_t type,
        uint32_t gold_give,
        GW::Merchant::TransactionInfo give,
        uint32_t gold_recv,
        GW::Merchant::TransactionInfo recv
    );

    typedef void __fastcall RequestQuote_t(
        uint32_t type,
        uint32_t unknown,
        GW::Merchant::QuoteInfo give,
        GW::Merchant::QuoteInfo recv
    );

    TransactItem_t* transactitem_func;
    RequestQuote_t* requestquote_func;
}

void GW::Merchant::TransactItems(TransactionType type,
    uint32_t gold_give, TransactionInfo give,
    uint32_t gold_recv, TransactionInfo recv) {

    if (transactitem_func == nullptr) {
        transactitem_func = (TransactItem_t*)Scanner::Find("\x8B\x45\x18\x83\xF8\x10\x76\x17\x68", "xxxxxxxxx", -0x2C);
        printf("[SCAN] transaction_function_ = %p\n", transactitem_func);
    }

    if (transactitem_func) {
        transactitem_func(type, gold_give, give, gold_recv, recv);
    }
}

void GW::Merchant::RequestQuote(TransactionType type,
    QuoteInfo give, QuoteInfo recv) {

    if (requestquote_func == nullptr) {
        requestquote_func = (RequestQuote_t*)Scanner::Find("\x53\x56\x8B\x75\x0C\x57\x83\xFE\x10", "xxxxxxxxx", -0x9);
        printf("[SCAN] quote_function_ = %p\n", requestquote_func);
    }

    if (requestquote_func) {
        requestquote_func(type, 0, give, recv);
    }
}

GW::MerchItemArray GW::Merchant::GetMerchantItemsArray() {
    return GameContext::instance()->world->merch_items;
}
