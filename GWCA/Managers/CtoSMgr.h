#pragma once

#include <Windows.h>
#include "GWCAManager.h"
#include "GameThreadMgr.h"

namespace GW {
    
    class CtoSMgr : public GWCAManager<CtoSMgr> {
        friend class GWCAManager<CtoSMgr>;
        
        public:

        // Send packet that uses only dword parameters, can copypaste most gwa2 sendpackets :D
        void SendPacket(DWORD size, ...);
        
        // Send a packet with a specific struct alignment, used for more complex packets.
        void SendPacket(DWORD size, void* buffer);

        template <class T>
        void SendPacket(T* packet) { SendPacket(sizeof(t),packet); }
        
        static SendCtoGSPacket_t gs_send_function_;
        private:
        
        CtoSMgr();
        void RestoreHooks() override {}
    };
}
