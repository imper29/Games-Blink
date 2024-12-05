#pragma once
#include "SceneSystem.h"

class SysPlayerGunCooldownRenderer : public SceneSystem
{
public:
	SysPlayerGunCooldownRenderer(Scene& scene) : SceneSystem(scene) {}

	void Render() override;
};
