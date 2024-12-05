#pragma once
#include "SceneSystem.h"
#include "SysPhysics.h"

class SysPlayerParticles : public SceneSystem
{
public:
	SysPlayerParticles(Scene& scene) : SceneSystem(scene), physics(scene.AddOrGetSystem<SysPhysics>()) {}

	void Update(float timestep, float time) override;
private:
	SysPhysics& physics;
};
