#pragma once

#include "DoomDX.hpp"

// A custom vector that is used to allow for easier controller over XMVECTORs that
// allows for typical vector operations
struct Vector {
private:
	// Calculates the distance of the vector
	float Norm() const {
		return sqrt(x * x + y * y + z * z);
	}
public:
	// Components of the vector
	float x, y, z, w;

	// simple constructors for the class
	Vector()
		: x(0.0f), y(0.0f), z(0.0f), w(0.0f) { }
	Vector(float x, float y, float z, float w)
		: x(x), y(y), z(z), w(w) { }

	// implicit cast from XMVECTOR
	Vector(DirectX::XMVECTOR vec) {
		x = vec.m128_f32[0];
		y = vec.m128_f32[1];
		z = vec.m128_f32[2];
		w = 1.0f;
	}

	// implicit cast to XMVECTOR
	operator DirectX::XMVECTOR() const {
		return DirectX::XMVectorSet(x, y, z, w);
	}

	// Returns a normalised vector of this vector
	Vector Normalise() const {
		float norm = 1 / Norm();
		return Vector{ x * norm, y * norm, z * norm, w };
	}

	// Add two vertex objects
	Vector operator +(const Vector& other) const {
		return Vector{ x + other.x, y + other.y, z + other.z, w };
	}
	Vector operator +(const DirectX::XMVECTOR& vec) const {
		return *this + (Vector)vec;
	}

	// Subtract vertex objects
	Vector operator -(const Vector& other) const {
		return Vector{ x - other.x, y - other.y, z - other.z, w };
	}
	Vector operator -(const DirectX::XMVECTOR& vec) const {
		return *this - (Vector)vec;
	}

	// Performs scalar multiplication of vector
	Vector operator *(float other) const {
		return Vector{ x * other, y * other, z * other, w };
	}
};