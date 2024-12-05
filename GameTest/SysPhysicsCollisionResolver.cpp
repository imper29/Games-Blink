#include "stdafx.h"
#include "SysPhysicsCollisionResolver.h"

#include "CmpMaterial.h"
#include "CmpPosition.h"
#include "CmpVelocity.h"
#include "CmpMass.h"

static void Solve(EntityRegistry& reg, const CmpMaterial& mat, const EvtCollision& col, Entity a) {
	auto& av = reg.GetComponent<CmpVelocity>(a);
	auto& ap = reg.GetComponent<CmpPosition>(a);

	av.value = av.value - col.normal * ((1.0f + mat.bounciness) * fmaxf(float2::dot(av.value, col.normal), 0.0f));
	float2 perp = float2(-col.normal.y, col.normal.x);
	av.value = av.value - perp * (mat.friction * float2::dot(av.value, perp));
	ap.value = ap.value - col.normal * col.depth;
}
static void Solve(EntityRegistry& reg, const CmpMaterial& mat, const EvtCollision& col, Entity a, Entity b) {
	auto& av = reg.GetComponent<CmpVelocity>(a);
	auto& ap = reg.GetComponent<CmpPosition>(a);
	auto& bv = reg.GetComponent<CmpVelocity>(b);
	auto& bp = reg.GetComponent<CmpPosition>(b);
	auto am = reg.GetComponentOrDefault<CmpMass>(a, CmpMass(1)).value;
	auto bm = reg.GetComponentOrDefault<CmpMass>(b, CmpMass(1)).value;

	float tm = am + bm;
	float2 relativeVelocity = av.value - bv.value;
	float v = fmaxf(float2::dot(relativeVelocity, col.normal), 0.0f);
	float2 perp = float2(-col.normal.y, col.normal.x);
	float f = float2::dot(relativeVelocity, perp);
	float2 avv = av.value - col.normal * ((1.0f + mat.bounciness) * v * bm / tm) - perp * (mat.friction * f * bm / tm);
	float2 bvv = bv.value + col.normal * ((1.0f + mat.bounciness) * v * am / tm) + perp * (mat.friction * f * am / tm);

	av.value = avv;
	bv.value = bvv;
	ap.value = ap.value - col.normal * col.depth * 0.5f;
	bp.value = bp.value + col.normal * col.depth * 0.5f;
}

void SysPhysicsCollisionResolver::Update(float time, float timestep)
{
	for (auto& c : physics.collisionEvents) {
		auto mat = CmpMaterial::GetMaterial(scene.entities, c.a, c.b);
		if (!scene.entities.Exists(c.a) || !scene.entities.Exists(c.b))
			continue;

		if (scene.entities.HasComponent<CmpVelocity>(c.a) && scene.entities.HasComponent<CmpPosition>(c.a)) {
			if (scene.entities.HasComponent<CmpVelocity>(c.b) && scene.entities.HasComponent<CmpPosition>(c.b)) {
				Solve(scene.entities, mat, c, c.a, c.b);
			}
			else {
				Solve(scene.entities, mat, c, c.a);
			}
		}
		else if (scene.entities.HasComponent<CmpVelocity>(c.b) && scene.entities.HasComponent<CmpPosition>(c.b)) {
			EvtCollision col = c;
			col.normal = -col.normal;
			Solve(scene.entities, mat, col, c.b);
		}
	}
}
