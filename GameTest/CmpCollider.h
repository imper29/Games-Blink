#pragma once
#include <memory>

struct CmpCollider
{
	uint32_t maskTrigger;
	uint32_t maskCollision;

	CmpCollider(uint32_t maskTrigger, uint32_t maskCollision)
		: maskTrigger(maskTrigger), maskCollision(maskCollision)
	{
	}
};
