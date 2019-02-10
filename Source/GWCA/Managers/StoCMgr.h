#pragma once

namespace GW {
    
    /*
     StoC Manager
     See https://github.com/GameRevision/GWLP-R/wiki/GStoC for some already explored packets.
    */

    namespace Packet {
        namespace StoC {
            struct PacketBase;

            template <typename T>
            struct Packet;
        }
    }

    struct Module;
    extern Module StoCModule;

    namespace StoC {
        template <typename T>
        using CallbackFunc = std::function<bool (T*)>;
        
        GWCA_API uint32_t AddCallback(uint32_t header,
            std::function<bool (Packet::StoC::PacketBase *)> callback);
        
        /* Use this to add handlers to the stocmgr, primary function. */
        template <typename T>
        uint32_t AddCallback(CallbackFunc<T> handler) {
            uint32_t header = Packet::StoC::Packet<T>::STATIC_HEADER;
            return AddCallback(header, [handler](Packet::StoC::PacketBase *pak) -> bool {
                return handler((T *)pak);
            });
        }
        
        GWCA_API void RemoveCallback(uint32_t header, uint32_t identifier);

        template <typename T>
        void RemoveCallback(uint32_t identifier) {
            uint32_t header = Packet::StoC::Packet<T>::STATIC_HEADER;
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
