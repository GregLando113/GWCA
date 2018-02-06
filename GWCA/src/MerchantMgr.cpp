#include <GWCA\Managers\MerchantMgr.h>

#include <GWCA\Utilities\Scanner.h>
#include <GWCA\Context\GameContext.h>

#include <stdio.h>

namespace {
	typedef void __fastcall TransactItem_t(
		DWORD type,
		DWORD gold_give,
		GW::Merchant::TransactionInfo give,
		DWORD gold_recv,
		GW::Merchant::TransactionInfo recv
	);

	typedef void __fastcall RequestQuote_t(
		DWORD type,
		DWORD unknown,
		GW::Merchant::QuoteInfo give,
		GW::Merchant::QuoteInfo recv
	);

	TransactItem_t* transactitem_func;
	RequestQuote_t* requestquote_func;
}

void GW::Merchant::TransactItems(TransactionType type,
	DWORD gold_give, TransactionInfo give,
	DWORD gold_recv, TransactionInfo recv) {

	if (transactitem_func == nullptr) {
		transactitem_func = (TransactItem_t*)Scanner::Find("\x8B\x45\x18\x83\xF8\x10\x76\x17\x68", "xxxxxxxxx", -0x2C);
		printf("transaction_function_ = %p\n", transactitem_func);
	}

	if (transactitem_func) {
		transactitem_func(type, gold_give, give, gold_recv, recv);
	}
}

void GW::Merchant::RequestQuote(TransactionType type,
	QuoteInfo give, QuoteInfo recv) {

	if (requestquote_func == nullptr) {
		requestquote_func = (RequestQuote_t*)Scanner::Find("\x53\x56\x8B\x75\x0C\x57\x83\xFE\x10", "xxxxxxxxx", -0x9);
		printf("quote_function_ = %p\n", requestquote_func);
	}

	if (requestquote_func) {
		requestquote_func(type, 0, give, recv);
	}
}

GW::MerchItemArray GW::Merchant::GetMerchantItemsArray() {
	return GameContext::instance()->world->merchitems;
}
