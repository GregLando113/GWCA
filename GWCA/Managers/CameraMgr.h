#pragma once

#include <GWCA\GameEntities\Camera.h>

namespace GW {

	namespace CameraMgr {
		void RestoreHooks();

		// ==== Camera ====
		GW::Camera* GetCamera();

		// X,Y,Z of camera in game world.
		inline Vec3f GetCameraPosition() { return GetCamera()->Position; }

		// LookAt Target (players head if not modified) of camera.
		inline Vec3f GetLookAtTarget() { return GetCamera()->LookAtTarget; }

		// camera yaw in radians from east
		inline float GetYaw() { return GetCamera()->Yaw; }

		// camera pitch
		inline float GetPitch() { return GetCamera()->Pitch; }

		// Distance of camera from lookat target (player)
		inline float GetCameraZoom() { return GetCamera()->Distance; }

		// Horizonal Field of View
		inline float GetFieldOfView() { return GetCamera()->fieldofview; }

		// Set yaw, radian angle
		inline void SetYaw(float yaw) {
			Camera *cam = GetCamera();
			cam->yaw_togo = yaw;
			cam->Yaw = yaw;
		}

		// Set pitch (sin(angle))
		inline void SetPitch(float pitch) { GetCamera()->pitch_togo = pitch; }

		inline void SetCameraPos(Vec3f newPos) {
			Camera *cam = GetCamera();
			cam->Position.x = newPos.x;
			cam->Position.y = newPos.y;
			cam->Position.z = newPos.z;
		}

		inline void SetLookAtTarget(Vec3f const& newPos) {
			Camera *cam = GetCamera();
			cam->LookAtTarget.x = newPos.x;
			cam->LookAtTarget.y = newPos.y;
			cam->LookAtTarget.z = newPos.z;
		}

		void ForwardMovement(float amount, bool true_forward);
		void RotateMovement(float angle);
		void SideMovement(float amount);

		inline void VerticalMovement(float amount) {
			GetCamera()->LookAtTarget.z += amount;
		}

		// Change max zoom dist
		void SetMaxDist(float dist = 750.0f);

		void SetFieldOfView(float fov);

		// Manual computation of the position of the Camera. (As close as possible to the original)
		Vec3f ComputeCamPos(float dist = 0); // 2.f is the first person dist (const by gw)
		inline void UpdateCameraPos() {
			CameraMgr::SetCameraPos(ComputeCamPos());
		}

		// ==== Projection matrix ====
		// Returns (possible?) projection matrix of the game. Needs to be delved into.
		float* GetProjectionMatrix();


		// ==== Camera patches ====
		// Unlock camera & return the new state of it
		bool UnlockCam(bool flag);
		bool GetCameraUnlock();

		// Enable or Disable the fog & return the state of it
		bool SetFog(bool flag);
	};
}
