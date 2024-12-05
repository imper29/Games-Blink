#pragma once
#include "SceneSystem.h"
#include "SysPhysics.h"

class SysPlayerMovementController : public SceneSystem
{
public:
	SysPlayerMovementController(Scene& scene) : SceneSystem(scene), physics(scene.AddOrGetSystem<SysPhysics>()) {}

	void Update(float time, float timestep) override;

	SysPhysics& physics;
};
