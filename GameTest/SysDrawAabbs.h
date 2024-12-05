#pragma once
#include "SceneSystem.h"

class SysDrawAabbs : public SceneSystem
{
public:
	SysDrawAabbs(Scene& scene) : SceneSystem(scene) {}

	virtual void Render();
	virtual void RenderOverlay();
};
