#pragma once

#include "DoomDX.hpp"

// Class that represents the state and functionality of the mouse. It is created by the main Doom class
	// to mitigate the functionality of the mouse to a separate class. It is passed to the Camera class when it needs to update.
class Mouse {
private:
	// pointer to the main class
	Doom* game;
	// changes in x and y from last tick
	int dx, dy;

	// whether to capture the mouse
	bool captureMouse;
	// method used to center mouse on the window
	void centreMousePos();
	// method to update dx and dy
	void updateChangesInPosition();

public:
	// Default constructor
	Mouse(Doom* g);

	// Displays the mouse cursor
	void ShowMouseCursor();
	// Hides the mouse cursor
	void HideMouseCursor();

	// Updates the internals of the mouse class
	void Update();

	// Toggles the capturing of the mouse to keep or not to keep it in the centre of window
	void ToggleCaptureMouse();
	// Sets the capturing of the mouse to keep or not to keep it in the centre of window
	void SetCaptureMouse(bool capture);

	// Get the change in x since the last tick
	int GetXChange() const;
	// Get the change in y since the last tick
	int GetYChange() const;
};