#pragma once
#include "SceneSystem.h"
#include "SysPhysics.h"

class SysPlayerBlinkController : public SceneSystem
{
public:
	SysPlayerBlinkController(Scene& scene) : SceneSystem(scene), physics(scene.AddOrGetSystem<SysPhysics>()) {}

	virtual void Update(float time, float timestep);

	SysPhysics& physics;
};
