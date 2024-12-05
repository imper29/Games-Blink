#include "stdafx.h"
#include "SysSelfDestruct.h"
#include "CmpSelfDestructTime.h"
#include "EntityGroup.h"

void SysSelfDestruct::Update(float time, float timestep)
{
	auto r = scene.entities.GetComponentMask<CmpSelfDestructTime>();
	auto e = scene.entities.GetEntities(r, 0);

	uint32_t count = 0;
	std::unique_ptr<Entity[]> entitiesToDestroy = std::make_unique<Entity[]>(e.GetEntityCount());
	for (uint32_t i = 0; i < e.GetEntityCount(); ++i) {
		auto& t = e.GetComponent<CmpSelfDestructTime>(i);
		if (t.time <= time) {
			entitiesToDestroy[count++] = e.GetEntity(i);
		}
	}
	for (uint32_t i = 0; i < count; ++i) {
		scene.entities.DestroyEntity(entitiesToDestroy[i]);
	}
}
