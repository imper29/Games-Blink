#include "stdafx.h"
#include "Level3.h"

Level3::Level3(float gameoverTime, uint32_t playerCount) : Level(gameoverTime, playerCount) {
	CreateDeathAround(float2(-30, -30), float2(30, 40));

	CreateGround(float2(-30, -30), float2(-25, 40));
	CreateGround(float2(25, -30), float2(30, 40));

	CreateGround(float2(-25, 12), float2(-20, 15));
	CreateSpawnpoint(float2(-22.5f, 15));
	CreateGround(float2(20, 12), float2(25, 15));
	CreateSpawnpoint(float2(22.5f, 15));

	CreateGround(float2(-25, -3), float2(-20, 0));
	CreateSpawnpoint(float2(-22.5f, 0));
	CreateGround(float2(20, -3), float2(25, 0));
	CreateSpawnpoint(float2(22.5f, 0));

	CreateGround(float2(-10, 17), float2(10, 20));
	CreateGround(float2(-10, 4), float2(10, 7));
	CreateGround(float2(-10, -8), float2(10, -5));
}
