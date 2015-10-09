#include "CameraMgr.h"
#include "PatternScanner.h"

GWAPI::CameraMgr::CameraMgr(GWAPIMgr* obj) : parent_(obj)
{
	PatternScanner scan(0x401000, 0x4FF000);
	DWORD scancamclass = scan.FindPattern("\x75\x0B\x51\xB9", "xxxx", 4);
	if (scancamclass){
		cam_class_ = *(GW::Camera**)scancamclass;
	}
	else{
		cam_class_ = NULL;
	}
}
