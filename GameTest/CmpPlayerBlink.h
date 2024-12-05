#pragma once
#include "Entity.h"

static float BLINK_RADIUS = 0.15f;

struct CmpPlayerBlink
{
	Entity playerCharacter;
	int mask;

	CmpPlayerBlink(const Entity& playerCharacter, int mask)
		: playerCharacter(playerCharacter), mask(mask)
	{
	}
};
