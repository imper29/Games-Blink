#pragma once
#include "SceneSystem.h"

class SysButtonColorizer : public SceneSystem
{
public:
	SysButtonColorizer(Scene& scene) : SceneSystem(scene) {}

	virtual void Render();
};
