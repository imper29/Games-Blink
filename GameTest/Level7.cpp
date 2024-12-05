#include "stdafx.h"
#include "Level7.h"

Level7::Level7(float gameoverTime, uint32_t playerCount) : Level(gameoverTime, playerCount) {
	CreateDeathAround(float2(-45.0f, -25.0f), float2(45.0f, 25.0f));

	CreateGround(float2(-25, -7), float2(-15, -4));
	CreateSpawnpoint(float2(-20, -4));
	CreateGround(float2(15, -7), float2(25, -4));
	CreateSpawnpoint(float2(20, -4));

	CreateGround(float2(-30, 2), float2(-20, 5));
	CreateSpawnpoint(float2(-25, 5));
	CreateGround(float2(20, 2), float2(30, 5));
	CreateSpawnpoint(float2(25, 5));
}
