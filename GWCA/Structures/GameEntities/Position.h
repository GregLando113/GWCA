#pragma once

#include <cmath>

namespace GWCA {
	// Simple 2D cartesian coordinate vector with associated operations available.
	struct Vector2f {
		float x;
		float y;

		// Default constructor
		Vector2f() : Vector2f(0, 0) {}

		// Set both coordinates to same value
		Vector2f(const float _v) : Vector2f(_v, _v) {}

		// Set float values
		Vector2f(const float _x, const float _y) :
			x(_x), y(_y){}

		// Test if two vectors are equal
		bool operator == (const Vector2f& _vec) const {
			return (x == _vec.x && y == _vec.y);
		}
		
		// Retrieve the negative (inverted) version of vector
		const Vector2f operator - () const {
			return Vector2f(-x, -y);
		}

		// Add two vectors
		const Vector2f operator + (const Vector2f& _vec) const {
			return Vector2f(x + _vec.x, y + _vec.y);
		}

		// Subtract vector 1 by vector 2
		const Vector2f operator - (const Vector2f& _vec) const {
			return Vector2f(x - _vec.x, y - _vec.y);
		}

		// Divide vector 1 by vector 2
		const Vector2f operator / (const Vector2f& _vec) const {
			return Vector2f(x / _vec.x, y / _vec.y);
		}

		// Add vector 2 to vector 1
		Vector2f& operator += (const Vector2f& _vec) {
			x += _vec.x; y += _vec.y;
			return *this;
		}

		// Subtract vector 2 from vector 1
		Vector2f& operator -= (const Vector2f& _vec) {
			x -= _vec.x; y -= _vec.y;
			return *this;
		}

		// Multiply vector 1 by vector 2
		Vector2f& operator *= (const Vector2f& _vec) {
			x *= _vec.x; y *= _vec.y;
			return *this;
		}

		// Divide vector 1 by vector 2
		Vector2f& operator /= (const Vector2f& _vec) {
			x /= _vec.x; y /= _vec.y;
			return *this;
		}

		// Multiply vector by a common multiple
		const Vector2f operator * (const float _w) const {
			return Vector2f(x * _w, y * _w);
		}
		Vector2f& operator *= (const float _w) {
			x *= _w; y *= _w;
			return *this;
		}

		// Divide vector by a common multiple
		const Vector2f operator / (const float _w) const {
			return Vector2f(x / _w, y / _w);
		}

		Vector2f& operator /= (const float _w) {
			x /= _w; 
			y /= _w;
			return *this;
		}

		friend const Vector2f operator * (const float _w, const Vector2f& _vec) {
			return _vec * _w;
		};

		friend const Vector2f operator / (const float _w, const Vector2f& _vec) {
			return _vec / _w;
		};

		// Dot product
		float operator * (const Vector2f& _vec) const {
			return (x * _vec.x + y * _vec.y);
		};

		float Norm1() const {
			return x + y;
		}

		float Norm() const {
			return sqrtf(x * x + y * y);
		}

		float SquaredNorm() const {
			return (x * x + y * y);
		}

		const Vector2f Normalized() const {
			return (*this / Norm());
		};

		Vector2f& Normalize() {
			const float n = Norm();
			x /= n; 
			y /= n;
			return *this;
		};

		// computes the distance to the other vector
		float DistanceTo(const Vector2f& _v) const {
			return std::sqrt((_v.x - x) * (_v.x - x) + (_v.y - y) * (_v.y - y));
		}

		float SquaredDistanceTo(const Vector2f& _v) const {
			return (_v.x - x) * (_v.x - x) + (_v.y - y) * (_v.y - y);
		}

		// Angle between two vectors in radians.
		float AngleBetween(const Vector2f& _v) const {
			return atan2f(_v.x - x, _v.y - y);
		}

		Vector2f Rotated(float rotation) const {
			return Rotated(std::cos(rotation), std::sin(rotation));
		}

		Vector2f Rotated(float cos, float sin) const {
			return Vector2f(x * cos - y * sin, x * sin + y * cos);
		}
	};

	// 3D version of vector seen above
	struct Vector3f {
		float x;
		float y;
		float z;

		// Default constructor
		Vector3f() : Vector3f(0, 0, 0) {}

		// Set both coordinates to same value
		Vector3f(const float _v) : Vector3f(_v, _v, _v) {}

		// Set float values
		Vector3f(const float _x, const float _y, const float _z) :
			x(_x), y(_y),z(_z){}

		// Test if two vectors are equal
		bool operator == (const Vector3f& _vec) const {
			return (x == _vec.x && y == _vec.y && z == _vec.z);
		}

		// Retrieve the negative (inverted) version of vector
		const Vector3f operator - () const {
			return Vector3f(-x, -y, -z);
		}

		// Add two vectors
		const Vector3f operator + (const Vector3f& _vec) const {
			return Vector3f(x + _vec.x, y + _vec.y,z + _vec.z);
		}

		// Subtract vector 1 by vector 2
		const Vector3f operator - (const Vector3f& _vec) const {
			return Vector3f(x - _vec.x, y - _vec.y, z - _vec.z);
		}

		// Divide vector 1 by vector 2
		const Vector3f operator / (const Vector3f& _vec) const {
			return Vector3f(x / _vec.x, y / _vec.y,z / _vec.z);
		}

		// Add vector 2 to vector 1
		Vector3f& operator += (const Vector3f& _vec) {
			x += _vec.x; y += _vec.y; z += _vec.z;
			return *this;
		}

		// Subtract vector 2 from vector 1
		Vector3f& operator -= (const Vector3f& _vec) {
			x -= _vec.x; y -= _vec.y; z -= _vec.z;
			return *this;
		}

		// Multiply vector 1 by vector 2
		Vector3f& operator *= (const Vector3f& _vec) {
			x *= _vec.x; y *= _vec.y; z *= _vec.z;
			return *this;
		}

		// Divide vector 1 by vector 2
		Vector3f& operator /= (const Vector3f& _vec) {
			x /= _vec.x; y /= _vec.y; z /= _vec.z;
			return *this;
		}

		// Multiply vector by a common multiple
		const Vector3f operator * (const float _w) {
			return Vector3f(x * _w, y * _w, z * _w);
		}

		Vector3f&  operator *= (const float _w) {
			x *= _w; y *= _w; z *= _w;
			return *this;
		}

		// Divide vector by a common multiple
		const Vector3f operator / (const float _w) {
			return Vector3f(x / _w, y / _w, z / _w);
		}

		Vector3f& operator /= (const float _w) {
			x /= _w; y /= _w; z /= _w;
			return *this;
		}

		friend const Vector3f operator * (const float _w, const Vector3f& _vec) {
			return _vec * _w;
		};

		friend const Vector3f operator / (const float _w, const Vector3f& _vec) {
			return _vec / _w;
		};

		// Dot product
		float operator * (const Vector3f& _vec) const {
			return (x * _vec.x + y * _vec.y + z * _vec.z);
		};

		// Cross product
		const Vector3f operator ^ (const Vector3f& _vec) const {
			return Vector3f(
							(y * _vec.z) - (_vec.y * z),
							(z * _vec.x) - (_vec.z * x),
							(x * _vec.y) - (_vec.x * y)
							);
		};

		float Norm1() const {
			return x + y + z;
		}

		float Norm() const {
			return sqrtf(x * x + y * y + z * z);
		}

		float SquaredNorm() const {
			return (x * x + y * y + z * z);
		}

		const Vector3f Normalized() const {
			return (*this / Norm());
		};

		Vector3f& Normalize() {
			const float n = Norm();
			x /= n; y /= n;
			return *this;
		};

		float DistanceTo(const Vector3f& v) const {
			return std::sqrt((v.x - x) * (v.x - x) + (v.y - y) * (v.y - y) + (v.z - z) * (v.z - z));
		}

		float SquaredDistanceTo(const Vector3f& v) const {
			return (v.x - x) * (v.x - x) + (v.y - y) * (v.y - y) + (v.z - z) * (v.z - z);
		}

		float AngleBetween(const Vector3f& v) const {
			return atan2f((*this ^ v).Norm(), (*this * v));
		}
	};


	namespace GW {
		struct GamePos : public Vector2f {
			unsigned int zplane;

			GamePos() : GamePos(0, 0, 0) {}

			GamePos(const float _x, const float _y)	: GamePos(_x, _y, 0) {}

			GamePos(const float _x, const float _y, const unsigned int _zplane)
			: Vector2f(_x,_y), zplane(_zplane) {}
		};
	}
}
