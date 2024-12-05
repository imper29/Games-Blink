#pragma once
#include "SceneSystem.h"

class SysPlayerCharacterSpawner : public SceneSystem
{
public:
	SysPlayerCharacterSpawner(Scene& scene) : SceneSystem(scene) {}

	void Update(float time, float timestep) override;
};
