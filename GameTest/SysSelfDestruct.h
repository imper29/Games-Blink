#pragma once
#include "SceneSystem.h"

class SysSelfDestruct : public SceneSystem
{
public:
	SysSelfDestruct(Scene& scene) : SceneSystem(scene) {}

	void Update(float time, float timestep) override;
};
