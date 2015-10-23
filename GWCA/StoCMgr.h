#pragma once

#include <functional>
#include <vector>
#include <map>

#include "GWCAManager.h"
#include "StoCPackets.h"
#include "GWStructures.h"

namespace GWAPI {

	/*
		StoC Manager
		See https://github.com/GameRevision/GWLP-R/wiki/GStoC for some already explored packets.
	*/

	class StoCMgr : public GWCAManager {
		friend class GWAPIMgr;

	public:

		/* 
			Base structure definition for stoc packets
			Inherit this, then ignore added header, just all other fields of packet in your definitions.
		*/

		typedef std::function<void(StoC::PacketBase*)> Handler;

		template <typename T>
		using CallbackFunc = std::function<void(T*)>;


		/* Use this to add handlers to the stocmgr, primary function. */
		template <typename T>
		void AddGameServerEvent(CallbackFunc<T> handler) {
			DWORD header = StoC::Packet<T>::STATIC_HEADER;
			event_calls_[header].push_back([handler](StoC::PacketBase* pak) {
				handler((T*)pak);
			});
			game_server_handler_[header].handlerfunc = StoCHandlerFunc;
		}

	private:
		typedef bool(__fastcall *StoCHandler_t)(StoC::PacketBase* pak, DWORD unk);

		struct StoCHandler {
			DWORD* packettemplate;
			int templatesize;
			StoCHandler_t handlerfunc;
		};
		typedef GW::gw_array<StoCHandler> StoCHandlerArray;

		StoCMgr(GWAPIMgr& api);
		void RestoreHooks() override;

		static bool __fastcall StoCHandlerFunc(StoC::PacketBase* pak, DWORD unk);

		static StoCHandlerArray game_server_handler_;
		static StoCHandler* original_functions_;
		static std::map<DWORD,std::vector<Handler>> event_calls_;
	};
}