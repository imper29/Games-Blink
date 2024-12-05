#pragma once
#include "SceneSystem.h"

class SysButtonHandler : public SceneSystem
{
public:
	SysButtonHandler(Scene& scene) : SceneSystem(scene), pressedPrev(false) {}

	virtual void Render();
private:
	bool pressedPrev;
};
