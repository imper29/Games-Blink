#include "stdafx.h"
#include "Gamepad.h"
#include "GLFW/glfw3.h"

GLFWgamepadstate Gamepad::gamepads[4];

void Gamepad::Init()
{
	glfwInit();
}
void Gamepad::Update()
{
	for (int i = 0; i < 4; ++i) {
		glfwGetGamepadState(i, &gamepads[i]);
	}
}
