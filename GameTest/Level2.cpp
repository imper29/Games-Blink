#include "stdafx.h"
#include "Level2.h"

Level2::Level2(float gameoverTime, uint32_t playerCount) : Level(gameoverTime, playerCount) {
	CreateDeathAround(float2(-45.0f, -25.0f), float2(45.0f, 25.0f));

	CreateGround(float2(-25, -7), float2(-15, -4));
	CreateSpawnpoint(float2(-20, -4));
	CreateGround(float2(15, -7), float2(25, -4));
	CreateSpawnpoint(float2(20, -4));

	CreateGround(float2(-25, 2), float2(-15, 5));
	CreateSpawnpoint(float2(-20, 5));
	CreateGround(float2(15, 2), float2(25, 5));
	CreateSpawnpoint(float2(20, 5));

	CreateGround(float2(-7, -3), float2(7, 1));
}
