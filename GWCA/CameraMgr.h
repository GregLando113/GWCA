#pragma once

#include "GWCAManager.h"
#include "GWStructures.h"

namespace GWAPI {

	class CameraMgr : public GWCAManager {
		friend class GWAPIMgr;

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
		Vector3f ComputeCamPos(float dist = 0);

		// Change max zoom dist
		inline bool SetMaxDist(float dist) {
			if (!patch_maxdist_enable)
				patch_maxdist_enable = TooglePatch16(patch_maxdist_addr, 3, true);
			cam_class_->maxdistance2 = dist;
			return patch_maxdist_enable;
		}

		// Unlock camera
		inline bool UnlockCam(bool enable) {
			patch_camupdate_enable = TooglePatch16(patch_camupdate_addr, 8, enable);
			return patch_camupdate_enable;
		}

		// Enable or Disable the fog & return the state of it
		inline bool SetFog(bool enable) {
			DWORD oldProt;
			VirtualProtect(patch_fog_addr, 1, PAGE_READWRITE, &oldProt);

			*(BYTE*)patch_fog_addr = enable ? 1 : 0;
			patch_fog_enable = !enable;

			VirtualProtect(patch_fog_addr, 1, oldProt, NULL);
			return enable;
		}

	private:

		CameraMgr(GWAPIMgr& obj);
		void RestoreHooks() override;

		GW::Camera* cam_class_;
		float* projection_matrix_;

		LPVOID patch_maxdist_addr;
		LPVOID patch_camupdate_addr;
		LPVOID patch_fog_addr;
		bool patch_maxdist_enable = false;
		bool patch_camupdate_enable = false;
		bool patch_fog_enable = false;

		bool TooglePatch16(LPVOID addr, WORD patchOrSize, bool enable);
	};

}