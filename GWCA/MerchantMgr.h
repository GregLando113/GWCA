#pragma once

#include <Windows.h>

#include "GWCAManager.h"
#include "Hooker.h"
#include "GWStructures.h"

namespace GWAPI {

	class MerchantMgr : public GWCAManager {
		friend class GWAPIMgr;

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
			DWORD gold_amount_;
			DWORD itemid_count_;
			DWORD* item_array_;
			DWORD* item_quantity_array_;
		public:

			// Use for an empty buy or sell operation, or if gold is only involved.
			TransactionPacket();

			// Use if items are involved in this transaction.
			TransactionPacket(size_t itemmaxcount);

			~TransactionPacket();

			// Set gold of packet.
			void SetGold(DWORD amount);

			// Add an item to the packet, do not add more then stated in constructor.
			void AddItem(DWORD itemid, DWORD quantity);;

		};

		GW::MerchItemArray GetMerchantItemsArray();

		GW::Item* GetMerchantItemByModelID(DWORD modelid);

		void EnqueueTransaction(TransactionType type, TransactionPacket give = TransactionPacket(), TransactionPacket recieve = TransactionPacket());

		void BuyMerchantItem(DWORD modelid,DWORD quantity = 1);

		void SellMerchantItem(GW::Item* itemtosell, DWORD sellquantity = NULL);

	private:
		typedef void(__fastcall *Transaction_t)
		(
			TransactionType type,
			TransactionPacket give,
			TransactionPacket recieve
		);



		MerchantMgr(GWAPIMgr& api);
		void RestoreHooks() override;

		Transaction_t transaction_function_;
	};
}