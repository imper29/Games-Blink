#include "stdafx.h"
#include "MenuControls.h"
#include "SceneManager.h"
#include "MenuMain.h"

static void BtnBack(Scene& scene, Entity button) {
	SceneManager::LoadScene(std::unique_ptr<Scene>(new MenuMain()));
}
static void CreateControl(Menu& menu, const char* name, const char* control, int index, const float2& position, const float2& size, const float2& h, const float2& v) {
	menu.CreateLabel(position + v * index + h * -1.0f, size, name);
	menu.CreateLabel(position + v * index + h * 0.5f, size + h * 0.5f, control);
}

MenuControls::MenuControls()
{
	float2 position = float2(50.0f, 42.5f);
	float2 size = float2(10.0f, 2.5f);
	float2 h = float2(20.0f, 0.0f);
	float2 v = float2(0.0f, 5.0f);

	CreateControl(*this, "Aim  ", "Right Stick  ", 5, position, size, h, v);
	CreateControl(*this, "Walk ", "Left Stick   ", 4, position, size, h, v);
	CreateControl(*this, "Jump ", "Left Bumper  ", 3, position, size, h, v);
	CreateControl(*this, "Blink", "Right Bumper ", 2, position, size, h, v);
	CreateControl(*this, "Shoot", "Right Trigger", 1, position, size, h, v);
	CreateButton(position, size + h, "Back", BtnBack);
}
