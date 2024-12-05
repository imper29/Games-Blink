#pragma once
#include "SceneSystem.h"
#include "SysPhysics.h"

class SysPlayerGunController : public SceneSystem
{
public:
	SysPlayerGunController(Scene& scene) : SceneSystem(scene), physics(scene.AddOrGetSystem<SysPhysics>()) {}

	void Update(float time, float timestep) override;
private:
	SysPhysics& physics;
};
