#include "stdafx.h"

#include <GWCA/Utilities/Export.h>
#include <GWCA/Utilities/Scanner.h>
#include <GWCA/Utilities/MemoryPatcher.h>

#include <GWCA/GameContainers/Vector.h>

#include <GWCA/GameEntities/Camera.h>

#include <GWCA/Managers/CameraMgr.h>

namespace {
    GW::MemoryPatcher *patch_max_dist;
    GW::MemoryPatcher *patch_cam_update;
    GW::MemoryPatcher *patch_fog;
    GW::MemoryPatcher *patch_fov;
}

namespace GW {
    Camera *CameraMgr::GetCamera() {
        static Camera *camera = nullptr;
        if (camera == nullptr) {
            uintptr_t scan_cam_class = Scanner::Find("\x75\x0B\x51\xB9", "xxxx", 4);
            if (scan_cam_class) {
                camera = *(Camera **)scan_cam_class;
            }
        }
        return camera;
    }

    float *CameraMgr::GetProjectionMatrix() {
        static float *proj_matrix = nullptr;
        if (proj_matrix == nullptr) {
            uintptr_t scan_proj_matrix = Scanner::Find(
                "\x89\x4D\xCC\x89\x45\xD4\x8B\x56\x08", "xxxxxxxxx", -4);
            if (scan_proj_matrix) {
                proj_matrix = (*(float **)scan_proj_matrix) + 0x68;
            }
        }
        return proj_matrix;
    }

    void CameraMgr::SetMaxDist(float dist) {
        if (patch_max_dist == nullptr) {
            uintptr_t patch_max_dist_addr = Scanner::Find(
                "\x8B\x45\x08\x89\x41\x68\x5D", "xxxxxxx", 3);
            patch_max_dist = new MemoryPatcher(patch_max_dist_addr, "\xEB\x01", 2);
            patch_max_dist->TooglePatch(true);
        }
        GetCamera()->max_distance2 = dist;
    }

    void CameraMgr::SetFieldOfView(float fov) {
        if (patch_fov == nullptr) {
            uintptr_t patch_fov_addr = Scanner::Find(
                "\x8B\x45\x0C\x89\x41\x04\xD9", "xxxxxxx", -0xC);
            patch_fov = new MemoryPatcher(patch_fov_addr, "\xC3", 1);
            patch_fov->TooglePatch(true);
        }
        GetCamera()->field_of_view = fov;
    }

    bool CameraMgr::UnlockCam(bool flag) {
        if (patch_cam_update == nullptr) {
            uintptr_t patch_cam_update_addr = Scanner::Find(
                "\x89\x0E\x89\x56\x04\x89\x7E\x08", "xxxxxxxx", 0);
            patch_cam_update = new MemoryPatcher(patch_cam_update_addr, "\xEB\x06", 2);
        }
        return patch_cam_update->TooglePatch(flag);
    }
    bool CameraMgr::GetCameraUnlock() {
        if (patch_cam_update) {
            return patch_cam_update->GetPatchState();
        } else {
            return false;
        }
    }

    bool CameraMgr::SetFog(bool flag) {
        if (patch_fog == nullptr) {
            uintptr_t patch_fog_addr = Scanner::Find(
                "\x83\xE2\x01\x52\x6A\x1C\x50", "xxxxxxx", 2);
            patch_fog = new MemoryPatcher(patch_fog_addr, "\x00", 1);
        }
        return patch_fog->TooglePatch(!flag);
    }

    void CameraMgr::RestoreHooks() {
        if (patch_max_dist) delete patch_max_dist;
        if (patch_cam_update) delete patch_cam_update;
        if (patch_fog) delete patch_fog;
        if (patch_fov) delete patch_fov;
    }

    void CameraMgr::ForwardMovement(float amount, bool true_forward) {
        if (amount == 0.f) return;
        Camera *cam = GetCamera();
        if (true_forward) {
            float pitchX = sqrtf(1.f - cam->pitch * cam->pitch);
            cam->look_at_target.x += amount * pitchX * cosf(cam->yaw);
            cam->look_at_target.y += amount * pitchX * sinf(cam->yaw);
            cam->look_at_target.z += amount * cam->pitch;
        } else {
            cam->look_at_target.x += amount * cosf(cam->yaw);
            cam->look_at_target.y += amount * sinf(cam->yaw);
        }
    }

    void CameraMgr::SideMovement(float amount) {
        if (amount == 0.f) return;
        Camera *cam = GetCamera();
        cam->look_at_target.x += amount * -sinf(cam->yaw);
        cam->look_at_target.y += amount *  cosf(cam->yaw);
    }

    void CameraMgr::RotateMovement(float angle) {
        if (angle == 0.f) return;
        // rotation with fixed z (vertical axe)
        Camera *cam = GetCamera();
        float pos_x = cam->position.x;
        float pos_y = cam->position.y;
        float px = cam->look_at_target.x - pos_x;
        float py = cam->look_at_target.y - pos_y;

        Vec3f newPos;
        newPos.x = pos_x + (cosf(angle) * px - sinf(angle) * py);
        newPos.y = pos_y + (sinf(angle) * px + cosf(angle) * py);
        newPos.z = cam->look_at_target.z;

        SetYaw(cam->yaw + angle);
        GetCamera()->look_at_target = newPos;
    }

    Vec3f CameraMgr::ComputeCamPos(float dist) {
        if (dist == 0) dist = GetCameraZoom();

        Vec3f newPos = GetLookAtTarget();

        float pitchX = sqrtf(1.f - GetCamera()->pitch*GetCamera()->pitch);
        newPos.x -= dist * pitchX * cosf(GetCamera()->yaw);
        newPos.y -= dist * pitchX * sinf(GetCamera()->yaw);
        newPos.z -= dist * 0.95f * GetCamera()->pitch; // 0.95 is the max pitch, not 1.0

        return newPos;
    }
} // namespace GW
