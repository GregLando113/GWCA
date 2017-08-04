#ifndef _TRADE_CONTEXT_INC
#define _TRADE_CONTEXT_INC

#include <Windows.h>

#include <GWCA\GameContainers\Array.h>
#include <GWCA\GameEntities\Item.h>

namespace GW {

	struct TradeContext {
		enum State : DWORD { NO_TRADE, TRADE_INITIATED, OFFER_ACCEPTED = 3 };

		struct Item {
			ItemID itemid;
			DWORD quantity;
		};

		struct Trader {
			DWORD gold;
			Array<TradeContext::Item> items;
		};

		/* +h0000 */ State  state;
		/* +h0004 */ DWORD  h0004[3]; // Seemingly 3 null dwords
		/* +h0010 */ Trader player;
		/* +h0020 */ Trader partner;

		// bool GetPartnerAccepted();
		// bool GetPartnerOfferSent();

		bool GetIsTradeInitiated() { return state >= TRADE_INITIATED; }
	};
}

#endif // _TRADE_CONTEXT_INC
