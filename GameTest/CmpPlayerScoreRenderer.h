#pragma once
#include "Entity.h"

struct CmpPlayerScoreRenderer
{
	Entity player;
	char* text;
	int length;

	CmpPlayerScoreRenderer(const Entity& player, char* text, int length)
		: player(player)
		, text(text)
		, length(length)
	{
	}
};
