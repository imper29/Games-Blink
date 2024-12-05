#pragma once
#include "Scene.h"

#include "CmpText.h"
#include "CmpButton.h"
#include "CmpOverlay.h"
#include "CmpColor.h"
#include "CmpPosition.h"
#include "CmpShapeAabb.h"

class Menu : public Scene
{
public:
	Menu();

	std::pair<Entity, Entity> CreateLabel(float2 center, float2 extents, const char* tex) {
		Entity btnBorder = entities.CreateEntity();
		entities.AddComponent<CmpOverlay>(btnBorder);
		entities.AddComponent<CmpText>(btnBorder, tex, 2.0f);
		entities.AddComponent<CmpColor>(btnBorder, float4(1.0f, 1.0f, 1.0f, 1.0f));
		entities.AddComponent<CmpPosition>(btnBorder, center);

		Entity btnCenter = entities.CreateEntity();
		entities.AddComponent<CmpOverlay>(btnCenter);
		entities.AddComponent<CmpColor>(btnCenter, float4(0.5f, 0.5f, 0.5f, 1.0f));
		entities.AddComponent<CmpShapeAabb>(btnCenter, extents - float2(0.25f, 0.25f));
		entities.AddComponent<CmpPosition>(btnCenter, center);
		return std::make_pair(btnBorder, btnCenter);
	}
	std::pair<Entity, Entity> CreateButton(float2 center, float2 extents, const char* tex, CmpButtonCallback onReleased) {
		Entity btnBorder = entities.CreateEntity();
		entities.AddComponent<CmpOverlay>(btnBorder);
		entities.AddComponent<CmpText>(btnBorder, tex, 2.0f);
		entities.AddComponent<CmpColor>(btnBorder, float4(1.0f, 1.0f, 1.0f, 1.0f));
		entities.AddComponent<CmpPosition>(btnBorder, center);

		Entity btnCenter = entities.CreateEntity();
		entities.AddComponent<CmpOverlay>(btnCenter);
		entities.AddComponent<CmpColor>(btnCenter, float4(1.0f, 1.0f, 1.0f, 1.0f));
		entities.AddComponent<CmpButton>(btnCenter, nullptr, onReleased);
		entities.AddComponent<CmpShapeAabb>(btnCenter, extents - float2(0.25f, 0.25f));
		entities.AddComponent<CmpPosition>(btnCenter, center);
		return std::make_pair(btnBorder, btnCenter);
	}
};
