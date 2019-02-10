#pragma once

namespace GW {
    struct Module;
    extern Module CtoSModule;

    namespace CtoS {
        // Send packet that uses only dword parameters, can copypaste most gwa2 sendpackets :D
        GWCA_API void SendPacket(uint32_t size, ...);
        
        // Send a packet with a specific struct alignment, used for more complex packets.
        GWCA_API void SendPacket(uint32_t size, void* buffer);

        template <class T>
        inline void SendPacket(T *packet) {
            SendPacket(sizeof(T), packet);
        }
    };
}
