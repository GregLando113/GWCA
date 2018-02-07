#include <GWCA\Managers\CameraMgr.h>

#include <GWCA\Utilities\Scanner.h>
#include <GWCA\Utilities\MemoryPatcher.h>

namespace {
	GW::MemoryPatcher *patch_maxdist;
	GW::MemoryPatcher *patch_camupdate;
	GW::MemoryPatcher *patch_fog;
	GW::MemoryPatcher *patch_fov;
}

GW::Camera* GW::CameraMgr::GetCamera() {
	static GW::Camera* camera = nullptr;
	if (camera == nullptr) {
		DWORD scancamclass = Scanner::Find("\x75\x0B\x51\xB9", "xxxx", 4);
		if (scancamclass) {
			camera = *(GW::Camera**)scancamclass;
		}
	}
	return camera;
}

float* GW::CameraMgr::GetProjectionMatrix() {
	static float* proj_matrix = nullptr;
	if (proj_matrix == nullptr) {
		DWORD scanprojmatrix = Scanner::Find("\x89\x4D\xCC\x89\x45\xD4\x8B\x56\x08", "xxxxxxxxx", -4);
		if (scanprojmatrix) {
			proj_matrix = (*(float**)scanprojmatrix) + 0x68;
		}
	}
	return proj_matrix;
}

void GW::CameraMgr::SetMaxDist(float dist) {
	if (patch_maxdist == nullptr) {
		LPVOID patch_maxdist_addr = (LPVOID)Scanner::Find("\x8B\x45\x08\x89\x41\x68\x5D", "xxxxxxx", 3);
		patch_maxdist = new MemoryPatcher(patch_maxdist_addr, (BYTE*)"\xEB\x01", 2);
		patch_maxdist->TooglePatch(true);
	}
	GetCamera()->maxdistance2 = dist;
}

void GW::CameraMgr::SetFieldOfView(float fov) {
	if (patch_fog == nullptr) {
		LPVOID patch_fov_addr = (LPVOID)Scanner::Find("\x8B\x45\x0C\x89\x41\x04\xD9", "xxxxxxx", -0xC);
		patch_fov = new MemoryPatcher(patch_fov_addr, (BYTE*)"\xC3", 1);
		patch_fov->TooglePatch(true);
	}
	GetCamera()->fieldofview = fov;
}

bool GW::CameraMgr::UnlockCam(bool flag) {
	if (patch_camupdate == nullptr) {
		LPVOID patch_camupdate_addr = (LPVOID)Scanner::Find("\x89\x0E\x89\x56\x04\x89\x7E\x08", "xxxxxxxx", 0);
		patch_camupdate = new MemoryPatcher(patch_camupdate_addr, (BYTE*)"\xEB\x06", 2);
	}
	return patch_camupdate->TooglePatch(flag);
}
bool GW::CameraMgr::GetCameraUnlock() {
	if (patch_camupdate) {
		return patch_camupdate->GetPatchState();
	} else {
		return false;
	}
}

bool GW::CameraMgr::SetFog(bool flag) {
	if (patch_fog == nullptr) {
		LPVOID patch_fog_addr = (LPVOID)Scanner::Find("\x83\xE2\x01\x52\x6A\x1C\x50", "xxxxxxx", 2);
		patch_fog = new MemoryPatcher(patch_fog_addr, (BYTE*)"\x00", 1);
	}
	return patch_fog->TooglePatch(!flag);
}

void GW::CameraMgr::RestoreHooks() {
	if (patch_maxdist) delete patch_maxdist;
	if (patch_camupdate) delete patch_camupdate;
	if (patch_fog) delete patch_fog;
	if (patch_fov) delete patch_fov;
}

void GW::CameraMgr::ForwardMovement(float amount, bool true_forward) {
	if (amount == 0.f) return;
	Camera *cam = GetCamera();
	if (true_forward) {
		float pitchX = sqrt(1.f - cam->Pitch * cam->Pitch);
		cam->LookAtTarget.x += amount * pitchX * cos(cam->Yaw);
		cam->LookAtTarget.y += amount * pitchX * sin(cam->Yaw);
		cam->LookAtTarget.z += amount * cam->Pitch;
	} else {
		cam->LookAtTarget.x += amount * cos(cam->Yaw);
		cam->LookAtTarget.y += amount * sin(cam->Yaw);
	}
}

void GW::CameraMgr::SideMovement(float amount) {
	if (amount == 0.f) return;
	Camera *cam = GetCamera();
	cam->LookAtTarget.x += amount * -sin(cam->Yaw);
	cam->LookAtTarget.y += amount *  cos(cam->Yaw);
}

void GW::CameraMgr::RotateMovement(float angle) {
	if (angle == 0.f) return;
	// rotation with fixed z (vertical axe)
	Camera *cam = GetCamera();
	float pos_x = cam->Position.x;
	float pos_y = cam->Position.y;
	float px = cam->LookAtTarget.x - pos_x;
	float py = cam->LookAtTarget.y - pos_y;

	Vec3f newPos;
	newPos.x = pos_x + (cos(angle) * px - sin(angle) * py);
	newPos.y = pos_y + (sin(angle) * px + cos(angle) * py);
	newPos.z = cam->LookAtTarget.z;

	SetYaw(cam->Yaw + angle);
	GetCamera()->LookAtTarget = newPos;
}

GW::Vector3f GW::CameraMgr::ComputeCamPos(float dist) {
	if (dist == 0) dist = GetCameraZoom();

	Vector3f newPos = GetLookAtTarget();

	float pitchX = sqrt(1.f - GetCamera()->Pitch*GetCamera()->Pitch);
	newPos.x -= dist * pitchX * cos(GetCamera()->Yaw);
	newPos.y -= dist * pitchX * sin(GetCamera()->Yaw);
	newPos.z -= dist * 0.95f * GetCamera()->Pitch; // 0.95 is the max pitch, not 1.0

	return newPos;
}
