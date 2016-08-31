#pragma once
#include <Windows.h>
#include "Position.h"

namespace GWCA {
	namespace GW {

		struct Camera {
			DWORD lookAtAgentID;
			DWORD unk1;
			float unk2[2];
			float maxdistance; // Max distance camera can go, usually constant @ 750 unless max zoom changed
			float unk3;
			float yaw; // left/right camera angle, radians w/ origin @ east
			float pitch; // up/down camera angle, range of [-1,1]
			float distance; // current distance from players head.
			DWORD unk4[4];
			float yaw_rightclick; // Only changes when you move camera using right click, keyboard movement does not effect.
			float yaw_rightclick2; // ^
			float pitch_rightclick; // ^
			float distance2;
			float accelerationconstant; // According to http://www.gamerevision.com/showthread.php?217-Guild-Wars-WorldToSreen-by-Cronos&p=20003&viewfull=1#post20003
			float timesincelastkeyboardrotation; // In seconds it seems.
			float timesincelastmouserotation;
			float timesincelastmousemove;
			float timesincelastagentselection;
			float timeinthemap;
			float timeinthedistrict;
			float yaw_togo;
			float pitch_togo;
			float dist_togo;
			float maxdistance2;
			float unk5[2];
			Vector3f camerapos;
			Vector3f camerapos_togo;
			Vector3f campos_inverted;
			Vector3f campos_inverted_togo;
			Vector3f LookAtTarget;
			Vector3f LookAt_togo;
			float fieldofview;
			float fieldofview2;
			// ...
		};
	}
}
