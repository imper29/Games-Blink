#pragma once
#include "SysPhysics.h"
#include "SceneSystem.h"

class SysDeathTriggerHandler : public SceneSystem
{
public:
	SysDeathTriggerHandler(Scene& scene) : SceneSystem(scene), physics(scene.AddOrGetSystem<SysPhysics>()) {}

	virtual void Update(float time, float timestep);
private:
	SysPhysics& physics;
};
