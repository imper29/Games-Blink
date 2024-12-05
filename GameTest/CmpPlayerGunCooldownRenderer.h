#pragma once
#include "Entity.h"

struct CmpPlayerGunCooldownRenderer
{
	Entity player;
	float left;
	float right;
	float4 color;

	CmpPlayerGunCooldownRenderer(const Entity& player, const float left, const float right, const float4& color)
		: player(player)
		, left(left)
		, right(right)
		, color(color)
	{
	}
};
