#pragma once
#include "SceneSystem.h"

class SysCameraFollowPlayers : public SceneSystem
{
public:
	SysCameraFollowPlayers(Scene& scene) : SceneSystem(scene) {}

	virtual void Update(float time, float timestep);
};
