#pragma once
#include "SceneSystem.h"

class SysCameraFree : public SceneSystem
{
public:
	SysCameraFree(Scene& scene) : SceneSystem(scene) {}

	virtual void Update(float time, float timestep);
};
