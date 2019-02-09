#pragma once

namespace GW {
    struct Vec3f;
    struct Camera;

    namespace CameraMgr {
        GWCA_API void RestoreHooks();

        // ==== Camera ====
        GWCA_API Camera *GetCamera();

        // @Cleanup:
        // Remove this from headers or put it with the Camera struct

        // X,Y,Z of camera in game world.
        inline Vec3f GetCameraPosition() { return GetCamera()->position; }

        // LookAt Target (players head if not modified) of camera.
        inline Vec3f GetLookAtTarget() { return GetCamera()->look_at_target; }

        // camera yaw in radians from east
        inline float GetYaw() { return GetCamera()->yaw; }

        // camera pitch
        inline float GetPitch() { return GetCamera()->pitch; }

        // Distance of camera from lookat target (player)
        inline float GetCameraZoom() { return GetCamera()->distance; }

        // Horizonal Field of View
        inline float GetFieldOfView() { return GetCamera()->field_of_view; }

        // Set yaw, radian angle
        inline void SetYaw(float yaw) {
            Camera *cam = GetCamera();
            cam->yaw_to_go = yaw;
            cam->yaw = yaw;
        }

        // Set pitch (sin(angle))
        inline void SetPitch(float pitch) { GetCamera()->pitch_to_go = pitch; }

        inline void SetCameraPos(Vec3f newPos) {
            Camera *cam = GetCamera();
            cam->position.x = newPos.x;
            cam->position.y = newPos.y;
            cam->position.z = newPos.z;
        }

        inline void SetLookAtTarget(Vec3f newPos) {
            Camera *cam = GetCamera();
            cam->look_at_target.x = newPos.x;
            cam->look_at_target.y = newPos.y;
            cam->look_at_target.z = newPos.z;
        }

        void ForwardMovement(float amount, bool true_forward);
        void RotateMovement(float angle);
        void SideMovement(float amount);

        inline void VerticalMovement(float amount) {
            GetCamera()->look_at_target.z += amount;
        }

        // Change max zoom dist
        GWCA_API void SetMaxDist(float dist = 900.0f);

        GWCA_API void SetFieldOfView(float fov);

        // Manual computation of the position of the Camera. (As close as possible to the original)
        GWCA_API Vec3f ComputeCamPos(float dist = 0); // 2.f is the first person dist (const by gw)
        inline void UpdateCameraPos() {
            CameraMgr::SetCameraPos(ComputeCamPos());
        }

        // ==== Projection matrix ====
        // Returns (possible?) projection matrix of the game. Needs to be delved into.
        GWCA_API float *GetProjectionMatrix();


        // ==== Camera patches ====
        // Unlock camera & return the new state of it
        GWCA_API bool UnlockCam(bool flag);
        GWCA_API bool GetCameraUnlock();

        // Enable or Disable the fog & return the state of it
        GWCA_API bool SetFog(bool flag);
    };
}
