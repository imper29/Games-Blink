#pragma once
#include "SysPhysics.h"
#include "SceneSystem.h"

class SysPhysicsCollisionCollector : public SceneSystem
{
public:
	SysPhysicsCollisionCollector(Scene& scene) : SceneSystem(scene), physics(scene.AddOrGetSystem<SysPhysics>()) {}

	virtual void Update(float time, float timestep);
private:
	SysPhysics& physics;
};
