#include "stdafx.h"

#include <GWCA/Utilities/Export.h>
#include <GWCA/Utilities/Macros.h>
#include <GWCA/Utilities/Scanner.h>
#include <GWCA/Utilities/MemoryPatcher.h>

#include <GWCA/GameContainers/Vector.h>

#include <GWCA/GameEntities/Camera.h>

#include <GWCA/Managers/Module.h>
#include <GWCA/Managers/CameraMgr.h>

namespace {
    using namespace GW;

    MemoryPatcher *patch_max_dist;
    MemoryPatcher *patch_cam_update;
    MemoryPatcher *patch_fog;
    MemoryPatcher *patch_fov;

    uintptr_t patch_fog_addr;
    uintptr_t patch_fov_addr;
    uintptr_t patch_max_dist_addr;
    uintptr_t patch_cam_update_addr;

    uintptr_t scan_cam_class;
    uintptr_t scan_proj_matrix_addr;

    void Init() {
        patch_fog_addr = Scanner::Find(
            "\x83\xE2\x01\x52\x6A\x1C\x50", "xxxxxxx", 2);
        printf("[SCAN] patch_fog_addr = %p\n", (void *)patch_fog_addr);

        patch_fov_addr = Scanner::Find(
            "\x8B\x45\x0C\x89\x41\x04\xD9", "xxxxxxx", -0xC);
        printf("[SCAN] patch_fov_addr = %p\n", (void *)patch_fov_addr);

        patch_max_dist_addr = Scanner::Find(
            "\x8B\x45\x08\x89\x41\x68\x5D", "xxxxxxx", 3);
        printf("[SCAN] patch_max_dist_addr = %p\n", (void *)patch_max_dist_addr);

        patch_cam_update_addr = Scanner::Find(
            "\x89\x0E\x89\x56\x04\x89\x7E\x08", "xxxxxxxx", 0);
        printf("[SCAN] patch_cam_update_addr = %p\n", (void *)patch_cam_update_addr);

        {
            uintptr_t address = Scanner::Find("\x75\x0B\x51\xB9", "xxxx", 4);
            printf("[SCAN] scan_cam_class = %p\n", (void *)address);
            if (Verify(address))
                scan_cam_class = *(uintptr_t *)address;
        }

        {
            uintptr_t address = Scanner::Find(
                "\x89\x4D\xCC\x89\x45\xD4\x8B\x56\x08", "xxxxxxxxx", -4);
            printf("[SCAN] scan_proj_matrix_addr = %p\n", (void *)address);
            if (Verify(address))
                scan_proj_matrix_addr = *(uintptr_t *)address;
        }
    }

    void CreateHooks() {
        if (Verify(patch_max_dist)) {
            patch_max_dist = new MemoryPatcher(patch_max_dist_addr, "\xEB\x01", 2);
            patch_max_dist->TooglePatch(true);
        }
        if (Verify(patch_fov)) {
            patch_fov = new MemoryPatcher(patch_fov_addr, "\xC3", 1);
            patch_max_dist->TooglePatch(true);
        }
        if (Verify(patch_cam_update)) {
            patch_cam_update = new MemoryPatcher(patch_cam_update_addr, "\xEB\x06", 2);
            patch_max_dist->TooglePatch(true);
        }
        if (Verify(patch_fog)) {
            patch_fog = new MemoryPatcher(patch_fog_addr, "\x00", 1);
            patch_max_dist->TooglePatch(true);
        }
    }

    void RemoveHooks() {
        if (patch_max_dist)
            delete patch_max_dist;
        if (patch_cam_update)
            delete patch_cam_update;
        if (patch_fog)
            delete patch_fog;
        if (patch_fov)
            delete patch_fov;
    }
}

namespace GW {
    
    Module CameraModule = {
        "CameraModule",     // name
        NULL,               // param
        ::Init,             // init_module
        NULL,               // exit_module
        ::CreateHooks,      // exit_module
        ::RemoveHooks,      // remove_hooks
    };

    Camera *CameraMgr::GetCamera() {
        Camera *camera = (Camera *)scan_cam_class;
        return camera;
    }

    float *CameraMgr::GetProjectionMatrix() {
        float *proj_matrix = (float *)(scan_proj_matrix_addr + 0x1A0);
        return proj_matrix;
    }

    void CameraMgr::SetMaxDist(float dist) {
        GetCamera()->max_distance2 = dist;
    }

    void CameraMgr::SetFieldOfView(float fov) {
        GetCamera()->field_of_view = fov;
    }

    bool CameraMgr::UnlockCam(bool flag) {
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
        return patch_fog->TooglePatch(!flag);
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

    void CameraMgr::VerticalMovement(float amount) {
        Camera *cam = GetCamera();
        cam->look_at_target.z += amount;
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

        cam->SetYaw(cam->yaw + angle);
        GetCamera()->look_at_target = newPos;
    }

    Vec3f CameraMgr::ComputeCamPos(float dist) {
        Camera *cam = GetCamera();
        if (dist == 0) dist = cam->GetCameraZoom();

        Vec3f newPos = cam->GetLookAtTarget();

        float pitchX = sqrtf(1.f - GetCamera()->pitch*GetCamera()->pitch);
        newPos.x -= dist * pitchX * cosf(GetCamera()->yaw);
        newPos.y -= dist * pitchX * sinf(GetCamera()->yaw);
        newPos.z -= dist * 0.95f * GetCamera()->pitch; // 0.95 is the max pitch, not 1.0

        return newPos;
    }

    void CameraMgr::UpdateCameraPos() {
        Camera *cam = GetCamera();
        cam->SetCameraPos(ComputeCamPos());
    }
} // namespace GW
