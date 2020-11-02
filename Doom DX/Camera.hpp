#pragma once

#include "DoomDX.hpp"

// Camera object that controls the movement of the camera, performing the appropriate operations
	// the provide a View and Projection matrices for the drawing of other objects
class Camera {
private:
	// refernce to the main game object
	Doom* game;
	bool recentChange = false;
	// currentState is the view matrix needed for the WVP Matrix
	DirectX::XMMATRIX currentState, camProj;
	// position of camera, looking at vector and up vector
	Vector pos, lookAt, up;

	// generate any matrices needed
	void generateMatrices();
	// updates global variables regarding camera and projection
	void updateGameState();
	bool pitchAndYaw(int dx, int dy);

public:
	// View matrice
	static DirectX::XMMATRIX camView;
	// Projection matrix
	static DirectX::XMMATRIX camProjection;

	// Default Constructor
	// Camera is give a default position of(0, 2, -8) and looks at the origin by default
	// with a default up direction of (0, 1, 0)
	Camera(Doom* g);

	// Specialised Constructror
	// Allows for custom start up for the camera without needing transformation after
	// creation
	Camera(Doom* g, Vector pos, Vector lookAt, Vector up);

	// Updates global variables if anything has been done to move the camera since the last
	// frame. Should be called once per frame
	void Update(std::shared_ptr<Mouse> mouse);
	// Move the camera in the X, Y and Z axies
	void Move(float deltaX, float deltaY, float deltaZ);
	// Move the camera on it's local axies
	void LocalMoveLeftOrRight(float delta);
	void LocalMoveForwardOrBack(float delta);

	// Get the position of the camera
	const Vector& GetPos() const;
	// Get the vector of where the camera is pointing
	const Vector& GetLookingAt() const;
	// Get the up vector 
	const Vector& GetUp() const;
};