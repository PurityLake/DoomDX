#pragma once

#include "DoomDX.hpp"

// Shader class that encapsulates the loading and setting of the shader of the game.
// TODO: Create a more abstract version of this class
class Shader {
private:
	Doom* game;
	// Description of the input structure that will be passed to the shader
	static inline D3D11_INPUT_ELEMENT_DESC defaultIed[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	// If this file loaded correctly
	bool valid;
	// filename
	LPCWSTR filename;
	// name of the vertex function
	std::string vertexName;
	// name of the pixel function
	std::string pixelName;

	ID3D10Blob* VS;
	ID3D11VertexShader* vertexShader;	// the pointer to the vertex shader
	ID3D10Blob* PS;
	ID3D11PixelShader* pixelShader;	// the pointer to the pixel shader
	ID3D11InputLayout* pLayout;		// the pointer to the input layout

public:
	// Only defined to allow storing of Shaders in vector
	Shader();
	// Valid constructor which takes the filename in the form of a L"" string with default names for the vertex
	// function and the pixel function as "VShader" and "PShader" respectively. To ensure the file loaded
	// correctly use the `ÌsValidShader` method
	Shader(Doom* g, LPCWSTR filename, std::string vertexName = "VShader", std::string pixelName = "PShader");
	// Copy constructor is deleted
	Shader(const Shader& other) = delete;
	~Shader();

	// Checks if the shader has been properly constructed
	bool IsValidShader();
	// Injects shader into the program to be used by any subsequent object
	void SetAsCurrentShader();

	// Get pointer to the vertex shader
	ID3D11VertexShader* VertexShader();
	// Get pointer to pixel shader
	ID3D11PixelShader* PixelShader();
};