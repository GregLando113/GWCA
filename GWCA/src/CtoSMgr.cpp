#include <GWCA\Managers\CtoSMgr.h>

#include <GWCA\Managers\MemoryMgr.h>

typedef void __fastcall SendCtoGSPacket_t(DWORD ctogsobj, DWORD size, void* packet);
static SendCtoGSPacket_t* gs_send_function_ = NULL;

GW::CtoSMgr::CtoSMgr() {
	gs_send_function_ = (SendCtoGSPacket_t*)MemoryMgr::CtoGSSendFunction;
}

void GW::CtoSMgr::SendPacket(DWORD size, ...) {
	DWORD* pak = &size + 1;
	DWORD gs = MemoryMgr::GetGSObject();
	if(gs)
		gs_send_function_(gs, size, pak);
}

void GW::CtoSMgr::SendPacket(DWORD size, void* buffer) {
	DWORD gs = MemoryMgr::GetGSObject();
	if(gs)
		gs_send_function_(gs, size, buffer);
}
