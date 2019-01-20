#pragma once

namespace GW {
    
    /*
     StoC Manager
     See https://github.com/GameRevision/GWLP-R/wiki/GStoC for some already explored packets.
    */

    namespace Packet {
        namespace StoC {
            struct PacketBase;
        }
    }

    namespace StoC {
        template <typename T>
        using CallbackFunc = std::function<bool(T*)>;
        
        void Initialize();
        void RestoreHooks();

        GWCA_API DWORD AddCallback(DWORD header, std::function<bool(Packet::StoC::PacketBase *)> callback);
        
        /* Use this to add handlers to the stocmgr, primary function. */
        template <typename T>
        DWORD AddCallback(CallbackFunc<T> handler) {
            DWORD header = Packet::StoC::Packet<T>::STATIC_HEADER;
            return AddCallback(header, [handler](Packet::StoC::PacketBase *pak) -> bool {
                return handler((T*)pak);
            });
        }
        
        GWCA_API void RemoveCallback(DWORD header, DWORD identifier);

        template <typename T>
        void RemoveCallback(DWORD identifier) {
            DWORD header = Packet::StoC::Packet<T>::STATIC_HEADER;
            RemoveCallback(header, identifier);
        }


        GWCA_API void EmulatePacket(Packet::StoC::PacketBase *packet);

        template <typename T>
        void EmulatePacket(Packet::StoC::Packet<T> *packet) {
            packet->header = Packet::StoC::Packet<T>::STATIC_HEADER;
            EmulatePacket((Packet::StoC::PacketBase *)packet);
        }
    };
}
