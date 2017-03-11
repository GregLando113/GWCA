#pragma once

#include <vector>
#include <map>
#include <functional>

#include <GWCA\Packets\StoC.h>
#include <GWCA\GameContainers\gw_array.h>
#include "GWCAManager.h"
#include "GameThreadMgr.h"

namespace GW {
    
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
        
        template <typename T>
            using CallbackFunc = std::function<bool(T*)>;
        
        
        /* Use this to add handlers to the stocmgr, primary function. */
        template <typename T>
            DWORD AddGameServerEvent(CallbackFunc<T> handler) {
            DWORD header = Packet::StoC::Packet<T>::STATIC_HEADER;
            last_identifier_++;
            event_calls_[header][last_identifier_] = [handler](Packet::StoC::PacketBase* pak) -> bool {
				return handler((T*)pak);
			};
            game_server_handler_[header].handlerfunc = StoCHandlerFunc;

            return last_identifier_;
        }

        template <typename T>
            void RemoveGameServerEvent(DWORD identifier) {
            DWORD header = Packet::StoC::Packet<T>::STATIC_HEADER;
            event_calls_[header].erase(identifier);
        }

        template <typename T>
            void EmulatePacket(T* packet) {
            packet->header = Packet::StoC::Packet<T>::STATIC_HEADER;
            VoidOriginalHandler(packet);
        }

        void EmulatePacket(Packet::StoC::PacketBase* packet) {
            VoidOriginalHandler(packet);
        }
        
        private:
        using StoCHandler_t = bool(__fastcall *)(Packet::StoC::PacketBase* pak);
        
        struct StoCHandler {
            DWORD* packettemplate;
            int templatesize;
            StoCHandler_t handlerfunc;
        };
        using StoCHandlerArray = GW::gw_array<StoCHandler>;
        
        StoCMgr();
        void RestoreHooks() override;
        
        static bool __fastcall StoCHandlerFunc(Packet::StoC::PacketBase* pak);
        
        static void VoidOriginalHandler(Packet::StoC::PacketBase* packet) {
            original_functions_[packet->header].handlerfunc(packet);
        }
        
        static StoCHandlerArray game_server_handler_;
        static StoCHandler* original_functions_;
        static std::map<DWORD, std::map<DWORD, CallbackFunc<Packet::StoC::PacketBase>>> event_calls_;
        static DWORD last_identifier_;
    };
}
