#include "stdafx.h"
#include "SysTimedEventHandler.h"
#include "CmpTimedEvent.h"

void SysTimedEventHandler::Update(float time, float timestep)
{
	std::vector<Entity> toDestroy;
	auto r = scene.entities.GetComponentMask<CmpTimedEvent>();
	auto e = scene.entities.GetEntities(r, 0);
	for (uint32_t i = 0; i < e.GetEntityCount(); ++i) {
		auto& evt = e.GetComponent<CmpTimedEvent>(i);
		if (evt.time <= time) {
			evt.time = evt.callback(scene, e.GetEntity(i), evt.time, evt.period);
			if (evt.time == -1.0f)
				toDestroy.push_back(e.GetEntity(i));
		}
	}
	for (auto& entity : toDestroy)
		scene.entities.DestroyEntity(entity);
}
