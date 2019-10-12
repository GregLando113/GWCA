#pragma once

#include <GWCA/Utilities/Hook.h>
#include <GWCA/Utilities/Export.h>

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
        typedef HookCallback<Packet::StoC::PacketBase *> PacketCallback;
        GWCA_API void RegisterPacketCallback(
            HookEntry *entry,
            uint32_t header,
            PacketCallback callback);
        
        /* Use this to add handlers to the stocmgr, primary function. */
        template <typename T>
        void RegisterPacketCallback(HookEntry *entry, HookCallback<T *> handler) {
            uint32_t header = Packet::StoC::Packet<T>::STATIC_HEADER;
            return RegisterPacketCallback(entry, header,
                [handler](HookStatus *status, Packet::StoC::PacketBase *pak) -> void {
                return handler(status, static_cast<T *>(pak));
            });
        }
        
        GWCA_API void RemoveCallback(uint32_t header, HookEntry *entry);

        template <typename T>
        void RemoveCallback(HookEntry *entry) {
            uint32_t header = Packet::StoC::Packet<T>::STATIC_HEADER;
            RemoveCallback(header, entry);
        }


        GWCA_API void EmulatePacket(Packet::StoC::PacketBase *packet);

        template <typename T>
        void EmulatePacket(Packet::StoC::Packet<T> *packet) {
            packet->header = Packet::StoC::Packet<T>::STATIC_HEADER;
            EmulatePacket((Packet::StoC::PacketBase *)packet);
        }
    };
}
