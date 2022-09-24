#pragma once

#include <GWCA/Utilities/Export.h>
#include <GWCA/Utilities/Hook.h>

#include <GWCA/Managers/Module.h>


namespace GW {
    struct Module;
    extern Module CtoSModule;

    namespace CtoS {
        typedef HookCallback<void*> PacketCallback;
        // Send packet that uses only dword parameters, can copypaste most gwa2 sendpackets :D. Returns true if enqueued.
        GWCA_API bool SendPacket(uint32_t size, ...);

        GWCA_API void RegisterPacketCallback(
            HookEntry* entry,
            uint32_t header,
            const PacketCallback& callback);

        GWCA_API void RemoveCallback(uint32_t header, HookEntry* entry);
        // Send a packet with a specific struct alignment, used for more complex packets. Returns true if enqueued.
        GWCA_API bool SendPacket(uint32_t size, void* buffer);

        template <class T>
        inline bool SendPacket(T *packet) {
            return SendPacket(sizeof(T), packet);
        }
    };
}
