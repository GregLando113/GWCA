#include "CameraMgr.h"
#include "PatternScanner.h"

GWAPI::CameraMgr::CameraMgr(GWAPIMgr& api) : GWCAManager(api)
{
	PatternScanner scan(0x401000, 0x4FF000);
	DWORD scancamclass = scan.FindPattern("\x75\x0B\x51\xB9", "xxxx", 4);
	if (scancamclass) {
		cam_class_ = *(GW::Camera**)scancamclass;
	}
	else {
		cam_class_ = NULL;
	}
	DWORD scanprojmatrix = scan.FindPattern("\x89\x4D\xCC\x89\x45\xD4\x8B\x56\x08", "xxxxxxxxx", -4);
	if (scancamclass) {
		projection_matrix_ = (*(float**)scanprojmatrix) + 0x68;
	}
	else {
		projection_matrix_ = NULL;
	}

	patch_maxdist_addr = (LPVOID)scan.FindPattern("\x8B\x45\x08\x89\x41\x68\x5D", "xxxxxxx", 3);
	patch_camupdate_addr = (LPVOID)scan.FindPattern("\x89\x0E\x89\x56\x04\x89\x7E\x08", "xxxxxxxx", 0);
	patch_fog_addr = (LPVOID)scan.FindPattern("\x83\xE2\x01\x52\x6A\x1C\x50", "xxxxxxx", 2);
}

void GWAPI::CameraMgr::RestoreHooks()
{
	if (patch_maxdist_enable)
		patch_maxdist_enable = TooglePatch16(patch_maxdist_addr, 0x4189, false);
	if (patch_camupdate_enable)
		patch_camupdate_enable = TooglePatch16(patch_camupdate_addr, 0x0E89, false);
	if (patch_fog_enable)
		patch_fog_enable = SetFog(true);
}

GWAPI::Vector3D GWAPI::CameraMgr::ComputeCamPos(float dist)
{
	if (dist == 0) dist = GetCameraZoom();

	Vector3D newPos = GetLookAtTarget();

	float pitchX = sqrt(1.f - cam_class_->pitch*cam_class_->pitch);
	newPos.x -= dist * pitchX * cos(cam_class_->yaw);
	newPos.y -= dist * pitchX * sin(cam_class_->yaw);
	newPos.z -= dist * 0.95f * cam_class_->pitch; // 0.95 is the max pitch, not 1.0

	return newPos;
}

bool GWAPI::CameraMgr::TooglePatch16(LPVOID addr, WORD patchOrSize, bool enable)
{
	DWORD oldProt;
	VirtualProtect(addr, 2, PAGE_READWRITE, &oldProt);

	if (enable)
		*(WORD*)addr = ((BYTE)patchOrSize - 2) << 8 | 0xEB;
	else
		*(WORD*)addr = patchOrSize;

	VirtualProtect(addr, 2, oldProt, NULL);

	return enable;
}
