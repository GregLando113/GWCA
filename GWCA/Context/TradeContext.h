#pragma once

#include <Windows.h>

#include <GWCA\GameContainers\Array.h>
#include <GWCA\GameEntities\Item.h>

namespace GW {

	class TradeContext {
		TradeContext() {}
		TradeContext(const TradeContext&) {}

	public:
		enum State : DWORD { NO_TRADE, TRADE_INITIATED, OFFER_ACCEPTED = 3 };

		struct Item {
			ItemID itemid;
			DWORD quantity;
		};

		struct Trader {
			DWORD gold;
			Array<TradeContext::Item> items;
		};

		State state;
		DWORD pad1[3]; // Seemingly 3 null dwords
		Trader player;
		Trader partner;
	};
}
