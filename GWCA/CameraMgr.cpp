#include "CameraMgr.h"
#include "PatternScanner.h"

GWAPI::CameraMgr::CameraMgr(GWAPIMgr& api) : GWCAManager(api)
{
	PatternScanner scan(0x401000, 0x4FF000);
	DWORD scancamclass = scan.FindPattern("\x75\x0B\x51\xB9", "xxxx", 4);
	if (scancamclass) {
		cam_class_ = *(GW::Camera**)scancamclass;
	} else {
		cam_class_ = NULL;
	}
	DWORD scanprojmatrix = scan.FindPattern("\x89\x4D\xCC\x89\x45\xD4\x8B\x56\x08", "xxxxxxxxx", -4);
	if (scancamclass) {
		projection_matrix_ = (*(float**)scanprojmatrix) + 0x68;
	} else {
		projection_matrix_ = NULL;
	}

}
