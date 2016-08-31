#pragma once

#include <Windows.h>

#include "GWCAManager.h"
#include <GWCA\GameEntities\Item.h>

namespace GW {

	class MerchantMgr : public GWCAManager<MerchantMgr> {
		friend class GWCAManager<MerchantMgr>;

	public:
		enum class TransactionType : DWORD {
			MERCHANT_BUY = 0x1,
			COLLECTOR_BUY,
			CRAFTER_BUY,
			WEAPONSMITH_CUSTOMIZE,

			MERCHANT_SELL = 0xB,
			TRADER_BUY,
			TRADER_SELL,

			BALTH_PRIEST_UNLOCK_RUNE = 0xF
		};

		class TransactionPacket {
		protected:
			DWORD itemid_count_;
			DWORD* item_array_;
			DWORD* item_quantity_array_;
		public:

			// Use for an empty buy or sell operation, or if gold is only involved.
			TransactionPacket();

			// Use if items are involved in this transaction.
			TransactionPacket(size_t itemmaxcount);

			~TransactionPacket();

			// Add an item to the packet, do not add more then stated in constructor.
			void AddItem(DWORD itemid, DWORD quantity);;

		};

		GW::MerchItemArray GetMerchantItemsArray();

		GW::Item* GetMerchantItemByModelID(DWORD modelid);

		void EnqueueTransaction(
			TransactionType type,
			DWORD gold_give = 0,
			TransactionPacket give = TransactionPacket(),
			DWORD gold_recieve = 0,
			TransactionPacket recieve = TransactionPacket()
		);

		void EnqueueQuoteRequest(
			TransactionType type,
			TransactionPacket give = TransactionPacket(),
			TransactionPacket recieve = TransactionPacket()
		);

		void BuyMerchantItem(DWORD modelid, DWORD quantity = 1);

		void SellMerchantItem(GW::Item* itemtosell, DWORD sellquantity = NULL);

	private:
		typedef void(__fastcall *Transaction_t)
			(
				TransactionType type,
				DWORD gold_give,
				TransactionPacket give,
				DWORD gold_recieve,
				TransactionPacket recieve
				);

		MerchantMgr();
		void RestoreHooks() override;

		Transaction_t transaction_function_;
		Transaction_t quote_function_;
	};
}
