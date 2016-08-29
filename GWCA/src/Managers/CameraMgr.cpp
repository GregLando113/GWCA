#include "CameraMgr.h"
#include "PatternScanner.h"

GWCA::CameraMgr::CameraMgr() {
	PatternScanner scan("Gw.exe");
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

	LPVOID patch_maxdist_addr = (LPVOID)scan.FindPattern("\x8B\x45\x08\x89\x41\x68\x5D", "xxxxxxx", 3);
	patch_maxdist = new MemoryPatcher(patch_maxdist_addr, (BYTE*)"\xEB\x01", 2);
	patch_maxdist->TooglePatch(true);

	LPVOID patch_camupdate_addr = (LPVOID)scan.FindPattern("\x89\x0E\x89\x56\x04\x89\x7E\x08", "xxxxxxxx", 0);
	patch_camupdate = new MemoryPatcher(patch_camupdate_addr, (BYTE*)"\xEB\x06", 2);

	LPVOID patch_fog_addr = (LPVOID)scan.FindPattern("\x83\xE2\x01\x52\x6A\x1C\x50", "xxxxxxx", 2);
	patch_fog = new MemoryPatcher(patch_fog_addr, (BYTE*)"\x00", 1);

	LPVOID patch_fov_addr = (LPVOID)scan.FindPattern("\x8B\x45\x0C\x89\x41\x04\xD9", "xxxxxxx", -0xC);
	patch_fov = new MemoryPatcher(patch_fov_addr, (BYTE*)"\xC3", 1);
	patch_fov->TooglePatch(true);
}

void GWCA::CameraMgr::RestoreHooks() {
	delete patch_maxdist;
	delete patch_camupdate;
	delete patch_fog;
	delete patch_fov;
}

GWCA::Vector3f GWCA::CameraMgr::ComputeCamPos(float dist) {
	if (dist == 0) dist = GetCameraZoom();

	Vector3f newPos = GetLookAtTarget();

	float pitchX = sqrt(1.f - cam_class_->pitch*cam_class_->pitch);
	newPos.x -= dist * pitchX * cos(cam_class_->yaw);
	newPos.y -= dist * pitchX * sin(cam_class_->yaw);
	newPos.z -= dist * 0.95f * cam_class_->pitch; // 0.95 is the max pitch, not 1.0

	return newPos;
}
