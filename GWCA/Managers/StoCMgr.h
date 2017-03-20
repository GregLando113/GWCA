#pragma once

#include <functional>

#include <GWCA\Packets\StoC.h>

namespace GW {
    
    /*
     StoC Manager
     See https://github.com/GameRevision/GWLP-R/wiki/GStoC for some already explored packets.
    */
    
	namespace StoC {

		template <typename T>
		using CallbackFunc = std::function<bool(T*)>;
        
		void Initialize();
		void RestoreHooks();

		DWORD AddCallback(DWORD header, std::function<bool(Packet::StoC::PacketBase*)> callback);
        
        /* Use this to add handlers to the stocmgr, primary function. */
        template <typename T>
		DWORD AddCallback(CallbackFunc<T> handler) {
			DWORD header = Packet::StoC::Packet<T>::STATIC_HEADER;
			return AddCallback(header, [handler](Packet::StoC::PacketBase* pak) -> bool {
				return handler((T*)pak);
			});
		}
		
		void RemoveCallback(DWORD header, DWORD identifier);

        template <typename T>
        void RemoveCallback(DWORD identifier) {
            DWORD header = Packet::StoC::Packet<T>::STATIC_HEADER;
			RemoveCallback(header, identifier);
        }


		void EmulatePacket(Packet::StoC::PacketBase* packet);

        template <typename T>
        void EmulatePacket(T* packet) {
            packet->header = Packet::StoC::Packet<T>::STATIC_HEADER;
			EmulatePacket(packet);
        }
    };
}
