#pragma once

#include <Windows.h>

namespace GWAPI {

	class CtoSMgr {
	public:

		// Send packet that uses only dword parameters, can copypaste most gwa2 sendpackets :D
		void SendPacket(DWORD size, ...);

		// Send a packet with a specific struct alignment, used for more complex packets.
		template <class T>
		void SendPacket(T* packet)
		{
			DWORD size = sizeof(T);
			parent_->Gamethread()->Enqueue(gs_send_function_, MemoryMgr::GetGSObject(), size, (DWORD*)packet);
		}

	private:		
		typedef void(__fastcall *SendCtoGSPacket_t)(DWORD ctogsobj, DWORD size, DWORD* packet);
		friend class GWAPIMgr;
		GWAPIMgr* const parent_;

		static SendCtoGSPacket_t gs_send_function_;

		static void __fastcall packetsendintermediary(DWORD thisptr, DWORD size, DWORD* packet);

		CtoSMgr(GWAPIMgr* obj);
	};

}