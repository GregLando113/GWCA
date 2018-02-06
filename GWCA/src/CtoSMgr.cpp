#include <GWCA\Managers\CtoSMgr.h>

#include <cstdio>

#include <GWCA\Utilities\Scanner.h>


void GW::CtoS::SendPacket(DWORD size, void* buffer) {
	typedef void(__fastcall *SendCtoGSPacket_t)(DWORD ctogsobj, DWORD size, void* packet);
	static SendCtoGSPacket_t gs_send_func = nullptr;
	static DWORD** GSObject = nullptr;
	if (!GSObject) {
		GSObject = (DWORD**)Scanner::Find("\x56\x33\xF6\x3B\xCE\x74\x0E\x56\x33\xD2", "xxxxxxxxxx", -4);
		printf("CtoGSObjectPtr = %p\n", GSObject);
	}
	if (!gs_send_func) {
		gs_send_func = (SendCtoGSPacket_t)Scanner::Find("\x55\x8B\xEC\x83\xEC\x2C\x53\x56\x57\x8B\xF9\x85", "xxxxxxxxxxxx", 0);
		printf("CtoGSSendFunction = %p\n", gs_send_func);
	}
	if (gs_send_func && GSObject) {
		gs_send_func(**GSObject, size, buffer);
	}
}

void GW::CtoS::SendPacket(DWORD size, ...) {
	DWORD* pak = &size + 1;
	SendPacket(size, pak);
}
