#include "stdafx.h"

#include <GWCA/GameContainers/GamePos.h>

namespace GW
{
    float GetDistance(Vec3f p1, Vec3f p2) {
        return sqrtf(GetSquareDistance(p1, p2));
    }

    float GetDistance(Vec2f p1, Vec2f p2) {
        return sqrtf(GetSquareDistance(p1, p2));
    }

    float GetNorm(Vec3f p) {
        return sqrtf(GetSquaredNorm(p));
    }

    float GetNorm(Vec2f p) {
        return sqrtf(GetSquaredNorm(p));
    }

    Vec2f Rotate(Vec2f v, float rotation) {
        return Rotate(v, cosf(rotation), sinf(rotation));
    }
}