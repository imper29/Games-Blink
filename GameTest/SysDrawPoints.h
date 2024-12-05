#pragma once
#include "SceneSystem.h"

class SysDrawPoints : public SceneSystem
{
public:
	SysDrawPoints(Scene& scene) : SceneSystem(scene) {}

	virtual void Render();
	virtual void RenderOverlay();
};
