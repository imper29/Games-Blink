#include "stdafx.h"
#include "SysCameraFree.h"
#include "App/app.h"

void SysCameraFree::Update(float time, float timestep) {
	for (int i = 0; i < MAX_CONTROLLERS; ++i) {
		auto c = App::GetController(i);
		scene.viewWorldPosition.x += scene.viewWorldHeight * c.GetLeftThumbStickX() * timestep * 2.0f;
		scene.viewWorldPosition.y += scene.viewWorldHeight * c.GetLeftThumbStickY() * timestep * 2.0f;
		scene.viewWorldHeight -= scene.viewWorldHeight * c.GetRightThumbStickY() * timestep * 3.0f;
	}
}
