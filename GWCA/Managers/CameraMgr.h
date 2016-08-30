#pragma once

#include "GWCAManager.h"

#include "..\Structures\GameEntities\Camera.h"
#include "..\Utilities\MemoryPatcher.h"

namespace GWCA {

	class CameraMgr : public GWCAManager<CameraMgr> {
		friend class GWCAManager<CameraMgr>;

	public:
		// X,Y,Z of camera in game world.
		inline Vector3f GetCameraPosition(){ return cam_class_->camerapos; }

		// LookAt Target (players head if not modified) of camera.
		inline Vector3f GetLookAtTarget(){ return cam_class_->LookAtTarget; }

		// camera yaw in radians from east
		inline float GetYaw() { return cam_class_->yaw; }

		// camera pitch
		inline float GetPitch() { return cam_class_->pitch; }

		// Distance of camera from lookat target (player)
		inline float GetCameraZoom() { return cam_class_->distance; }

		// Horizonal Field of View
		inline float GetFieldOfView() { return cam_class_->fieldofview; }

		// Returns (possible?) projection matrix of the game. Needs to be delved into.
		inline float* GetProjectionMatrix() { return projection_matrix_; }

		// Set yaw, radian angle
		inline void SetYaw(float yaw) { cam_class_->yaw_togo = yaw; }

		// Set pitch (sin(angle))
		inline void SetPitch(float pitch) { cam_class_->pitch_togo = pitch; }

		// Manual computation of the position of the Camera. (As close as possible to the original)
		void UpdateCameraPos() {
			SetCameraPos(ComputeCamPos());
		}
		Vector3f ComputeCamPos(float dist = 0); // 2.f is the first person dist (const by gw)
		void SetCameraPos(Vector3f const& newPos) {
			cam_class_->camerapos.x = newPos.x;
			cam_class_->camerapos.y = newPos.y;
			cam_class_->camerapos.z = newPos.z;
		}

		// Change max zoom dist
		void SetMaxDist(float dist) { cam_class_->maxdistance2 = dist; }

		// Unlock camera & return the new state of it
		bool UnlockCam(bool flag) { return patch_camupdate->TooglePatch(flag); }
		bool GetCameraUnlock() { return patch_camupdate->GetPatchState(); }

		void SetLookAtTarget(Vector3f const& newPos) {
			cam_class_->LookAtTarget.x = newPos.x;
			cam_class_->LookAtTarget.y = newPos.y;
			cam_class_->LookAtTarget.z = newPos.z;
		}

		void ForwardMovement(float amount) {
			float pitchX = sqrt(1.f - cam_class_->pitch*cam_class_->pitch);
			cam_class_->LookAtTarget.x += amount * pitchX * cos(cam_class_->yaw);
			cam_class_->LookAtTarget.y += amount * pitchX * sin(cam_class_->yaw);
			cam_class_->LookAtTarget.z += amount * cam_class_->pitch;
		}
		void SideMovement(float amount) {
			cam_class_->LookAtTarget.x += amount * -sin(cam_class_->yaw);
			cam_class_->LookAtTarget.y += amount * cos(cam_class_->yaw);
		}
		void VerticalMovement(float amount) {
			cam_class_->LookAtTarget.z += amount;
		}

		// Enable or Disable the fog & return the state of it
		bool SetFog(bool flag) { return patch_fog->TooglePatch(!flag); }

		void SetFieldOfView(float fov) { cam_class_->fieldofview = fov; };

	private:

		CameraMgr();
		void RestoreHooks() override;

		GW::Camera* cam_class_;
		float* projection_matrix_;

		MemoryPatcher *patch_maxdist;
		MemoryPatcher *patch_camupdate;
		MemoryPatcher *patch_fog;
		MemoryPatcher *patch_fov;
	};

}