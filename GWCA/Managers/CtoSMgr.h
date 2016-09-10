#pragma once

#include <Windows.h>
#include "GWCAManager.h"
#include "GameThreadMgr.h"

namespace GW {

	class CtoSMgr : public GWCAManager<CtoSMgr> {
		friend class GWCAManager<CtoSMgr>;

	public:
		typedef void(__fastcall *SendCtoGSPacket_t)(DWORD ctogsobj, DWORD size, DWORD* packet);

		// Send packet that uses only dword parameters, can copypaste most gwa2 sendpackets :D
		void SendPacket(DWORD size, ...);

		// Send a packet with a specific struct alignment, used for more complex packets.
		template <class T>
		void SendPacket(T* packet) {
			GameThreadMgr::Instance().Enqueue([this, packet]() {
				DWORD size = sizeof(T);
				gs_send_function_(MemoryMgr::GetGSObject(), size, (DWORD*)packet);
			});
		}

		static SendCtoGSPacket_t gs_send_function_;
	private:

		CtoSMgr();
		void RestoreHooks() override {}
	};
}
