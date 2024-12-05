#pragma once
#include "SceneSystem.h"
#include "SysPhysics.h"

class SysPlayerBlinkParticles : public SceneSystem
{
public:
	SysPlayerBlinkParticles(Scene& scene) : SceneSystem(scene) {}

	void Update(float timestep, float time) override;
};
