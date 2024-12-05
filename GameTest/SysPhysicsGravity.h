#pragma once
#include "SceneSystem.h"
class SysPhysicsGravity : public SceneSystem
{
public:
	SysPhysicsGravity(Scene& scene) : SceneSystem(scene), gravity(0.0f, -20.0f) {}

	void Update(float time, float timestep) override;

	float2 gravity;
};
