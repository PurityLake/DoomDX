#include <bitset>
#include <fstream>
#include <vector>

#include "DoomDX.hpp"

#define WIDTH 800
#define HEIGHT 600

using namespace std;
using namespace DirectX;

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow) {
// DEBUG ONLY
// Allows for the creation of console in development mode
// Should NOT be enabled in release mode
#ifdef _DEBUG
	AllocConsole();
# ifdef _MSC_VER
#  pragma warning(push)
#  pragma warning(disable : 4996)
# endif
	FILE* f;
	f = freopen("CONOUT$", "w", stdout);
# ifdef _MSC_VER
#  pragma warning(pop)
# endif
#endif
	auto game = make_unique<Doom>(hInstance, nCmdShow, WIDTH, HEIGHT);
	// enter main loop
	MSG msg;
	while (TRUE) {

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			// May need more complex message handling later
			if (msg.message == WM_QUIT) {
				break;
			}
		}

		game->Update();
		game->Render();
	}

#ifdef _DEBUG
	if (f) fclose(f);
#endif

	return (int)msg.wParam;
}