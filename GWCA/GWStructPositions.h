#pragma once

#include <cmath>

namespace GWAPI {
	// Simple 2D cartesian coordinate vector with associated operations available.
	struct Vector2D {
		float x;
		float y;

		// Default constructor
		Vector2D() : x(0),y(0){}

		// Set both coordinates to same value
		Vector2D(const float _v) : x(_v), y(_v){}

		// Set float values
		Vector2D(const float _x, const float _y) :
			x(_x), y(_y){}

		// Test if two vectors are equal
		bool operator == (const Vector2D& _vec) const {
			return (x == _vec.x && y == _vec.y);
		}
		
		// Retrieve the negative (inverted) version of vector
		const Vector2D operator - () const {
			return Vector2D(-x, -y);
		}

		// Add two vectors
		const Vector2D operator + (const Vector2D& _vec) const {
			return Vector2D(x + _vec.x, y + _vec.y);
		}

		// Subtract vector 1 by vector 2
		const Vector2D operator - (const Vector2D& _vec) const {
			return Vector2D(x - _vec.x, y - _vec.y);
		}

		// Divide vector 1 by vector 2
		const Vector2D operator / (const Vector2D& _vec) const {
			return Vector2D(x / _vec.x, y / _vec.y);
		}


		// Add vector 2 to vector 1
		Vector2D& operator += (const Vector2D& _vec) {
			x += _vec.x; y += _vec.y;
			return *this;
		}

		// Subtract vector 2 from vector 1
		Vector2D& operator -= (const Vector2D& _vec) {
			x -= _vec.x; y -= _vec.y;
			return *this;
		}

		// Multiply vector 1 by vector 2
		Vector2D& operator *= (const Vector2D& _vec) {
			x *= _vec.x; y *= _vec.y;
			return *this;
		}

		// Divide vector 1 by vector 2
		Vector2D& operator /= (const Vector2D& _vec) {
			x /= _vec.x; y /= _vec.y;
			return *this;
		}

		// Multiply vector by a common multiple
		const Vector2D operator * (const float _w) {
			return Vector2D(x * _w, y * _w);
		}
		Vector2D&  operator *= (const float _w) {
			x *= _w; y *= _w;
			return *this;
		}

		// Divide vector by a common multiple
		const Vector2D operator / (const float _w) {
			return Vector2D(x / _w, y / _w);
		}

		Vector2D& operator /= (const float _w) {
			x /= _w; y /= _w;
			return *this;
		}

		friend const Vector2D operator * (const float _w, const Vector2D& _vec) {
			return _vec * _w;
		};

		friend const Vector2D operator / (const float _w, const Vector2D& _vec) {
			return _vec / _w;
		};

		// Dot product
		float operator * (const Vector2D& _vec) const {
			return (x * _vec.x + y * _vec.y);
		};

		float norm1() const {
			return x + y;
		}

		float norm() const {
			return sqrtf(x * x + y * y);
		}

		float sqNorm() const {
			return (x * x + y * y);
		}

		const Vector2D normalized() const {
			return (*this / norm());
		};

		Vector2D& normalize() {
			const float n = norm();
			x /= n; y /= n;
			return *this;
		};

		float distanceTo(const Vector2D& _vec){
			return sqrtf(powf(_vec.x - x, 2.0) + powf(_vec.y - y, 2.0));
		}

		float sqDistanceTo(const Vector2D& _vec){
			return powf(_vec.x - x, 2.0) + powf(_vec.y - y, 2.0);
		}

		// Angle between two vectors in radians.
		float angleBetween(const Vector2D& _vec){
			return atan2f(_vec.x - x, _vec.y - y);
		}
	};

	// 3D version of vector seen above
	struct Vector3D {
		float x;
		float y;
		float z;

		// Default constructor
		Vector3D() : x(0), y(0),z(0){}

		// Set both coordinates to same value
		Vector3D(const float _v) : x(_v), y(_v),z(_v){}

		// Set float values
		Vector3D(const float _x, const float _y, const float _z) :
			x(_x), y(_y),z(_z){}

		// Test if two vectors are equal
		bool operator == (const Vector3D& _vec) const {
			return (x == _vec.x && y == _vec.y && z == _vec.z);
		}

		// Retrieve the negative (inverted) version of vector
		const Vector3D operator - () const {
			return Vector3D(-x, -y, -z);
		}

		// Add two vectors
		const Vector3D operator + (const Vector3D& _vec) const {
			return Vector3D(x + _vec.x, y + _vec.y,z + _vec.z);
		}

		// Subtract vector 1 by vector 2
		const Vector3D operator - (const Vector3D& _vec) const {
			return Vector3D(x - _vec.x, y - _vec.y, z - _vec.z);
		}

		// Divide vector 1 by vector 2
		const Vector3D operator / (const Vector3D& _vec) const {
			return Vector3D(x / _vec.x, y / _vec.y,z / _vec.z);
		}


		// Add vector 2 to vector 1
		Vector3D& operator += (const Vector3D& _vec) {
			x += _vec.x; y += _vec.y; z += _vec.z;
			return *this;
		}

		// Subtract vector 2 from vector 1
		Vector3D& operator -= (const Vector3D& _vec) {
			x -= _vec.x; y -= _vec.y; z -= _vec.z;
			return *this;
		}

		// Multiply vector 1 by vector 2
		Vector3D& operator *= (const Vector3D& _vec) {
			x *= _vec.x; y *= _vec.y; z *= _vec.z;
			return *this;
		}

		// Divide vector 1 by vector 2
		Vector3D& operator /= (const Vector3D& _vec) {
			x /= _vec.x; y /= _vec.y; z /= _vec.z;
			return *this;
		}

		// Multiply vector by a common multiple
		const Vector3D operator * (const float _w) {
			return Vector3D(x * _w, y * _w, z * _w);
		}

		Vector3D&  operator *= (const float _w) {
			x *= _w; y *= _w; z *= _w;
			return *this;
		}

		// Divide vector by a common multiple
		const Vector3D operator / (const float _w) {
			return Vector3D(x / _w, y / _w, z / _w);
		}

		Vector3D& operator /= (const float _w) {
			x /= _w; y /= _w; z /= _w;
			return *this;
		}

		friend const Vector3D operator * (const float _w, const Vector3D& _vec) {
			return _vec * _w;
		};

		friend const Vector3D operator / (const float _w, const Vector3D& _vec) {
			return _vec / _w;
		};

		// Dot product
		float operator * (const Vector3D& _vec) const {
			return (x * _vec.x + y * _vec.y + z * _vec.z);
		};

		// Cross product
		const Vector3D operator ^ (const Vector3D& _vec) const {
			return Vector3D(
							(y * _vec.z) - (_vec.y * z),
							(z * _vec.x) - (_vec.z * x),
							(x * _vec.y) - (_vec.x * y)
							);
		};

		float norm1() const {
			return x + y + z;
		}

		float norm() const {
			return sqrtf(x * x + y * y + z * z);
		}

		float sqNorm() const {
			return (x * x + y * y + z * z);
		}

		const Vector3D normalized() const {
			return (*this / norm());
		};

		Vector3D& normalize() {
			const float n = norm();
			x /= n; y /= n;
			return *this;
		};

		float distanceTo(const Vector3D& _vec){
			return sqrtf(powf(_vec.x - x, 2.0) + powf(_vec.y - y, 2.0) + powf(_vec.z - z,2.0));
		}

		float sqDistanceTo(const Vector3D& _vec){
			return powf(_vec.x - x, 2.0) + powf(_vec.y - y, 2.0) + powf(_vec.z - z, 2.0);
		}

		float angleBetween(const Vector3D& _vec) const {
			return atan2f((*this ^ _vec).norm(), (*this * _vec));
		}

	};


	namespace GW {


		struct GamePos : public Vector2D {
			unsigned int zplane;

			GamePos() : Vector2D(), zplane(0) {}

			GamePos(const float _x, const float _y)
				: Vector2D(_x, _y), zplane(0) {}

			GamePos(const float _x, const float _y, const unsigned int _zplane)
			: Vector2D(_x,_y),zplane(_zplane) {}
		};

	}

}