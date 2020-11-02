#include "DoomDX.hpp"

using namespace std;
using namespace DirectX;

Square::Square(Doom* g) : game(g), drawReverseSide(true), rotX(0.0f), rotY(0.0f), rotZ(0.0f),
	translateX(0.0f), translateY(0.0f), translateZ(0.0f),
	scaleX(1.0f), scaleY(1.0f), scaleZ(1.0f)
{
	// only runs the first time creating a square
	if (!initialised) {
		Initialise();
		initialised = true;
	}
}

// Cleanup static data buffers
void Square::CleanUp() {
	if (initialised) {
		pVBuffer->Release();
		pIBuffer->Release();
		pIBufferReverse->Release();
	}
}

// Used to create the static vertex and index buffers for future instances
void Square::Initialise() {
	// create index buffer
	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(DWORD) * 12 * 3;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	// Creates front facing index buffer
	D3D11_SUBRESOURCE_DATA iinitData;

	iinitData.pSysMem = indices;
	game->dev->CreateBuffer(&indexBufferDesc, &iinitData, &pIBuffer);

	// Creat3es back facing index buffer
	D3D11_SUBRESOURCE_DATA reverseInitData;

	reverseInitData.pSysMem = reverseIndices;
	game->dev->CreateBuffer(&indexBufferDesc, &reverseInitData, &pIBufferReverse);

	// create the vertex buffer
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_DYNAMIC;				// write access for CPU and GPU
	bd.ByteWidth = sizeof(VERTEX) * 8;			// size is the VERTEX struct * 3
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;	// use as a vertex buffer
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	// allow CPU to write in buffer

	game->dev->CreateBuffer(&bd, NULL, &pVBuffer);	// create the buffer

	// copy the vertices into the buffer
	D3D11_MAPPED_SUBRESOURCE ms;
	game->devcon->Map(pVBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);	// map the buffer
	memcpy(ms.pData, vertices, sizeof(vertices));					// copy the data
	game->devcon->Unmap(pVBuffer, NULL);
}

void Square::Draw() {
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	// Set the vertex buffer
	game->devcon->IASetVertexBuffers(0, 1, &pVBuffer, &stride, &offset);

	// Only calculate if there has been a change since the last frame
	if (stateChanged) {
		// Reset World to be the indentity Matrix
		World = XMMatrixIdentity();
		// Rotate in the order X, Y, Z
		World = XMMatrixRotationX(rotX) * XMMatrixRotationY(rotY) * XMMatrixRotationZ(rotZ);
		// Add the scaling
		World *= XMMatrixScaling(scaleX, scaleY, scaleZ);
		// Add the translation
		World *= XMMatrixTranslation(translateX, translateY, translateZ);
	}

	// Set the WVP Matrix by adding the new World Matrix
	game->cbPerObj.WVP = XMMatrixTranspose(World * Camera::camView * Camera::camProjection);
	game->devcon->UpdateSubresource(game->cbPerObjectBuffer, 0, NULL, &game->cbPerObj, 0, 0);
	game->devcon->VSSetConstantBuffers(0, 1, &game->cbPerObjectBuffer);
	
	// Set the front facing index buffer
	game->devcon->IASetIndexBuffer(pIBuffer, DXGI_FORMAT_R32_UINT, 0);
	game->devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	game->devcon->DrawIndexed(6, 0, 0);

	if (drawReverseSide) {
		// Set the back facing index buffer
		game->devcon->IASetIndexBuffer(pIBufferReverse, DXGI_FORMAT_R32_UINT, 0);
		game->devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		game->devcon->DrawIndexed(6, 0, 0);
	}
}

void Square::AddRot(float x, float y, float z) {
	rotX += x;
	rotY += y;
	rotZ += z;
	stateChanged = true;
}
void Square::SetRot(float x, float y, float z) {
	rotX = x;
	rotY = y;
	rotZ = z;
	stateChanged = true;
}
float Square::GetRotX() {
	return rotX;
}
float Square::GetRotY() {
	return rotY;
}
float Square::GetRotZ() {
	return rotZ;
}

void Square::Move(float x, float y, float z) {
	translateX += x;
	translateY += y;
	translateZ += z;
	stateChanged = true;
}
void Square::SetPos(float x, float y, float z) {
	translateX = x;
	translateY = y;
	translateZ = z;
	stateChanged = true;
}
float Square::GetX() {
	return translateX;
}
float Square::GetY() {
	return translateY;
}
float Square::GetZ() {
	return translateZ;
}

void Square::AddScale(float x, float y, float z) {
	scaleX += x;
	scaleY += y;
	scaleZ += z;
	stateChanged = true;
}
void Square::SetScale(float x, float y, float z) {
	scaleX = x;
	scaleY = y;
	scaleZ = z;
	stateChanged = true;
}
float Square::GetScaleX() {
	return scaleX;
}
float Square::GetScaleY() {
	return scaleY;
}
float Square::GetScaleZ() {
	return scaleZ;
}