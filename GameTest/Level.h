#pragma once
#include "Scene.h"
#include <memory>

class Level : public Scene
{
public:
	Level(float gameoverTime, uint32_t playerCount);

	float gameoverTime;
	char clockTimeText[6] = "00.00";
	char playerScoreText[3 * 4] = "XX\0XX\0XX\0XX";
	std::vector<Entity> players;

protected:
	Entity CreateSpawnpoint(float2 pos, int mask = ~0, int lives = -1);
	Entity CreateGround(float2 min, float2 max);
	Entity CreateBox(float2 min, float2 max, float mass = 1.0f);
	Entity CreateDeath(float2 min, float2 max);
	void CreateDeathAround(float2 min, float2 max);
};
