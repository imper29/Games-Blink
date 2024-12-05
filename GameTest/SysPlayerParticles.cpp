#include "stdafx.h"
#include "SysPlayerParticles.h"
#include "App/AppSettings.h"

#include "CmpColor.h"
#include "CmpPosition.h"
#include "CmpVelocity.h"
#include "CmpShapePoint.h"
#include "CmpShapeAabb.h"
#include "CmpPlayerCharacter.h"
#include "CmpSelfDestructTime.h"
#include "CmpGravityScale.h"

static float2 randRange(float2 min, float2 max) {
	return float2(FRAND_RANGE(min.x, max.x), FRAND_RANGE(min.y, max.y));
}
static void GenerateParticles(EntityRegistry& reg, EvtCollision& c, Entity e, float time) {
	if (!reg.HasComponent<CmpVelocity>(e) || !reg.HasComponent<CmpPlayerCharacter>(e) || !reg.HasComponent<CmpShapeAabb>(e))
		return;
	
	auto& ext = reg.GetComponent<CmpShapeAabb>(e);
	auto& pos = reg.GetComponent<CmpPosition>(e);
	auto& vel = reg.GetComponent<CmpVelocity>(e);
	
	auto off = -c.normal * ext.extents;
	auto size = float2(c.normal.y, c.normal.x) * ext.extents;

	int count = (int)fminf(fmaxf((float2::length(vel.value) * 0.5f), c.depth * 1000.0f), 10);
	for (int i = 0; i < count; ++i) {
		Entity p = reg.CreateEntity();
		reg.AddComponent<CmpShapePoint>(p);
		reg.AddComponent<CmpSelfDestructTime>(p, time + FRAND_RANGE(0.2f, 0.3f));
		reg.AddComponent<CmpGravityScale>(p, 0.0f);
		reg.AddComponent<CmpPosition>(p, pos.value + off + randRange(-size, size));
		reg.AddComponent<CmpVelocity>(p, randRange(float2(-1.0f, -1.0f), float2(1.0f, 1.0f)));
		reg.AddComponent<CmpColor>(p, reg.GetComponentOrDefault<CmpColor>(e, CmpColor(float4(1, 1, 1, 1)).value)).value.w = 0.3f;
	}
}

void SysPlayerParticles::Update(float time, float timestep)
{
	for (auto& c : physics.collisionEvents) {
		GenerateParticles(scene.entities, c, c.a, time);
		GenerateParticles(scene.entities, c, c.b, time);
	}
}
