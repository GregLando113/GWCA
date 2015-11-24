#include "MerchantMgr.h"

#include "PatternScanner.h"
#include "GWAPIMgr.h"


GWAPI::MerchantMgr::MerchantMgr(GWAPIMgr& api) : GWCAManager(api)
{
	PatternScanner scan(0x401000,0x4FF000);

	transaction_function_ = (Transaction_t)scan.FindPattern("\x8B\x45\x18\x83\xF8\x10\x76\x17\x68", "xxxxxxxxx", -0x2C);
	if (transaction_function_){
		printf("TransactionFunction = %X\n", transaction_function_);
	}
	else {
		printf("TransactionFunction = ERR\n");
	}
}

void GWAPI::MerchantMgr::RestoreHooks()
{
}

void GWAPI::MerchantMgr::SellMerchantItem(GW::Item* itemtosell, DWORD sellquantity /*= NULL*/)
{
	if (sellquantity == NULL) sellquantity = itemtosell->Quantity;

	TransactionPacket give = TransactionPacket(1);
	give.AddItem(itemtosell->ItemId, sellquantity);

	TransactionPacket recv = TransactionPacket();

	EnqueueTransaction(TransactionType::MERCHANT_SELL,0, give, sellquantity * itemtosell->value, recv);
}

void GWAPI::MerchantMgr::BuyMerchantItem(DWORD modelid, DWORD quantity /*= 1*/)
{
	GW::Item* itemrecv = GetMerchantItemByModelID(modelid);
	if (!itemrecv) return;

	TransactionPacket give = TransactionPacket();

	TransactionPacket recv = TransactionPacket(1);
	recv.AddItem(itemrecv->ItemId, quantity);

	EnqueueTransaction(TransactionType::MERCHANT_BUY, 2 * quantity * itemrecv->value, give, 0, recv);
}

void GWAPI::MerchantMgr::EnqueueTransaction(TransactionType type, DWORD gold_give, TransactionPacket give /*= TransactionPacket()*/, DWORD gold_recieve, TransactionPacket recieve /*= TransactionPacket()*/)
{
	api().Gamethread().Enqueue(transaction_function_, type, gold_give, give, gold_recieve, recieve);
}

GWAPI::GW::Item* GWAPI::MerchantMgr::GetMerchantItemByModelID(DWORD modelid)
{
	GW::MerchItemArray merchitems = GetMerchantItemsArray();
	GW::ItemArray items = api().Items().GetItemArray();

	for (DWORD i = 0; i < merchitems.size_allocated(); ++i)
	{
		if (items[merchitems[i]]->ModelId == modelid)
		{
			return items[merchitems[i]];
		}
	}
	return NULL;
}

GWAPI::GW::MerchItemArray GWAPI::MerchantMgr::GetMerchantItemsArray()
{
	return *MemoryMgr::ReadPtrChain<GW::MerchItemArray*>(MemoryMgr::GetContextPtr(), 2, 0x2C, 0x24);
}

void GWAPI::MerchantMgr::TransactionPacket::AddItem(DWORD itemid, DWORD quantity)
{
	if (item_array_) item_array_[itemid_count_] = itemid;
	if (item_quantity_array_) item_quantity_array_[itemid_count_] = quantity;
	itemid_count_++;
}

GWAPI::MerchantMgr::TransactionPacket::TransactionPacket(size_t itemmaxcount) :
itemid_count_(0),
item_array_(new DWORD[itemmaxcount]),
item_quantity_array_(new DWORD[itemmaxcount])
{

}

GWAPI::MerchantMgr::TransactionPacket::TransactionPacket() :
itemid_count_(0),
item_array_(NULL),
item_quantity_array_(NULL)
{

}

GWAPI::MerchantMgr::TransactionPacket::~TransactionPacket()
{
}
