#pragma once

namespace GW {
    typedef struct Mat4x3f {
        float _11;
        float _12;
        float _13;
        float _14;
        float _21;
        float _22;
        float _23;
        float _24;
        float _31;
        float _32;
        float _33;
        float _34;
        uint32_t flags;
    } Mat4x3f;

    struct Vec3f {
        float x;
        float y;
        float z;

        Vec3f(float _x, float _y, float _z)
            : x(_x), y(_y), z(_z)
        {
        }

        Vec3f() : Vec3f(0.f, 0.f, 0.f)
        {
        }
    };

    inline Vec3f& operator+=(Vec3f& lhs, Vec3f rhs) {
        lhs.x += rhs.x;
        lhs.y += rhs.y;
        lhs.z += rhs.z;
        return lhs;
    }

    inline Vec3f operator+(Vec3f lhs, Vec3f rhs) {
        lhs += rhs;
        return lhs;
    }

    inline Vec3f& operator-=(Vec3f& lhs, Vec3f rhs) {
        lhs.x -= rhs.x;
        lhs.y -= rhs.y;
        lhs.z -= rhs.z;
        return lhs;
    }

    inline Vec3f operator-(Vec3f lhs, Vec3f rhs) {
        lhs -= rhs;
        return lhs;
    }

    inline Vec3f operator-(Vec3f v) {
        return Vec3f(-v.x, -v.y, -v.z);
    }

    inline Vec3f& operator*=(Vec3f& lhs, float rhs) {
        lhs.x *= rhs;
        lhs.y *= rhs;
        lhs.z *= rhs;
        return lhs;
    }

    inline Vec3f operator*(Vec3f lhs, float rhs) {
        lhs *= rhs;
        return lhs;
    }

    inline Vec3f operator*(float lhs, Vec3f rhs) {
        rhs *= lhs;
        return rhs;
    }

    inline Vec3f& operator/=(Vec3f& lhs, float rhs) {
        lhs.x /= rhs;
        lhs.y /= rhs;
        lhs.z /= rhs;
        return lhs;
    }

    inline Vec3f operator/(Vec3f lhs, float rhs) {
        lhs /= rhs;
        return lhs;
    }

    inline Vec3f operator/(float lhs, Vec3f rhs) {
        rhs *= lhs;
        return rhs;
    }

    inline bool operator==(Vec3f lhs, Vec3f rhs) {
        return (lhs.x == rhs.x) && (lhs.y == rhs.y) && (lhs.z == rhs.z);
    }

    struct Vec2f {
        float x;
        float y;

        Vec2f(float _x, float _y)
            : x(_x), y(_y)
        {
        }

        Vec2f() : Vec2f(0.f, 0.f)
        {
        }
    };

    inline Vec2f& operator+=(Vec2f& lhs, Vec2f rhs)
    {
        lhs.x += rhs.x;
        lhs.y += rhs.y;
        return lhs;
    }

    inline Vec2f operator+(Vec2f lhs, Vec2f rhs) {
        lhs += rhs;
        return lhs;
    }

    inline Vec2f& operator-=(Vec2f& lhs, Vec2f rhs)
    {
        lhs.x -= rhs.x;
        lhs.y -= rhs.y;
        return lhs;
    }

    inline Vec2f operator-(Vec2f lhs, Vec2f rhs) {
        lhs -= rhs;
        return lhs;
    }

    inline Vec2f operator-(Vec2f v) {
        return Vec2f(-v.x, -v.y);
    }

    inline Vec2f& operator*=(Vec2f& lhs, float rhs) {
        lhs.x *= rhs;
        lhs.y *= rhs;
        return lhs;
    }

    inline Vec2f operator*(Vec2f lhs, float rhs) {
        lhs *= rhs;
        return lhs;
    }

    inline Vec2f operator*(float lhs, Vec2f rhs) {
        rhs *= lhs;
        return rhs;
    }

    inline Vec2f& operator/=(Vec2f& lhs, float rhs) {
        lhs.x /= rhs;
        lhs.y /= rhs;
        return lhs;
    }

    inline Vec2f operator/(Vec2f lhs, float rhs) {
        lhs /= rhs;
        return lhs;
    }

    inline Vec2f operator/(float lhs, Vec2f rhs) {
        rhs *= lhs;
        return rhs;
    }

    inline bool operator==(Vec2f lhs, Vec2f rhs) {
        return (lhs.x == rhs.x) && (lhs.y == rhs.y);
    }

    inline float GetSquareDistance(Vec3f p1, Vec3f p2) {
        return (p1.x - p2.x) * (p1.x - p2.x) +
               (p1.y - p2.y) * (p1.y - p2.y) +
               (p1.z - p2.z) * (p1.z - p2.z);
    }

    inline float GetSquareDistance(Vec2f p1, Vec2f p2) {
        return (p1.x - p2.x) * (p1.x - p2.x) +
               (p1.y - p2.y) * (p1.y - p2.y);
    }

    float GetDistance(Vec3f p1, Vec3f p2);
    float GetDistance(Vec2f p1, Vec2f p2);

    inline float GetSquaredNorm(Vec3f p) {
        return (p.x * p.x) + (p.y * p.y) + (p.z * p.z);
    }

    inline float GetSquaredNorm(Vec2f p) {
        return (p.x * p.x) + (p.y * p.y);
    }

    float GetNorm(Vec3f p);
    float GetNorm(Vec2f p);

    inline Vec3f Normalize(Vec3f v) {
        float n = GetNorm(v);
        v.x /= n;
        v.y /= n;
        v.z /= n;
        return v;
    }

    inline Vec2f Normalize(Vec2f v) {
        float n = GetNorm(v);
        v.x /= n;
        v.y /= n;
        return v;
    }

    inline Vec2f Rotate(Vec2f v, float cos, float sin) {
        Vec2f res;
        res.x = (v.x * cos) - (v.y * sin);
        res.y = (v.x * sin) + (v.y * cos);
        return res;
    }

    Vec2f Rotate(Vec2f v, float rotation);

    struct GamePos {
        float    x;
        float    y;
        uint32_t zplane;

        GamePos(float _x, float _y, uint32_t _zplane)
            : x(_x), y(_y), zplane(_zplane)
        {
        }

        GamePos() : GamePos(0.f, 0.f, 0)
        {
        }

        GamePos(Vec2f v)
            : GamePos(v.x, v.y, 0)
        {
        }

        operator Vec2f() const {
            return Vec2f(x, y);
        }
    };

    inline bool operator==(GamePos lhs, GamePos rhs) {
        return (lhs.x == rhs.x) && (lhs.y == rhs.y) && (lhs.zplane == rhs.zplane);
    }
}
