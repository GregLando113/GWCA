#include <GWCA\Managers\MerchantMgr.h>

#include <GWCA\Utilities\PatternScanner.h>
#include <GWCA\Managers\ItemMgr.h>
#include <GWCA\Managers\MemoryMgr.h>
#include <GWCA\Managers\StoCMgr.h>


typedef void __fastcall TransactItem_t(
DWORD type, 
DWORD gold_give,
TransactionInfo give,
DWORD gold_recv,
TransactionInfo recv
);

typedef void __fastcall RequestQuote_t(
DWORD type, 
DWORD unknown,
QuoteInfo give,
QuoteInfo recv
);

TransactItem_t* g_transactitemfn;
RequestQuote_t* g_requestquotefn;

DWORD           g_lastitemid;
DWORD           g_lastitemprice;

GW::MerchantMgr::MerchantMgr() : GWCAManager() {
    PatternScanner scan(0x401000, 0x4FF000);
    
    g_transactitemfn = reinterpret_cast<TransactItem_t*>(scan.FindPattern("\x8B\x45\x18\x83\xF8\x10\x76\x17\x68", "xxxxxxxxx", -0x2C));
    
    if (transaction_function_) {
        printf("transaction_function_ = %X\n", transaction_function_);
    } else {
        printf("transaction_function_ = ERR\n");
    }
    
    g_requestquotefn = reinterpret_cast<RequestQuote_t*>(scan.FindPattern("\x53\x56\x8B\x75\x0C\x57\x83\xFE\x10", "xxxxxxxxx", -0x9));
    
    if (quote_function_) {
        printf("quote_function_ = %X\n", transaction_function_);
    } else {
        printf("quote_function_ = ERR\n");
    }
    
    StoCMgr::Instance().AddGameServerEvent<P235_QuotedItemPrice>([](P235_QuotedItemPrice* p) -> bool {
                                                                 g_lastitemid    = p->itemid;
                                                                 g_lastitemprice = p->price;
                                                                 return false;
                                                                 }
                                                                 );
}

void GW::MerchantMgr::RestoreHooks() {}

void 
GW::MerchantMgr::TransactItems(
TransactionType type,
DWORD gold_give,
TransactionInfo give,
DWORD gold_recv,
TransactionInfo recv
) {
    
    g_transactitemfn(
        type,
        gold_give,
        give,
        gold_recv,
        recv
        );
    
}

void
GW::MerchantMgr::RequestQuote(
TransactionType type,
QuoteInfo give,
QuoteInfo recv
) {
    
    g_lastitemprice = 0;
    g_requestquotefn(
        type,
        0,
        give,
        recv
        );
    
}

DWORD 
GW::MerchantMgr::GetQuotedItemPrice() {
    return g_lastitemprice;
}