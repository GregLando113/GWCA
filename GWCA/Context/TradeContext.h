#ifndef _TRADE_CONTEXT_INC
#define _TRADE_CONTEXT_INC

#include <Windows.h>

#include <GWCA\GameContainers\Array.h>
#include <GWCA\GameEntities\Item.h>

namespace GW {

	struct TradeContext {
		const DWORD TRADE_CLOSED     = 0;
		const DWORD TRADE_INITIATED  = 1;
		const DWORD TRADE_OFFER_SEND = 2;
		const DWORD TRADE_ACCEPTED   = 4;

		struct Item {
			ItemID itemid;
			DWORD quantity;
		};

		struct Trader {
			DWORD gold;
			Array<TradeContext::Item> items;
		};

		/* +h0000 */ DWORD  flags; // this is actually a flags
		/* +h0004 */ DWORD  h0004[3]; // Seemingly 3 null dwords
		/* +h0010 */ Trader player;
		/* +h0024 */ Trader partner;

		// bool GetPartnerAccepted();
		// bool GetPartnerOfferSent();

		bool GetIsTradeInitiated() { return (flags & TRADE_INITIATED) != 0; }
		bool GetIsTradeAccepted()  { return (flags & TRADE_ACCEPTED)  != 0; }
	};
}

#endif // _TRADE_CONTEXT_INC
