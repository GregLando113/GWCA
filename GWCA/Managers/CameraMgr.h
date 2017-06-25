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
		inline void SetYaw(float yaw) { GetCamera()->yaw_togo = yaw; }

		// Set pitch (sin(angle))
		inline void SetPitch(float pitch) { GetCamera()->pitch_togo = pitch; }

		inline void SetCameraPos(Vec3f const& newPos) {
			GetCamera()->Position.x = newPos.x;
			GetCamera()->Position.y = newPos.y;
			GetCamera()->Position.z = newPos.z;
		}

		inline void SetLookAtTarget(Vec3f const& newPos) {
			GetCamera()->LookAtTarget.x = newPos.x;
			GetCamera()->LookAtTarget.y = newPos.y;
			GetCamera()->LookAtTarget.z = newPos.z;
		}

		inline void ForwardMovement(float amount) {
			float pitchX = sqrt(1.f - GetCamera()->Pitch*GetCamera()->Pitch);
			GetCamera()->LookAtTarget.x += amount * pitchX * cos(GetCamera()->Yaw);
			GetCamera()->LookAtTarget.y += amount * pitchX * sin(GetCamera()->Yaw);
			GetCamera()->LookAtTarget.z += amount * GetCamera()->Pitch;
		}
		inline void SideMovement(float amount) {
			GetCamera()->LookAtTarget.x += amount * -sin(GetCamera()->Yaw);
			GetCamera()->LookAtTarget.y += amount * cos(GetCamera()->Yaw);
		}
		inline void VerticalMovement(float amount) {
			GetCamera()->LookAtTarget.z += amount;
		}

		// Change max zoom dist
		void SetMaxDist(float dist = 750.0f);

		void SetFieldOfView(float fov);

		// Manual computation of the position of the Camera. (As close as possible to the original)
		Vec3f ComputeCamPos(float dist = 0); // 2.f is the first person dist (const by gw)
		inline void UpdateCameraPos() {
			SetCameraPos(ComputeCamPos());
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
