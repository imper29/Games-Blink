#include "stdafx.h"
#include "SysPhysicsCollisionCollector.h"

#include "CmpPosition.h"
#include "CmpCollider.h"
#include "CmpShapeAabb.h"
#include "CmpShapeCircle.h"

struct Aabb {
	float2 min, max;
};
struct Circle {
	float2 pos;
	float radius;
};

static float2 clamp(float2 min, float2 max, float2 pos) {
	return float2(fmaxf(fminf(pos.x, max.x), min.x), fmaxf(fminf(pos.y, max.y), min.y));
}
static bool Intersects(const Aabb& a, const Aabb& b, float2& norm, float& depth, float2& point) {
	if ((a.max.x > b.min.x) &&
		(a.min.x < b.max.x) &&
		(a.max.y > b.min.y) &&
		(a.min.y < b.max.y)) {

		Aabb i = Aabb{
			float2(fmaxf(a.min.x, b.min.x), fmaxf(a.min.y, b.min.y)),
			float2(fminf(a.max.x, b.max.x), fminf(a.max.y, b.max.y))
		};

		float2 s = i.max - i.min;
		float2 ac = (a.max + a.min) * 0.5f;
		float2 bc = (b.max + b.min) * 0.5f;
		if (s.x > s.y) {
			//Norm is vertical.
			if (ac.y > bc.y)
				norm = float2(0, -1);
			else
				norm = float2(0, 1);
			depth = i.max.y - i.min.y;
		}
		else {
			//Norm is horizontal.
			if (ac.x > bc.x)
				norm = float2(-1, 0);
			else
				norm = float2(1, 0);
			depth = i.max.x - i.min.x;
		}
		point = (i.max + i.min) * 0.5f;
		return true;
	}
	return false;
}
static bool Intersects(const Aabb& a, const Circle& b, float2& norm, float& depth, float2& point) {
	float2 p = clamp(a.min, a.max, b.pos);
	float2 o = b.pos - p;
	if (float2::lengthSq(o) < b.radius * b.radius) {
		norm = float2::normalize(o);
		depth = b.radius - float2::length(o);
		point = b.pos + norm * b.radius;
		return true;
	}
	return false;
}
static bool Intersects(const Circle& a, const Circle& b, float2& norm, float& depth, float2& point) {
	float2 o = b.pos - a.pos;
	if (float2::length(o) < (a.radius + b.radius)) {
		norm = float2::normalize(o);
		depth = a.radius + b.radius - float2::length(o);
		point = b.pos + norm * b.radius;
		return true;
	}
	return false;
}

static void CollectAabbAabb(EntityRegistry& entities, SysPhysics& physics) {
	auto r = entities.GetComponentMask<CmpPosition>()
		| entities.GetComponentMask<CmpCollider>()
		| entities.GetComponentMask<CmpShapeAabb>();
	auto e = entities.GetEntities(r, 0);

	EvtTrigger trigger;
	EvtCollision collision;
	for (uint32_t i = 0; i < e.GetEntityCount(); ++i) {
		auto& pos = e.GetComponent<CmpPosition>(i);
		auto& col = e.GetComponent<CmpCollider>(i);
		auto& shp = e.GetComponent<CmpShapeAabb>(i);
		for (uint32_t j = i + 1; j < e.GetEntityCount(); ++j) {
			auto& opos = e.GetComponent<CmpPosition>(j);
			auto& ocol = e.GetComponent<CmpCollider>(j);
			auto& oshp = e.GetComponent<CmpShapeAabb>(j);

			Aabb abox = Aabb{ pos.value - shp.extents, pos.value + shp.extents };
			Aabb bbox = Aabb{ opos.value - oshp.extents, opos.value + oshp.extents };
			if (Intersects(abox, bbox, collision.normal, collision.depth, collision.point)) {
				if (col.maskCollision & ocol.maskCollision) {
					collision.a = e.GetEntity(i);
					collision.b = e.GetEntity(j);
					physics.collisionEvents.push_back(collision);
				}
				if (col.maskTrigger & ocol.maskTrigger) {
					trigger.a = e.GetEntity(i);
					trigger.b = e.GetEntity(j);
					trigger.mask = col.maskTrigger & ocol.maskTrigger;
					physics.triggerEvents.push_back(trigger);
				}
			}
		}
	}
}
static void CollectAabbCircle(EntityRegistry& entities, SysPhysics& physics) {
	auto raabb = entities.GetComponentMask<CmpPosition>()
		| entities.GetComponentMask<CmpCollider>()
		| entities.GetComponentMask<CmpShapeAabb>();
	auto rcircle = entities.GetComponentMask<CmpPosition>()
		| entities.GetComponentMask<CmpCollider>()
		| entities.GetComponentMask<CmpShapeCircle>();
	auto eaabb = entities.GetEntities(raabb, 0);
	auto ecircle = entities.GetEntities(rcircle, 0);

	EvtTrigger trigger;
	EvtCollision collision;
	for (uint32_t i = 0; i < eaabb.GetEntityCount(); ++i) {
		auto& pos = eaabb.GetComponent<CmpPosition>(i);
		auto& col = eaabb.GetComponent<CmpCollider>(i);
		auto& shp = eaabb.GetComponent<CmpShapeAabb>(i);
		for (uint32_t j = 0; j < ecircle.GetEntityCount(); ++j) {
			auto& opos = ecircle.GetComponent<CmpPosition>(j);
			auto& ocol = ecircle.GetComponent<CmpCollider>(j);
			auto& oshp = ecircle.GetComponent<CmpShapeCircle>(j);

			Aabb abox = Aabb{ pos.value - shp.extents, pos.value + shp.extents };
			Circle bcircle = Circle{ opos.value, oshp.radius };
			if (Intersects(abox, bcircle, collision.normal, collision.depth, collision.point)) {
				if (col.maskCollision & ocol.maskCollision) {
					collision.a = eaabb.GetEntity(i);
					collision.b = ecircle.GetEntity(j);
					physics.collisionEvents.push_back(collision);
				}
				if (col.maskTrigger & ocol.maskTrigger) {
					trigger.a = eaabb.GetEntity(i);
					trigger.b = ecircle.GetEntity(j);
					trigger.mask = col.maskTrigger & ocol.maskTrigger;
					physics.triggerEvents.push_back(trigger);
				}
			}
		}
	}
}
static void CollectCircleCircle(EntityRegistry& entities, SysPhysics& physics) {
	auto r = entities.GetComponentMask<CmpPosition>()
		| entities.GetComponentMask<CmpCollider>()
		| entities.GetComponentMask<CmpShapeCircle>();
	auto e = entities.GetEntities(r, 0);

	EvtTrigger trigger;
	EvtCollision collision;
	for (uint32_t i = 0; i < e.GetEntityCount(); ++i) {
		auto& pos = e.GetComponent<CmpPosition>(i);
		auto& col = e.GetComponent<CmpCollider>(i);
		auto& shp = e.GetComponent<CmpShapeCircle>(i);
		for (uint32_t j = i + 1; j < e.GetEntityCount(); ++j) {
			auto& opos = e.GetComponent<CmpPosition>(j);
			auto& ocol = e.GetComponent<CmpCollider>(j);
			auto& oshp = e.GetComponent<CmpShapeCircle>(j);

			Circle acircle = { pos.value, shp.radius };
			Circle bcircle = { opos.value, oshp.radius };
			if (Intersects(acircle, bcircle, collision.normal, collision.depth, collision.point)) {
				if (col.maskCollision & ocol.maskCollision) {
					collision.a = e.GetEntity(i);
					collision.b = e.GetEntity(j);
					physics.collisionEvents.push_back(collision);
				}
				if (col.maskTrigger & ocol.maskTrigger) {
					trigger.a = e.GetEntity(i);
					trigger.b = e.GetEntity(j);
					trigger.mask = col.maskTrigger & ocol.maskTrigger;
					physics.triggerEvents.push_back(trigger);
				}
			}
		}
	}
}

void SysPhysicsCollisionCollector::Update(float time, float timestep)
{
	physics.triggerEvents.clear();
	physics.collisionEvents.clear();
	CollectAabbAabb(scene.entities, physics);
	CollectAabbCircle(scene.entities, physics);
	CollectCircleCircle(scene.entities, physics);
}
