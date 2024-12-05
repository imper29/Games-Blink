#pragma once
#include "Entity.h"
#include "float4.h"

struct CmpPlayer
{
	Entity character;
	int gamepadId;
	int spawnpointMask;
	int score;
	float respawnTimeoutLimit;
	float respawnTimeout;
	bool characterExistedPrev;

	CmpPlayer(int gamepadId, int spawnpointMask)
		: character{ ~0U, ~0U }
		, gamepadId(gamepadId)
		, spawnpointMask(spawnpointMask)
		, score(0)
		, respawnTimeoutLimit(1.5f)
		, respawnTimeout(0.0f)
		, characterExistedPrev(false)
	{
	}

	static float4 GetColor(int playerId) {
		const static float4 colors[4] = {
			float4(1,0,0,1),
			float4(0,1,0,1),
			float4(0.5f,0.5f,1,1),
			float4(1,0,1,1),
		};
		return colors[playerId];
	}
	static const char* GetName(int playerId) {
		static const char* names[4] = {
			"Player 1",
			"Player 2",
			"Player 3",
			"Player 4"
		};
		return names[playerId];
	}
};
