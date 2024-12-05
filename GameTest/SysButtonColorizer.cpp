#pragma once
#include "stdafx.h"
#include "SceneSystem.h"
#include "SysButtonColorizer.h"
#include "EntityGroup.h"

#include "CmpButton.h"
#include "CmpColor.h"

void SysButtonColorizer::Render()
{
	auto colNormal = float4(0.5f, 0.5f, 0.5f, 1);
	auto colHovered = float4(0.6f, 0.6f, 0.6f, 1);
	auto colPressed = float4(0.3f, 0.3f, 0.3f, 1);

	auto r = scene.entities.GetComponentMask<CmpButton>()
		   | scene.entities.GetComponentMask<CmpColor>();
	auto e = scene.entities.GetEntities(r, 0);
	for (uint32_t i = 0; i < e.GetEntityCount(); ++i) {
		auto& btn = e.GetComponent<CmpButton>(i);
		auto& col = e.GetComponent<CmpColor>(i);
		if (btn.isPressed) {
			col.value = colPressed;
		}
		else if (btn.isEntered) {
			col.value = colHovered;
		}
		else {
			col.value = colNormal;
		}
	}
}
