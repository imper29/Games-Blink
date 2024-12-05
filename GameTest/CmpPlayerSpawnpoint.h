#pragma once
struct CmpPlayerSpawnpoint
{
	int mask;
	int lives;

	CmpPlayerSpawnpoint(int mask, int lives)
		: mask(mask), lives(lives)
	{
	}
};
