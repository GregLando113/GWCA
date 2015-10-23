#pragma once

#include <Windows.h>
#include "GWCAManager.h"

namespace GWAPI {

	class CtoSMgr : public GWCAManager {
		friend class GWAPIMgr;

	public:

		// Send packet that uses only dword parameters, can copypaste most gwa2 sendpackets :D
		void SendPacket(DWORD size, ...);

		// Send a packet with a specific struct alignment, used for more complex packets.
		template <class T>
		void SendPacket(T* packet)
		{
			DWORD size = sizeof(T);
			api().Gamethread().Enqueue(gs_send_function_, MemoryMgr::GetGSObject(), size, (DWORD*)packet);
		}

	private:		
		typedef void(__fastcall *SendCtoGSPacket_t)(DWORD ctogsobj, DWORD size, DWORD* packet);

		static SendCtoGSPacket_t gs_send_function_;

		static void __fastcall packetsendintermediary(DWORD thisptr, DWORD size, DWORD* packet);

		CtoSMgr(GWAPIMgr& api);
		void RestoreHooks() override {}
	};

}