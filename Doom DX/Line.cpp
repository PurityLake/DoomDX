#include "DoomDX.hpp"

using namespace std;
using namespace DirectX;

Line::Line(Doom* d, float x1, float y1, float x2, float y2, float r, float g, float b, float a) {
	game = d;
	// set the first two vertices
	vertices[0] = { x1, y1, 0.0f, DirectX::XMFLOAT4{ r, g, b, a } };
	vertices[1] = { x2, y2, 0.0f, DirectX::XMFLOAT4{ r, g, b, a } };

	// create vertex buffer
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX) * 2;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.MiscFlags = 0;

	game->dev->CreateBuffer(&bd, NULL, &pVBuffer);

	if (pVBuffer) {
		D3D11_MAPPED_SUBRESOURCE ms;
		game->devcon->Map(pVBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
		memcpy(ms.pData, vertices, sizeof(vertices));
		game->devcon->Unmap(pVBuffer, NULL);
	}
	else {
		throw runtime_error("Could not allocate pVBuffer for Line instance");
	}
}

void Line::Draw() {
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	
	// set the topology to drawing lines
	game->devcon->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
	// set the vertex buffer
	game->devcon->IASetVertexBuffers(0, 1, &pVBuffer, &stride, &offset);

	// set WVP for the shader
	game->cbPerObj.WVP = XMMatrixTranspose(XMMatrixScaling(1.5f, 1.5f, 1.0f) * XMMatrixTranslation(-0.6f, 0.7f * 1.5f, 0.0f));
	game->devcon->UpdateSubresource(game->cbPerObjectBuffer, 0, NULL, &game->cbPerObj, 0, 0);
	game->devcon->VSSetConstantBuffers(0, 1, &game->cbPerObjectBuffer);

	// draw the two loaded vertices
	game->devcon->Draw(2, 0);
	game->devcon->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}