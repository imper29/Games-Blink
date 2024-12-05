#pragma once
#include "Entity.h"

struct CmpPlayerBullet
{
	Entity player;

	CmpPlayerBullet(const Entity& player)
		: player(player)
	{
	}
};
