#pragma once

#include "DoomDX.hpp"

// Class that allows for the drawing of a square
class Square {
private:
	Doom* game;

	static inline bool initialised = false;
	// TODO: Replace colour here with textures
	static inline VERTEX vertices[] = {
		{ -1.0f, -1.0f, 0.5f, DirectX::XMFLOAT4{ 1.0f, 0.0f, 0.0f, 1.0f } },
		{ -1.0f,  1.0f, 0.5f, DirectX::XMFLOAT4{ 0.0f, 1.0f, 0.0f, 1.0f } },
		{  1.0f,  1.0f, 0.5f, DirectX::XMFLOAT4{ 0.0f, 0.0f, 1.0f, 1.0f } },
		{  1.0f, -1.0f, 0.5f, DirectX::XMFLOAT4{ 0.0f, 1.0f, 0.0f, 1.0f } }
	};
	// front facing indices
	static inline DWORD indices[] = {
		0, 1, 2,
		0, 2, 3
	};
	// back facing indices
	static inline DWORD reverseIndices[] = {
		2, 1, 0,
		3, 2, 0
	};

	// vertex and index buffers
	static inline ID3D11Buffer* pVBuffer;		 // the pointer to the vertex buffer
	static inline ID3D11Buffer* pIBuffer;		 // the pointer to the index buffer
	static inline ID3D11Buffer* pIBufferReverse; // the pointer to the reversed index buffer

	static inline const DirectX::XMVECTOR up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	static inline const DirectX::XMVECTOR right = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	static inline const DirectX::XMVECTOR forward = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	bool stateChanged = true;

	float rotX;
	float rotY;
	float rotZ;

	float translateX;
	float translateY;
	float translateZ;

	float scaleX;
	float scaleY;
	float scaleZ;

	bool drawReverseSide;

	DirectX::XMMATRIX World;

public:
	Square(Doom* g);

	// Cleanup static data buffers
	void CleanUp();

	// Used to create the static vertex and index buffers for future instances
	void Initialise();

	// Draws the square
	// Front only if passed false in the constructor
	// Back and Front is true is passed to the constructor
	// By default should be Back and Front
	void Draw();

	// Add to the current rotations around the X, Y and Z axis
	void AddRot(float x, float y, float z);
	// Set the rotation for X, Y and Z access
	void SetRot(float x, float y, float z);
	// Returns the current rotation around the X Axis
	float GetRotX();
	// Returns the current rotation around the Y Axis
	float GetRotY();
	// Returns the current rotation around the Z Axis
	float GetRotZ();

	// Move the square in the X, Y and Z Axis
	void Move(float x, float y, float z);
	// Set the position of the square 
	void SetPos(float x, float y, float z);
	// Get X pos
	float GetX();
	// Get Y pos
	float GetY();
	// Get Z pos
	float GetZ();

	// Scale the square in X, Y and Z Axis
	void AddScale(float x, float y, float z);
	// Set the scale of the square in X, Y and Z axis
	void SetScale(float x, float y, float z);
	// Get X Scale
	float GetScaleX();
	// Get Y Scale
	float GetScaleY();
	// Get Z Scale
	float GetScaleZ();
};