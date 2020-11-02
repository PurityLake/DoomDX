#include "DoomDX.hpp"

using namespace std;

Shader::Shader() {
	valid = false;
	game = nullptr;
	filename = nullptr;
	PS = nullptr;
	VS = nullptr;
	pLayout = nullptr;
	pixelShader = nullptr;
	vertexShader = nullptr;
}

Shader::Shader(Doom* g, LPCWSTR filename, string vertexName, string pixelName)
	: game(g), filename(filename), vertexName(vertexName), pixelName(pixelName),
	PS(nullptr), VS(nullptr), pLayout(nullptr), pixelShader(nullptr), vertexShader(nullptr)
{
	HRESULT hr;
	hr = D3DCompileFromFile(filename, 0, 0, LPSTR(vertexName.c_str()), "vs_4_0", 0, 0, &VS, 0);
	if (FAILED(hr)) {
		valid = false;
		return;
	}
	hr = D3DCompileFromFile(filename, 0, 0, LPSTR(pixelName.c_str()),  "ps_4_0", 0, 0, &PS, 0);
	if (FAILED(hr)) {
		valid = false;
		return;
	}

	// encapsulate both shaders into shader objects
	hr = game->dev->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &this->vertexShader);
	if (FAILED(hr)) {
		valid = false;
		return;
	}
	hr = game->dev->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &this->pixelShader);
	if (FAILED(hr)) {
		valid = false;
		return;
	}

	valid = true;
}

Shader::~Shader() {
	if (VS) VS->Release();
	if (PS) PS->Release();
	if (vertexShader) vertexShader->Release();
	if (pixelShader) pixelShader->Release();
}

bool Shader::IsValidShader() {
	return valid;
}

void Shader::SetAsCurrentShader() {
	// set Vertex shader
	game->devcon->VSSetShader(vertexShader, 0, 0);
	// set Pixel shader
	game->devcon->PSSetShader(pixelShader, 0, 0);

	// Sets input layout
	game->dev->CreateInputLayout(defaultIed, 2, VS->GetBufferPointer(), VS->GetBufferSize(), &pLayout);
	game->devcon->IASetInputLayout(pLayout);
}

ID3D11VertexShader* Shader::VertexShader() {
	return this->vertexShader;
}

ID3D11PixelShader* Shader::PixelShader() {
	return this->pixelShader;
}