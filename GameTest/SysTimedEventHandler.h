#pragma once
#include "SceneSystem.h"

class SysTimedEventHandler : public SceneSystem
{
public:
	SysTimedEventHandler(Scene& scene) : SceneSystem(scene) {}

	virtual void Update(float time, float timestep);
};
