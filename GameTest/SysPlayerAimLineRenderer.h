#pragma once
#include "SceneSystem.h"

class SysPlayerAimLineRenderer : public SceneSystem
{
public:
	SysPlayerAimLineRenderer(Scene& scene) : SceneSystem(scene) {}

	void Render() override;
};
