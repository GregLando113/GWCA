#pragma once

#include <functional>

#include "APIMain.h"

namespace GWAPI {

	class StoCMgr {
		template <typename PAK>
		using StoCHandlerFunc = (void(__fastcall *)(PAK*, DWORD));
	public:
		template <typename PacketType>
		using Handler = std::function<StoCHandlerFunc<PacketType>>;

		template <typename PacketType>
		void AddGameServerEvent(DWORD packet, Handler<PacketType> func) {

		}

	private:

		struct StoCHandler {
			DWORD* packettemplate;
			int templatesize;
			void* handlerfunc;
		};
		typedef GW::gw_array<StoCHandler> StoCHandlerArray;

		

		StoCMgr(GWAPIMgr* obj);
		~StoCMgr();

		StoCHandlerArray game_server_handler_;
		StoCHandler* original_functions_;
		GWAPIMgr* const parent_;
	};
}