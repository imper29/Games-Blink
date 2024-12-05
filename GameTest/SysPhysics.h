#pragma once
#include "SceneSystem.h"

struct EvtTrigger {
	Entity a, b;
	int mask;
};
struct EvtCollision {
	Entity a, b;
	float2 point;
	float2 normal;
	float depth;
};

class SysPhysics : public SceneSystem
{
public:
	SysPhysics(Scene& scene) : SceneSystem(scene) {}

	std::vector<EvtTrigger> triggerEvents;
	std::vector<EvtCollision> collisionEvents;
};
