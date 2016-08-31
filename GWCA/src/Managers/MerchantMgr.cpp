#include <GWCA\Managers\MerchantMgr.h>

#include <GWCA\Context\GameContext.h>
#include <GWCA\Context\WorldContext.h>

#include <GWCA\Utilities\PatternScanner.h>
#include <GWCA\Managers\GameThreadMgr.h>
#include <GWCA\Managers\ItemMgr.h>
#include <GWCA\Managers\MemoryMgr.h>

GW::MerchantMgr::MerchantMgr() : GWCAManager() {
	PatternScanner scan(0x401000, 0x4FF000);

	transaction_function_ = (Transaction_t)scan.FindPattern("\x8B\x45\x18\x83\xF8\x10\x76\x17\x68", "xxxxxxxxx", -0x2C);
	if (transaction_function_) {
		printf("transaction_function_ = %X\n", transaction_function_);
	} else {
		printf("transaction_function_ = ERR\n");
	}

	quote_function_ = (Transaction_t)scan.FindPattern("\x53\x56\x8B\x75\x0C\x57\x83\xFE\x10", "xxxxxxxxx", -0x9);

	if (quote_function_) {
		printf("quote_function_ = %X\n", transaction_function_);
	} else {
		printf("quote_function_ = ERR\n");
	}
}

void GW::MerchantMgr::RestoreHooks() {}

void GW::MerchantMgr::SellMerchantItem(GW::Item* itemtosell, DWORD sellquantity /*= NULL*/) {
	if (sellquantity == NULL) sellquantity = itemtosell->Quantity;

	TransactionPacket give = TransactionPacket(1);
	give.AddItem(itemtosell->ItemId, sellquantity);

	TransactionPacket recv = TransactionPacket();

	EnqueueTransaction(TransactionType::MERCHANT_SELL, 0, give, sellquantity * itemtosell->value, recv);
}

void GW::MerchantMgr::BuyMerchantItem(DWORD modelid, DWORD quantity /*= 1*/) {
	GW::Item* itemrecv = GetMerchantItemByModelID(modelid);
	if (!itemrecv) return;

	TransactionPacket give = TransactionPacket();

	TransactionPacket recv = TransactionPacket(1);
	recv.AddItem(itemrecv->ItemId, quantity);

	EnqueueTransaction(TransactionType::MERCHANT_BUY, 2 * quantity * itemrecv->value, give, 0, recv);
}

void GW::MerchantMgr::EnqueueTransaction(TransactionType type, DWORD gold_give, TransactionPacket give /*= TransactionPacket()*/, DWORD gold_recieve, TransactionPacket recieve /*= TransactionPacket()*/) {
	GameThreadMgr::Instance().Enqueue(transaction_function_, type, gold_give, give, gold_recieve, recieve);
}

void GW::MerchantMgr::EnqueueQuoteRequest(TransactionType type, TransactionPacket give, TransactionPacket recieve) {
	GameThreadMgr::Instance().Enqueue(quote_function_, type, 0, give, 0, recieve);
}

GW::Item* GW::MerchantMgr::GetMerchantItemByModelID(DWORD modelid) {
	GW::MerchItemArray merchitems = GetMerchantItemsArray();
	GW::ItemArray items = ItemMgr::Instance().GetItemArray();

	for (DWORD i = 0; i < merchitems.size_allocated(); ++i) {
		if (items[merchitems[i]]->ModelId == modelid) {
			return items[merchitems[i]];
		}
	}
	return NULL;
}

GW::MerchItemArray GW::MerchantMgr::GetMerchantItemsArray() {
	return GameContext::instance()->world->merchitems;
}

void GW::MerchantMgr::TransactionPacket::AddItem(DWORD itemid, DWORD quantity) {
	if (item_array_) item_array_[itemid_count_] = itemid;
	if (item_quantity_array_) item_quantity_array_[itemid_count_] = quantity;
	itemid_count_++;
}

GW::MerchantMgr::TransactionPacket::TransactionPacket(size_t itemmaxcount) :
	itemid_count_(0),
	item_array_(new DWORD[itemmaxcount]),
	item_quantity_array_(new DWORD[itemmaxcount]) {}

GW::MerchantMgr::TransactionPacket::TransactionPacket() :
	itemid_count_(0),
	item_array_(NULL),
	item_quantity_array_(NULL) {}

GW::MerchantMgr::TransactionPacket::~TransactionPacket() {}
