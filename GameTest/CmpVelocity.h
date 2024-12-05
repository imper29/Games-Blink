#pragma once
#include "float2.h"

struct CmpVelocity
{
	float2 value;

	CmpVelocity(const float2& value)
		: value(value)
	{
	}
};
