#include "DoomDX.hpp"

#include <fstream>
#include <iomanip>
#include <intrin.h>
#include <string>

using namespace std;
using namespace DirectX;

template <typename T>
void HandleWadSection(vector<T>& lump, const LumpInfo& info, ifstream& wad) {
	auto currentPos = wad.tellg();
	int endPos = info.filepos + info.size;
	T t;

	wad.seekg(info.filepos);
	while (wad.tellg() < endPos) {
		wad.read((char *)&t, sizeof(T));
		lump.push_back(t);
	}
	wad.clear();
	wad.seekg(currentPos);
}

pair<int, int> getMaxDimensions(const vector<Vertex>& vertices) {
	int width = 0, height = 0;
	for (const auto& v : vertices) {
		int x = abs(v.x);
		int y = abs(v.y);
		if (x > width) {
			width = x;
		}
		if (y > height) {
			height = y;
		}
	}
	cout << width << " " << height;
	return make_pair(width, height);
}

void Doom::RecurseOverNodes(vector<Node>& nodes, pair<int, int> dims, Node& n) {
	float x = float(n.x) / float(dims.first),
		y = float(n.y) / float(dims.second),
		dx = float(n.dx) / float(dims.first),
		dy = float(n.dy) / float(dims.second),
		r = (rand() % 255) / 255.0f,
		g = (rand() % 255) / 255.0f,
		b = (rand() % 255) / 255.0f;

	if (n.leftChild >= 0) {
		RecurseOverNodes(nodes, dims, nodes[n.leftChild]);
	}
	else {
		auto idx = n.leftChild & 0b0111111111111111;
		auto sub = subsectors[idx];
		for (int i = sub.firstSegNum; i < sub.firstSegNum + sub.segCount; i++) {
			auto seg = segments[i];
			auto start_v = vertices[seg.startnum];
			auto end_v = vertices[seg.endnum];
			lines.push_back(make_shared<Line>(this,
				float(start_v.x) / float(dims.first), float(start_v.y) / float(dims.second),
				float(end_v.x) / float(dims.first), float(end_v.y) / float(dims.second),
				r, g, b));
		}
	}

	if (n.rightChild >= 0) {
		RecurseOverNodes(nodes, dims, nodes[n.rightChild]);
	}
	else {
		auto idx = n.rightChild & 0b0111111111111111;
		auto sub = subsectors[idx];
		for (int i = sub.firstSegNum; i < sub.firstSegNum + sub.segCount; i++) {
			auto seg = segments[i];
			auto start_v = vertices[seg.startnum];
			auto end_v = vertices[seg.endnum];
			cout << start_v.x << " " << end_v.x << '\n';
			lines.push_back(make_shared<Line>(this, 
				float(start_v.x) / float(dims.first), float(start_v.y) / float(dims.second),
				float(end_v.x) / float(dims.first), float(end_v.y) / float(dims.second),
				r, g, b));
		}
	}
}

Doom::Doom(HINSTANCE instance, int nCmdShow, int width, int height) {
	CreateWindowHandle(hInstance, nCmdShow, width, height);
	InitDoomDX();
	CalcDeltaTime();
	mouse = make_shared<Mouse>(this);
	mouse->SetCaptureMouse(true);
	mouse->HideMouseCursor();
	camera = make_shared<Camera>(this);
	//square = new Square(this);
	shader = make_shared<Shader>(this, L"shaders.shader");
	if (!shader->IsValidShader()) {
		char err[32];
		strerror_s(err, errno);
		cout << err << '\n';
		exit(1);
	}
	shader->SetAsCurrentShader();

	ifstream wad("DOOM.WAD", ios::binary);

	Header header;
	wad.read((char *)&header, sizeof(Header));

	wad.seekg(header.infoOffset);

	LumpInfo lumpinfo;

	bool firstThing = false;
	bool firstLineDef = false;
	bool firstSideDef = false;
	bool firstVertices = false;
	bool firstSeg = false;
	bool firstSubSector = false;
	bool firstNode = false;
	bool firstSector = false;
	bool map = false;

	int i = 0;
	while (!wad.eof() && (!firstLineDef || !firstVertices || !firstThing || !firstSideDef || !firstSeg || !firstSubSector || !firstNode || !firstSector)) {
		wad.read((char *)&lumpinfo, sizeof(LumpInfo));
		if (wad.eof() && i + 1 < header.numLumps) {
			while (wad.eof()) {
				wad.clear();
				wad.seekg(1, SEEK_CUR);
			}
			wad.seekg(-1, SEEK_CUR);
			wad.read((char *)&lumpinfo, sizeof(LumpInfo));
		}
		string name(lumpinfo.name, 8);
		if (name.find("E1M1") != string::npos) {
			map = true;
		}
		if (map && !firstLineDef && name.find("LINEDEFS") != string::npos) {
			HandleWadSection(linedefs, lumpinfo, wad);
			firstLineDef = true;
		}
		if (map && !firstVertices && name.find("VERTEXES") != string::npos) {
			HandleWadSection(vertices, lumpinfo, wad);
			firstVertices = true;
		}
		if (map && !firstThing && name.find("THING") != string::npos) {
			HandleWadSection(things, lumpinfo, wad);
			firstThing = true;
		}
		if (map && !firstSideDef && name.find("SIDEDEF") != string::npos) {
			HandleWadSection(sidedefs, lumpinfo, wad);
			firstSideDef = true;
		}
		if (map && !firstSeg && name.find("SEGS") != string::npos) {
			HandleWadSection(segments, lumpinfo, wad);
			firstSeg = true;
		}
		if (map && !firstSubSector && name.find("SSECTORS") != string::npos) {
			HandleWadSection(subsectors, lumpinfo, wad);
			firstSubSector = true;
		}
		if (map && !firstNode && name.find("NODES") != string::npos) {
			HandleWadSection(nodes, lumpinfo, wad);
			firstNode = true;
		}
		if (map && !firstSector && name.find("SECTORS") != string::npos) {
			HandleWadSection(sectors, lumpinfo, wad);
			firstSector = true;
		}
		//info.push_back(lumpinfo);
		i++;
	}
	wad.close();

	auto dims = getMaxDimensions(vertices);
	RecurseOverNodes(nodes, dims, nodes[nodes.size() - 1]);
}

// Creates the handle to our window so we can create the DX11 instance.
// `_hInstance` should be the same one as passed to the program in WinMain
void Doom::CreateWindowHandle(HINSTANCE _hInstance, int nCmdShow, int width, int height) {
	HWND hWnd;
	WNDCLASSEX wc;

	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW; // Horizontal and vertical redraw
	wc.lpfnWndProc = WindowProc; // Window Procedure
	wc.hInstance = _hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszClassName = "WindowClass";

	RegisterClassEx(&wc);

	RECT wr = { 0, 0, width, height };
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

	hWnd = CreateWindowEx(NULL,
		"WindowClass",
		"Doom DX", // Title
		WS_OVERLAPPEDWINDOW,
		300, // x pos
		300, // y pos
		wr.right - wr.left,
		wr.bottom - wr.top,
		NULL,
		NULL,
		_hInstance,
		NULL);

	ShowWindow(hWnd, nCmdShow);

	ScreenWidth = width;
	ScreenHeight = height;

	hInstance = _hInstance;
	window = hWnd;
}

void Doom::InitDoomDX() {
	// TODO: Handle errors better using HRESULT
	// TODO: Disallow calling this if window creation fails

	// Direct3D initialisation
	// describe our buffer
	DXGI_MODE_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(DXGI_MODE_DESC));

	bufferDesc.Width = ScreenWidth;
	bufferDesc.Height = ScreenHeight;
	bufferDesc.RefreshRate.Numerator = 60;
	bufferDesc.RefreshRate.Denominator = 1;
	bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// describe our swap chain
	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	// fill the swap chain description struct
	scd.BufferDesc = bufferDesc;
	scd.SampleDesc.Count = 1;
	scd.SampleDesc.Quality = 0;
	scd.BufferCount = 1;									// one back buffer
	scd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;		// use 32-bit color
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;		// how the swap chain is the be used
	scd.OutputWindow = window;								// the window to be used
	scd.SampleDesc.Count = 4;								// how many multisamples
	scd.Windowed = TRUE;									// windowed/full-screen mode
	scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// create a device, device context and swap chain using the information in the scd struct
	D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		D3D11_CREATE_DEVICE_DEBUG,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&scd,
		&swapchain,
		&dev,
		NULL,
		&devcon);

	// Set the render target
	// get the address of the back buffer
	ID3D11Texture2D *pBackBuffer;
	swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

	if (!pBackBuffer) {
		throw runtime_error("Failed to allocate the back buffer");
	}
	// use the back buffer address to create the render target
	dev->CreateRenderTargetView(pBackBuffer, NULL, &backbuffer);
	pBackBuffer->Release();

	// Describe Depth/Stencil Buffer
	D3D11_TEXTURE2D_DESC dsd;
	ZeroMemory(&dsd, sizeof(D3D11_TEXTURE2D_DESC));

	dsd.Width = ScreenWidth;
	dsd.Height = ScreenHeight;
	dsd.MipLevels = 1;
	dsd.ArraySize = 1;
	dsd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsd.SampleDesc.Count = 4;
	dsd.SampleDesc.Quality = 0;
	dsd.Usage = D3D11_USAGE_DEFAULT;
	dsd.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	dsd.CPUAccessFlags = 0;
	dsd.MiscFlags = 0;

	dev->CreateTexture2D(&dsd, NULL, &depthStencilBuffer);
	
	if (!depthStencilBuffer) {
		throw runtime_error("Failed to create depth stencil buffer");
	}
	dev->CreateDepthStencilView(depthStencilBuffer, NULL, &depthStencilView);

	devcon->OMSetRenderTargets(1, &backbuffer, depthStencilView);

	// Set the viewport
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = (FLOAT)ScreenWidth;
	viewport.Height = (FLOAT)ScreenHeight;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	devcon->RSSetViewports(1, &viewport);

	// Createbuffer to send to the cbuffer in effect file
	D3D11_BUFFER_DESC cbbd;
	ZeroMemory(&cbbd, sizeof(D3D11_BUFFER_DESC));

	cbbd.Usage = D3D11_USAGE_DEFAULT;
	cbbd.ByteWidth = sizeof(cbPerObject);
	cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbbd.CPUAccessFlags = 0;
	cbbd.MiscFlags = 0;

	dev->CreateBuffer(&cbbd, NULL, &cbPerObjectBuffer);
}

// Call this once per frame to update clock and delta time
void Doom::CalcDeltaTime() {
	if (previousTime) {
		clock_t now = clock();
		deltaTime = float(now - previousTime);
		previousTime = now;
		return;
	}
	previousTime = clock();
}

void Doom::Render() {
	static FLOAT clearColor[4] = { 0.0f, 0.2f, 0.4f, 1.0f };
	// clear the back buffer to a deep blue
	devcon->ClearRenderTargetView(backbuffer, clearColor);
	devcon->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	{
		for (const auto& line : lines) {
			line->Draw();
		}
		//square->Draw();
	}
	// switch the back buffer and the front buffer
	swapchain->Present(0, 0);
}

void Doom::Update() {
	CalcDeltaTime();
	mouse->Update();
	camera->Update(mouse);
	HandleKeyboardInput();
}

void Doom::HandleKeyboardInput() {
	static float MoveAmount = 0.00001f;
	if (KeyDown('A')) {
		camera->LocalMoveLeftOrRight(-MoveAmount * deltaTime);
	}
	else if (KeyDown('D')) {
		camera->LocalMoveLeftOrRight(MoveAmount * deltaTime);
	}

	if (KeyDown('W')) {
		camera->LocalMoveForwardOrBack(MoveAmount * deltaTime);
	}
	else if (KeyDown('S')) {
		camera->LocalMoveForwardOrBack(-MoveAmount * deltaTime);
	}

	if (KeyDown(VK_ESCAPE)) {
		PostQuitMessage(0);
	}
}