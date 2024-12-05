#pragma once
#include "SysPhysics.h"
#include "SceneSystem.h"

class SysPhysicsCollisionResolver : public SceneSystem
{
public:
	SysPhysicsCollisionResolver(Scene& scene) : SceneSystem(scene), physics(scene.AddOrGetSystem<SysPhysics>()) {}

	virtual void Update(float time, float timestep);
private:
	SysPhysics& physics;
};
