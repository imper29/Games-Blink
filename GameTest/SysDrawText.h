#pragma once
#include "SceneSystem.h"

class SysDrawText : public SceneSystem
{
public:
	SysDrawText(Scene& scene) : SceneSystem(scene) {}

	virtual void Render();
	virtual void RenderOverlay();
};
