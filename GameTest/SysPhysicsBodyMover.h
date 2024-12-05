#pragma once
#include "SceneSystem.h"

class SysPhysicsBodyMover : public SceneSystem
{
public:
	SysPhysicsBodyMover(Scene& scene) : SceneSystem(scene) {}

	virtual void Update(float time, float timestep);
};
