#pragma once

#include <GWCA/Utilities/Export.h>

namespace GW {
    struct Vec3f;
    struct Camera;
    struct Module;

    extern Module CameraModule;

    namespace CameraMgr {
        // ==== Camera ====
        GWCA_API Camera *GetCamera();

        GWCA_API void ForwardMovement(float amount, bool true_forward);
        GWCA_API void VerticalMovement(float amount);
        GWCA_API void RotateMovement(float angle);
        GWCA_API void SideMovement(float amount);

        // Change max zoom dist
        GWCA_API void SetMaxDist(float dist = 900.0f);

        GWCA_API void SetFieldOfView(float fov);

        // Manual computation of the position of the Camera. (As close as possible to the original)
        GWCA_API Vec3f ComputeCamPos(float dist = 0); // 2.f is the first person dist (const by gw)
        GWCA_API void UpdateCameraPos();

        GWCA_API float GetFieldOfView();
        GWCA_API float GetYaw();
        GWCA_API float GetCurrentYaw();

        // ==== Camera patches ====
        // Unlock camera & return the new state of it
        GWCA_API bool UnlockCam(bool flag);
        GWCA_API bool GetCameraUnlock();

        // Enable or Disable the fog & return the state of it
        GWCA_API bool SetFog(bool flag);
    };
}
