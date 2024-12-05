#pragma once
#include "GLFW/glfw3.h"

struct Gamepad
{
	static Gamepad gamepads[4];

	float axes[GLFW_GAMEPAD_AXIS_LAST + 1];
	unsigned char buttons[GLFW_GAMEPAD_BUTTON_LAST + 1];

	static void Init();
	static void Update();
};
