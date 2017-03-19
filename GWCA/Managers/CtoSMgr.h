#pragma once

#include <Windows.h>

namespace GW {
	namespace CtoS {
        // Send packet that uses only dword parameters, can copypaste most gwa2 sendpackets :D
        void SendPacket(DWORD size, ...);
        
        // Send a packet with a specific struct alignment, used for more complex packets.
        void SendPacket(DWORD size, void* buffer);

        template <class T>
        void SendPacket(T* packet) { SendPacket(sizeof(T), packet); }
    };
}
