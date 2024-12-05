#include "stdafx.h"
#include "SysPlayerScoreRenderer.h"

#include "CmpPlayer.h"
#include "CmpPlayerScoreRenderer.h"

void SysPlayerScoreRenderer::Render()
{
	auto r = scene.entities.GetComponentMask<CmpPlayerScoreRenderer>();
	auto e = scene.entities.GetEntities(r, 0);
	for (uint32_t i = 0; i < e.GetEntityCount(); ++i) {
		auto& psr = e.GetComponent<CmpPlayerScoreRenderer>(i);
		if (scene.entities.Exists(psr.player) && scene.entities.HasComponent<CmpPlayer>(psr.player)) {
			auto& plr = scene.entities.GetComponent<CmpPlayer>(psr.player);
			snprintf(psr.text, psr.length, "%02d", plr.score);
		}
	}
}
