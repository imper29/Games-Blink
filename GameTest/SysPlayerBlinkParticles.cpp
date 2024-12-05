#include "stdafx.h"
#include "SysPlayerBlinkParticles.h"
#include "App/AppSettings.h"

#include "CmpColor.h"
#include "CmpPosition.h"
#include "CmpVelocity.h"
#include "CmpShapePoint.h"
#include "CmpPlayerBlink.h"
#include "CmpSelfDestructTime.h"
#include "CmpGravityScale.h"

static float2 randRange(float2 min, float2 max) {
	return float2(FRAND_RANGE(min.x, max.x), FRAND_RANGE(min.y, max.y));
}

static void GenerateParticles(EntityRegistry& reg, EntityGroup& grp, uint32_t i, float time) {
	if (FRAND < 0.8f)
		return;

	auto& pos = grp.GetComponent<CmpPosition>(i);
	auto& col = grp.GetComponent<CmpColor>(i);

	Entity p = reg.CreateEntity();
	reg.AddComponent<CmpShapePoint>(p);
	reg.AddComponent<CmpSelfDestructTime>(p, time + FRAND_RANGE(0.2f, 0.3f));
	reg.AddComponent<CmpGravityScale>(p, 0.0f);
	reg.AddComponent<CmpPosition>(p, pos.value + randRange(float2(-BLINK_RADIUS, -BLINK_RADIUS), float2(BLINK_RADIUS, BLINK_RADIUS)));
	reg.AddComponent<CmpVelocity>(p, randRange(float2(-1.0f, -1.0f), float2(1.0f, 1.0f)));
	reg.AddComponent<CmpColor>(p, col.value).value.w = 0.3f;
}

void SysPlayerBlinkParticles::Update(float time, float timestep)
{
	auto r = scene.entities.GetComponentMask<CmpColor>()
		   | scene.entities.GetComponentMask<CmpPosition>()
		   | scene.entities.GetComponentMask<CmpPlayerBlink>();
	auto e = scene.entities.GetEntities(r, 0);
	for (uint32_t i = 0; i < e.GetEntityCount(); ++i) {
		GenerateParticles(scene.entities, e, i, time);
	}
}
