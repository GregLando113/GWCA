#pragma once

#include "GWAPIMgr.h"

namespace GWAPI {

	class CameraMgr {
	public:
		
		// X,Y,Z of camera in game world.
		inline GW::Vector3D GetCameraPosition(){ return cam_class_->camerapos; }

		// LookAt Target (players head if not modified) of camera.
		inline GW::Vector3D GetLookAtTarget(){ return cam_class_->LookAtTarget; }

		// Yaw and Pitch in X and Y fields respectively.
		inline GW::Vector3D GetViewAngles(){ return GW::Vector3D{ cam_class_->yaw,cam_class_->pitch}; }

		// Distance of camera from lookat target (player)
		inline float GetCameraZoom() { return cam_class_->distance; }

		// Horizonal Field of View
		inline float GetFieldOfView() { return cam_class_->fieldofview; }

	private:
		friend class GWAPIMgr;
		CameraMgr(GWAPIMgr* obj);
		GWAPIMgr* const parent_;
		GW::Camera* cam_class_;
	};

}