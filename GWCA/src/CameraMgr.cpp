#include <assert.h>
#include <stdint.h>

#include <Windows.h>

#include <GWCA/Utilities/Export.h>
#include <GWCA/Utilities/Scanner.h>
#include <GWCA/Utilities/MemoryPatcher.h>

// @Cleanup: Fix this Position & StoC includes
#include <GWCA/GameEntities/Position.h>

#include <GWCA/GameEntities/Camera.h>

#include <GWCA/Managers/CameraMgr.h>

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
        uintptr_t patch_maxdist_addr = Scanner::Find("\x8B\x45\x08\x89\x41\x68\x5D", "xxxxxxx", 3);
        patch_maxdist = new MemoryPatcher(patch_maxdist_addr, "\xEB\x01", 2);
        patch_maxdist->TooglePatch(true);
    }
    GetCamera()->max_distance2 = dist;
}

void GW::CameraMgr::SetFieldOfView(float fov) {
    if (patch_fov == nullptr) {
        uintptr_t patch_fov_addr = Scanner::Find("\x8B\x45\x0C\x89\x41\x04\xD9", "xxxxxxx", -0xC);
        patch_fov = new MemoryPatcher(patch_fov_addr, "\xC3", 1);
        patch_fov->TooglePatch(true);
    }
    GetCamera()->field_of_view = fov;
}

bool GW::CameraMgr::UnlockCam(bool flag) {
    if (patch_camupdate == nullptr) {
        uintptr_t patch_camupdate_addr = Scanner::Find("\x89\x0E\x89\x56\x04\x89\x7E\x08", "xxxxxxxx", 0);
        patch_camupdate = new MemoryPatcher(patch_camupdate_addr, "\xEB\x06", 2);
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
        uintptr_t patch_fog_addr = Scanner::Find("\x83\xE2\x01\x52\x6A\x1C\x50", "xxxxxxx", 2);
        patch_fog = new MemoryPatcher(patch_fog_addr, "\x00", 1);
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
        float pitchX = sqrt(1.f - cam->pitch * cam->pitch);
        cam->look_at_target.x += amount * pitchX * cos(cam->yaw);
        cam->look_at_target.y += amount * pitchX * sin(cam->yaw);
        cam->look_at_target.z += amount * cam->pitch;
    } else {
        cam->look_at_target.x += amount * cos(cam->yaw);
        cam->look_at_target.y += amount * sin(cam->yaw);
    }
}

void GW::CameraMgr::SideMovement(float amount) {
    if (amount == 0.f) return;
    Camera *cam = GetCamera();
    cam->look_at_target.x += amount * -sin(cam->yaw);
    cam->look_at_target.y += amount *  cos(cam->yaw);
}

void GW::CameraMgr::RotateMovement(float angle) {
    if (angle == 0.f) return;
    // rotation with fixed z (vertical axe)
    Camera *cam = GetCamera();
    float pos_x = cam->position.x;
    float pos_y = cam->position.y;
    float px = cam->look_at_target.x - pos_x;
    float py = cam->look_at_target.y - pos_y;

    Vec3f newPos;
    newPos.x = pos_x + (cos(angle) * px - sin(angle) * py);
    newPos.y = pos_y + (sin(angle) * px + cos(angle) * py);
    newPos.z = cam->look_at_target.z;

    SetYaw(cam->yaw + angle);
    GetCamera()->look_at_target = newPos;
}

GW::Vector3f GW::CameraMgr::ComputeCamPos(float dist) {
    if (dist == 0) dist = GetCameraZoom();

    Vector3f newPos = GetLookAtTarget();

    float pitchX = sqrt(1.f - GetCamera()->pitch*GetCamera()->pitch);
    newPos.x -= dist * pitchX * cos(GetCamera()->yaw);
    newPos.y -= dist * pitchX * sin(GetCamera()->yaw);
    newPos.z -= dist * 0.95f * GetCamera()->pitch; // 0.95 is the max pitch, not 1.0

    return newPos;
}
