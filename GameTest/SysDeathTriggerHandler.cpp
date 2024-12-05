#include "stdafx.h"
#include "SysDeathTriggerHandler.h"
#include "CmpDeath.h"
#include "CmpCollider.h"

void SysDeathTriggerHandler::Update(float time, float timestep)
{
	uint32_t count = 0;
	std::unique_ptr<Entity[]> entitiesToDestroy = std::make_unique<Entity[]>(physics.triggerEvents.size() * 2);
	for (auto& t : physics.triggerEvents) {
		if (scene.entities.Exists(t.a) && scene.entities.Exists(t.b)) {
			if (scene.entities.HasComponent<CmpDeath>(t.a)) {
				entitiesToDestroy[count++] = t.b;
			}
			if (scene.entities.HasComponent<CmpDeath>(t.b)) {
				entitiesToDestroy[count++] = t.a;
			}
		}
	}
	for (uint32_t i = 0; i < count; ++i) {
		scene.entities.DestroyEntity(entitiesToDestroy[i]);
	}
}
