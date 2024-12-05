#pragma once
#include "float2.h"

struct CmpShapeAabb
{
	float2 extents;

	CmpShapeAabb(const float2& extents)
		: extents(extents)
	{
	}
};
