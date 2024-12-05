#pragma once
#include "SceneSystem.h"

class SysDrawCircles : public SceneSystem
{
public:
	SysDrawCircles(Scene& scene) : SceneSystem(scene) {}

	virtual void Render();
	virtual void RenderOverlay();
};
