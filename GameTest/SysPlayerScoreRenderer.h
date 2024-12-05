#pragma once
#include "SceneSystem.h"

class SysPlayerScoreRenderer : public SceneSystem
{
public:
	SysPlayerScoreRenderer(Scene& scene) : SceneSystem(scene) {}

	void Render() override;
};
