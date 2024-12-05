#include "stdafx.h"
#include "Gamepad.h"
#include "GLFW/glfw3.h"

Gamepad Gamepad::gamepads[4];

void Gamepad::Init()
{
	glfwInit();
}
void Gamepad::Update()
{
	for (int i = 0; i < 4; ++i) {
		auto axesNum = 0;
		auto axes = glfwGetJoystickAxes(i, &axesNum);
		if (axes != nullptr)
			for (int j = 0; j < axesNum; ++j)
				gamepads[i].axes[j] = axes[j];

		auto buttonsNum = 0;
		auto buttons = glfwGetJoystickButtons(i, &buttonsNum);
		if (buttons != nullptr)
			for (int j = 0; j < buttonsNum; ++j)
				gamepads[i].buttons[j] = buttons[j];
	}
}
