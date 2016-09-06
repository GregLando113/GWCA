#pragma once

#include <functional>
#include <vector>
#include <map>

#include "GWCAManager.h"
#include "StoCPackets.h"
#include "GWStructures.h"
#include "GameThreadMgr.h"

namespace GWCA {

	/*
		StoC Manager
		See https://github.com/GameRevision/GWLP-R/wiki/GStoC for some already explored packets.
	*/

	class StoCMgr : public GWCAManager<StoCMgr> {
		friend class GWCAManager<StoCMgr>;

	public:

		/* 
			Base structure definition for stoc packets
			Inherit this, then ignore added header, just all other fields of packet in your definitions.
		*/

		typedef std::function<bool(StoC_Pak::PacketBase*)> Handler;

		template <typename T>
		using CallbackFunc = std::function<bool(T*)>;


		/* Use this to add handlers to the stocmgr, primary function. */
		template <typename T>
		void AddGameServerEvent(CallbackFunc<T> handler) {
			DWORD header = StoC_Pak::Packet<T>::STATIC_HEADER;
			event_calls_[header].push_back([handler](StoC_Pak::PacketBase* pak) -> bool {
				return handler((T*)pak);
			});
			game_server_handler_[header].handlerfunc = StoCHandlerFunc;
		}

		template <typename T>
		void EmulatePacket(T* packet) {
			packet->header = StoC_Pak::Packet<T>::STATIC_HEADER;
			GameThreadMgr::Instance().Enqueue(VoidOriginalHandler, packet);
		}

		void EmulatePacket(StoC_Pak::PacketBase* packet) {
			GameThreadMgr::Instance().Enqueue(VoidOriginalHandler, packet);
		}

	private:
		using StoCHandler_t = bool(__fastcall *)(StoC_Pak::PacketBase* pak);

		struct StoCHandler {
			DWORD* packettemplate;
			int templatesize;
			StoCHandler_t handlerfunc;
		};
		using StoCHandlerArray = GW::gw_array<StoCHandler>;

		StoCMgr();
		void RestoreHooks() override;

		static bool __fastcall StoCHandlerFunc(StoC_Pak::PacketBase* pak);

		static void VoidOriginalHandler(StoC_Pak::PacketBase* packet) {
			original_functions_[packet->header].handlerfunc(packet);
		}

		static StoCHandlerArray game_server_handler_;
		static StoCHandler* original_functions_;
		static std::map<DWORD,std::vector<Handler>> event_calls_;
	};
}
