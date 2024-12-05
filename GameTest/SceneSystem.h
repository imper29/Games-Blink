#pragma once
#include "Scene.h"
#include "EntityGroup.h"

class SceneSystem
{
public:
	SceneSystem(Scene& scene) : scene(scene) {}
	virtual ~SceneSystem() {}

	virtual void Render() {}
	virtual void RenderOverlay() {}
	virtual void Update(float time, float timestep) {}

	Scene& scene;
};
