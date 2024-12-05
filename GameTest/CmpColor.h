#pragma once
#include "float4.h"

struct CmpColor
{
	float4 value;

	CmpColor(const float3& value)
		: value(value, 1.0f)
	{
	}
	CmpColor(const float4& value)
		: value(value)
	{
	}
};
