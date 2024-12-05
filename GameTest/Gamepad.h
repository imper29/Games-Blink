#pragma once
#include "GLFW/glfw3.h"

struct Gamepad
{
	static GLFWgamepadstate gamepads[4];

	static void Init();
	static void Update();
};
