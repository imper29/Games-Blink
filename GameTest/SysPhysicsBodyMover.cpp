#include "stdafx.h"
#include "SysPhysicsBodyMover.h"
#include "EntityGroup.h"

#include "CmpPosition.h"
#include "CmpVelocity.h"

void SysPhysicsBodyMover::Update(float time, float timestep)
{
	auto halfTimestepSquared = timestep * timestep * 0.5f;

	auto r = scene.entities.GetComponentMask<CmpPosition>()
		   | scene.entities.GetComponentMask<CmpVelocity>();
	auto e = scene.entities.GetEntities(r, 0);
	for (uint32_t i = 0; i < e.GetEntityCount(); ++i) {
		auto& pos = e.GetComponent<CmpPosition>(i);
		auto& vel = e.GetComponent<CmpVelocity>(i);

		pos.value = pos.value + vel.value * timestep;
	}
}
