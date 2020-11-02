#include "DoomDX.hpp"

using namespace std;
using namespace DirectX;

XMMATRIX Camera::camProjection;
XMMATRIX Camera::camView;

Camera::Camera(Doom *g) {
	game = g;
	// set the defaults for the pos, look at and up vectors
	pos	= { 0.0f, 2.0f, -8.0f, 0.0f };
	lookAt = { 0.0f, 0.0f, 0.0f, 0.0f };
	up = { 0.0f, 1.0f, 0.0f, 0.0f };
	// creates the projection matrix
	camProj = XMMatrixPerspectiveFovLH(0.4f *3.14f, (float)game->ScreenWidth / game->ScreenHeight, 1.0f, 1000.0f);
	// set the view matrix
	currentState = XMMatrixLookAtLH(pos, lookAt, up);
	camView = currentState;
	camProjection = camProj;
}

Camera::Camera(Doom *g, Vector pos, Vector lookAt, Vector up)
	: game(g), pos(pos), lookAt(lookAt), up(up) {
	camProj = XMMatrixPerspectiveFovLH(0.4f *3.14f, (float)game->ScreenWidth / game->ScreenHeight, 1.0f, 1000.0f);
	currentState = XMMatrixLookAtLH(pos, lookAt, up);
	camView = currentState;
	camProjection = camProj;
}

void Camera::generateMatrices() {
	currentState = XMMatrixLookAtLH(pos, lookAt, up);
}

void Camera::updateGameState() {
	camView = currentState;
	camProjection = camProj;
}

bool Camera::pitchAndYaw(int dx, int dy) {
	// if not movement then don't make a move
	if (dx == 0 && dy == 0) return false;
	// gets a pitch, yaw and roll transformation matrices to effect the look at of the class
	auto mat = XMMatrixRotationRollPitchYaw(
		dy * 0.0005f * game->deltaTime, dx * 0.0005f * game->deltaTime, 0.0f);
	// transforms the look at vector
	lookAt = XMVector3Transform(lookAt - pos, mat);
	return true;
}

void Camera::Update(shared_ptr<Mouse> mouse) {
	if (recentChange || pitchAndYaw(mouse->GetXChange(), mouse->GetYChange())) {
		recentChange = false;
		generateMatrices();
		updateGameState();
	}
}

void Camera::Move(float x, float y, float z) {
	if (x == 0.0f && y == 0.0f && z == 0.0f) return;
	recentChange = true;
	pos.x += x;
	pos.y += y;
	pos.z += z;
}

void Camera::LocalMoveLeftOrRight(float delta) {
	if (delta == 0.0f) return;
	recentChange = true;
	XMVECTOR s = XMVectorReplicate(delta);
	auto flatLookAt = lookAt;
	flatLookAt.y = pos.y;
	pos = XMVectorMultiplyAdd(s, XMVector3Cross(up, flatLookAt - pos), pos);
	lookAt = XMVectorMultiplyAdd(s, XMVector3Cross(up, lookAt - pos), lookAt);
}

void Camera::LocalMoveForwardOrBack(float delta) {
	if (delta == 0.0f) return;
	recentChange = true;
	XMVECTOR s = XMVectorReplicate(delta);
	auto flatLookAt = lookAt;
	flatLookAt.y = pos.y;
	pos = XMVectorMultiplyAdd(s, flatLookAt, pos);
	lookAt = XMVectorMultiplyAdd(s, pos - lookAt, lookAt);
}

const Vector& Camera::GetPos() const {
	return pos;
}

const Vector& Camera::GetLookingAt() const {
	return lookAt;
}
const Vector& Camera::GetUp() const {
	return up;
}