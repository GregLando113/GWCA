#pragma once

#include "GWCAManager.h"
#include "GWStructures.h"

namespace GWAPI {

	class CameraMgr : public GWCAManager {
		friend class GWAPIMgr;

	public:
		
		// X,Y,Z of camera in game world.
		inline GW::Vector3D GetCameraPosition(){ return cam_class_->camerapos; }

		// LookAt Target (players head if not modified) of camera.
		inline GW::Vector3D GetLookAtTarget(){ return cam_class_->LookAtTarget; }

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

	private:

		CameraMgr(GWAPIMgr& obj);

		GW::Camera* cam_class_;
		float* projection_matrix_;
	};

}