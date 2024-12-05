#include "stdafx.h"
#include "SysPlayerGunCooldownRenderer.h"

#include "CmpColor.h"
#include "CmpPosition.h"
#include "CmpShapeAabb.h"
#include "CmpPlayer.h"
#include "CmpPlayerCharacter.h"
#include "CmpPlayerGunCooldownRenderer.h"

void SysPlayerGunCooldownRenderer::Render()
{
	auto r = scene.entities.GetComponentMask<CmpPosition>()
		   | scene.entities.GetComponentMask<CmpColor>()
		   | scene.entities.GetComponentMask<CmpShapeAabb>()
		   | scene.entities.GetComponentMask<CmpPlayerGunCooldownRenderer>();
	auto e = scene.entities.GetEntities(r, 0);
	for (uint32_t i = 0; i < e.GetEntityCount(); ++i) {
		auto& col = e.GetComponent<CmpColor>(i);
		auto& pos = e.GetComponent<CmpPosition>(i);
		auto& shp = e.GetComponent<CmpShapeAabb>(i);
		auto& cgr = e.GetComponent<CmpPlayerGunCooldownRenderer>(i);
		if (scene.entities.Exists(cgr.player) && scene.entities.HasComponent<CmpPlayer>(cgr.player)) {
			auto& plr = scene.entities.GetComponent<CmpPlayer>(cgr.player);
			if (scene.entities.Exists(plr.character) && scene.entities.HasComponent<CmpPlayerCharacter>(plr.character)) {
				auto& chr = scene.entities.GetComponent<CmpPlayerCharacter>(plr.character);
				float charge = chr.gunCharge / chr.gunChargeLimit;
				float l = cgr.left;
				float r = cgr.left + (cgr.right - cgr.left) * charge;
				float c = (l + r) * 0.5f;
				pos.value.x = c;
				shp.extents.x = (r - l) * 0.5f;

				if (chr.gunOverheated)
					col.value = cgr.color * float4(0.3f, 0.3f, 0.3f, 1.0f);
				else
					col.value = cgr.color;
			}
		}
	}
}
