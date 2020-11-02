#include "DoomDX.hpp"

using namespace std;

Mouse::Mouse(Doom* game) : game(game), captureMouse(true), dx(0.0f), dy(0.0f) {}

void Mouse::centreMousePos() {
	RECT rect = { NULL };
	if (GetWindowRect(game->window, &rect)) {
		SetCursorPos(rect.left + (int)(game->ScreenWidth / 2.0f),
			rect.top + (int)(game->ScreenHeight / 2.0f));
	}
}

void Mouse::ShowMouseCursor() {
	ShowCursor(TRUE);
}

void Mouse::HideMouseCursor() {
	ShowCursor(FALSE);
}

void Mouse::updateChangesInPosition() {
	POINT pos;
	if (GetCursorPos(&pos)) {
		RECT rect = { NULL };
		if (GetWindowRect(game->window, &rect)) {
			int cx = rect.left + (int)(game->ScreenWidth / 2.0f);
			int cy = rect.top + (int)(game->ScreenHeight / 2.0f);
			dx = pos.x - cx;
			dy = pos.y - cy;
		}
	}
}

void Mouse::Update() {
	if (captureMouse) {
		updateChangesInPosition();
		//centreMousePos();
	}
}

void Mouse::ToggleCaptureMouse() {
	captureMouse = !captureMouse;
}
void Mouse::SetCaptureMouse(bool capture) {
	captureMouse = capture;
}

int Mouse::GetXChange() const {
	return dx;
}

int Mouse::GetYChange() const {
	return dy;
}