#pragma once

#include <vector>
#include <map>

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
            using CallbackFunc = bool(*)(T*);
        
        
        /* Use this to add handlers to the stocmgr, primary function. */
        template <typename T>
            void AddGameServerEvent(CallbackFunc<T> handler) {
            DWORD header = Packet::StoC::Packet<T>::STATIC_HEADER;
            event_calls_[header].push_back(reinterpret_cast<CallbackFunc<Packet::StoC::PacketBase>>(handler));
            game_server_handler_[header].handlerfunc = StoCHandlerFunc;
        }
        
        template <typename T>
            void EmulatePacket(T* packet) {
            packet->header = Packet::StoC::Packet<T>::STATIC_HEADER;
            GameThreadMgr::Instance().Enqueue([this, packet]() {
                                              VoidOriginalHandler(packet);
                                              });
        }
        
        void EmulatePacket(Packet::StoC::PacketBase* packet) {
            GameThreadMgr::Instance().Enqueue([this, packet]() {
                                              VoidOriginalHandler(packet);
                                              });
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
        static std::map<DWORD, std::vector<CallbackFunc<Packet::StoC::PacketBase>>> event_calls_;
    };
}
