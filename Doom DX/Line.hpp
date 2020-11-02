#pragma once

#include "DoomDX.hpp"

// Class representing a drawable line that can be used for drawing meshes and overhead maps.
// A Line is defined by two vertices and a colour
class Line {
private:
	// reference to the main game object
	Doom* game;
	// The two vertices that define the line
	VERTEX vertices[2];

	// vertex buffer
	ID3D11Buffer* pVBuffer;

	// DUR vectors

	// up vector
	static inline const DirectX::XMVECTOR up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	// right vecctor
	static inline const DirectX::XMVECTOR right = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	// forward vector
	static inline const DirectX::XMVECTOR forward = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

public:
	// Constructor used to create the Line class
	// d - pointer to the main game oject
	// x1, y1 - first point of the line
	// x2, y2 - second point of the line
	// r, g, b, a - color of the line
	Line(Doom* d, float x1, float y1, float x2, float y2, float r = 1.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f);

	// Method to draw the line
	void Draw();
};