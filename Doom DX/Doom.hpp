#pragma once

#include "DoomDX.hpp"

// CBuffer object passed to shaders
struct cbPerObject {
	// World View Projection
	DirectX::XMMATRIX WVP;
	FLOAT RandomColor;
};

// Object that represents each vertex
struct VERTEX {
	FLOAT X, Y, Z;
	DirectX::XMFLOAT4 Color;
};

class Doom {
public:
	// used to implement a delta time feature that will speed up or slow down transforms so they
	// are not too fast on a high spec computer and not too slow on lower spec computers
	std::clock_t previousTime;
	float deltaTime;

	// Screen width and height
	// #######################
	// TODO: Implement way to change windows size at runtime
	// #######################
	int ScreenWidth;
	int ScreenHeight;
	HINSTANCE					hInstance;
	Window						window;
	IDXGISwapChain* swapchain;				// the pointer to the swap chain interfce
	ID3D11Device* dev;					// the pointer to our Direct3D device interface
	ID3D11DeviceContext* devcon;				// the pointer to our Direct3D device context
	ID3D11RenderTargetView* backbuffer;			// the pointer to our back buffer
	ID3D11DepthStencilView* depthStencilView;		// depth stencil
	ID3D11Texture2D* depthStencilBuffer;	// depth stencil buffer
	ID3D11Buffer* cbPerObjectBuffer;		// constant buffer object

	std::vector<LineDef> linedefs;
	std::vector<Vertex> vertices;
	std::vector<Thing> things;
	std::vector<SideDef> sidedefs;
	std::vector<Seg> segments;
	std::vector<SubSector> subsectors;
	std::vector<Node> nodes;
	std::vector<Sector> sectors;

	cbPerObject cbPerObj;

	std::shared_ptr<Camera> camera;
	std::shared_ptr<Shader> shader;
	std::shared_ptr<Mouse> mouse;
	std::vector<std::shared_ptr<Line>> lines;

	Square* square;

	bool depthBuffer = true;

	Doom(HINSTANCE instance, int nCmdShow, int width, int height);

	void Update();
	void Render();

private:
	// Simple function that is used to test if a key is down
	inline bool KeyDown(char k) {
		return GetKeyState(k) & HIGH_ORDER_BIT;
	}

	// Recurses over Node objects to draw the overhead map. It is passed the max dimension of the map
	// Nodes start in the reverse so passing the last Node on the first call will start recursive calls through
	// each child of the Node
	void RecurseOverNodes(std::vector<Node>& nodes, std::pair<int, int> dims, Node& n);

	// The speed of the player
	static const inline float MoveAmount = 0.00001f;

	// Handles the keyboard inputs for the game. This should be delegated later to allow for modding later
	void HandleKeyboardInput();

	// Sets up DirectX necessary to allow graphics to be rendered to our window. It is vital
	// that the window be created prior to initialising DX11
	void InitDoomDX();

	// Call this once per frame to update clock and delta time
	void CalcDeltaTime();

	// Creates the handle to our window so we can create the DX11 instance.
	// `_hInstance` should be the same one as passed to the program in WinMain
	void CreateWindowHandle(HINSTANCE _hInstance, int nCmdShow, int width, int height);

	// A Windows specific funciton that handles events
	static inline LRESULT CALLBACK WindowProc(Window hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
		switch (message) {
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		}
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
};