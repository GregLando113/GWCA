#ifndef _ENTITIE_CAMERA_INC
#define _ENTITIE_CAMERA_INC

#include <Windows.h>
#include <GWCA\GameEntities\Position.h>

namespace GW {
    using Vec3f = Vector3f;

    struct Camera {
        /* +h0000 */ DWORD LookAtAgentID;
        /* +h0004 */ DWORD h0004;
        /* +h0008 */ float h0008;
        /* +h000C */ float h000C;
        /* +h0010 */ float MaxDistance;
        /* +h0014 */ float h0014;
        /* +h0018 */ float Yaw; // left/right camera angle, radians w/ origin @ east
        /* +h001C */ float Pitch; // up/down camera angle, range of [-1,1]
        /* +h0020 */ float Distance; // current distance from players head.
        /* +h0024 */ DWORD h0024[4];
        /* +h0034 */ float yaw_rightclick;
        /* +h0038 */ float yaw_rightclick2;
        /* +h003C */ float pitch_rightclick;
        /* +h0040 */ float distance2;
        /* +h0044 */ float accelerationconstant;
        /* +h0048 */ float timesincelastkeyboardrotation; // In seconds it seems.
        /* +h004C */ float timesincelastmouserotation;
        /* +h0050 */ float timesincelastmousemove;
        /* +h0054 */ float timesincelastagentselection;
        /* +h0058 */ float timeinthemap;
        /* +h005C */ float timeinthedistrict;
        /* +h0060 */ float yaw_togo;
        /* +h0064 */ float pitch_togo;
        /* +h0068 */ float dist_togo;
        /* +h006C */ float maxdistance2;
        /* +h0070 */ float h0070[2];
        /* +h0078 */ Vec3f Position;
        /* +h0084 */ Vec3f camerapos_togo;
        /* +h0090 */ Vec3f campos_inverted;
        /* +h009C */ Vec3f campos_inverted_togo;
        /* +h00A8 */ Vec3f LookAtTarget;
        /* +h00B4 */ Vec3f LookAt_togo;
        /* +h00C0 */ float fieldofview;
        /* +h00C4 */ float fieldofview2;
        // ...
    };
}

#endif // _ENTITIE_CAMERA_INC
