#pragma once

#include <GWCA\GameContainers\Array.h>
#include <GWCA\Utilities\Macros.h>
#include <GWCA\Packets\StoC.h>

namespace GW {

	struct MsgConn {

		struct Cipher {
			int x, y;
			char m[256];
		};

		typedef bool(__fastcall *StoCHandler_t)(Packet::StoC::PacketBase* pak);
		struct MsgProtocol {
			DWORD* params;
			DWORD  paramcount;
		};
		struct StoCMsgProtocol 
			: public MsgProtocol {
			StoCHandler_t handlerfunc;
		};
		
		struct Protocol {
			char pad_0000[0x1C];
			Array<MsgProtocol> ctos_table;
			Array<StoCMsgProtocol> stoc_table;
		}; 

		char pad_0000[8];
		Protocol* protocol;
		char pad_000C[0x94 - 0xC];
		Cipher recv_cipher; 
		Cipher send_cipher; 
		char send_buffer[0x5B4];
		char recv_buffer[0x5B4];

		void Send(unsigned int len, void* data);
		void Send(unsigned int argc, ...);
	};
	STATIC_ASSERT(OFFSET_OF(MsgConn, recv_cipher) == 0x94);
	STATIC_ASSERT(OFFSET_OF(MsgConn::Protocol, ctos_table) == 0x1C);

	MsgConn* GetGameConn();
}