#include "stdafx.h"
#include "SysButtonHandler.h"
#include "App/app.h"
#include "EntityGroup.h"

#include "CmpPosition.h"
#include "CmpShapeAabb.h"
#include "CmpShapeCircle.h"
#include "CmpButton.h"
#include "CmpOverlay.h"

static bool PointIntersects(EntityGroup& e, uint32_t eIndex, float2 point) {
	auto& pos = e.GetComponent<CmpPosition>(eIndex);
	if (e.HasComponent<CmpShapeAabb>(eIndex)) {
		auto& shp = e.GetComponent<CmpShapeAabb>(eIndex);
		auto min = pos.value - shp.extents;
		auto max = pos.value + shp.extents;
		if (point.x >= min.x && point.y >= min.y && point.x <= max.x && point.y <= max.y) {
			return true;
		}
	}
	if (e.HasComponent<CmpShapeCircle>(eIndex)) {
		auto& shp = e.GetComponent<CmpShapeCircle>(eIndex);
		if (float2::lengthSq(pos.value - point) <= (shp.radius * shp.radius)) {
			return true;
		}
	}
	return false;
}
static void ProcessButton(Scene& scene, EntityGroup& e, uint32_t eIndex, float2 p, bool pressed, bool pressedNow) {
	//Update button states.
	auto& btn = e.GetComponent<CmpButton>(eIndex);
	btn.isEnteredPrev = btn.isEntered;
	btn.isPressedPrev = btn.isPressed;
	btn.isEntered = PointIntersects(e, eIndex, p);
	if (pressedNow) {
		btn.isPressed = btn.isEntered;
	}
	else if (!pressed) {
		btn.isPressed = false;
	}
	//Dispatch button events.
	if (btn.isPressed != btn.isPressedPrev) {
		if (btn.isPressed) {
			if (btn.onPressed) {
				btn.onPressed(scene, e.GetEntity(eIndex));
			}
		}
		else {
			if (btn.onReleased) {
				btn.onReleased(scene, e.GetEntity(eIndex));
			}
		}
	}
}

void SysButtonHandler::Render()
{
	//Query mouse input state.
	auto pressed = App::IsKeyPressed(VK_LBUTTON);
	auto pressedNow = pressed && !pressedPrev;
	pressedPrev = pressed;
	//Process world buttons.
	{
		auto p = scene.GetWorldMousePos();
		auto r = scene.entities.GetComponentMask<CmpButton>()
			| scene.entities.GetComponentMask<CmpPosition>();
		auto f = scene.entities.GetComponentMask<CmpOverlay>();
		auto e = scene.entities.GetEntities(r, f);
		for (uint32_t i = 0; i < e.GetEntityCount(); ++i)
			ProcessButton(scene, e, i, p, pressed, pressedNow);
	}
	//Process overlay buttons.
	{
		auto p = scene.GetViewMousePos();
		auto r = scene.entities.GetComponentMask<CmpButton>()
			| scene.entities.GetComponentMask<CmpPosition>()
			| scene.entities.GetComponentMask<CmpOverlay>();
		auto e = scene.entities.GetEntities(r, 0);
		for (uint32_t i = 0; i < e.GetEntityCount(); ++i)
			ProcessButton(scene, e, i, p, pressed, pressedNow);
	}
}
