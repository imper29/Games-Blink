#include "stdafx.h"
#include "SysPhysicsGravity.h"
#include "CmpVelocity.h"
#include "CmpGravityScale.h"

void SysPhysicsGravity::Update(float time, float timestep)
{
	auto g = gravity * timestep;
	{
		auto r = scene.entities.GetComponentMask<CmpVelocity>();
		auto f = scene.entities.GetComponentMask<CmpGravityScale>();
		auto e = scene.entities.GetEntities(r, f);
		for (uint32_t i = 0; i < e.GetEntityCount(); ++i) {
			auto& vel = e.GetComponent<CmpVelocity>(i);
			vel.value = vel.value + g;
		}
	}
	{
		auto r = scene.entities.GetComponentMask<CmpVelocity>()
			   | scene.entities.GetComponentMask<CmpGravityScale>();
		auto e = scene.entities.GetEntities(r, 0);
		for (uint32_t i = 0; i < e.GetEntityCount(); ++i) {
			auto& vel = e.GetComponent<CmpVelocity>(i);
			auto& scl = e.GetComponent<CmpGravityScale>(i);
			vel.value = vel.value + g * scl.scale;
		}
	}
}
