#pragma once
#include "EntityRegistry.h"

struct CmpMaterial
{
	float friction;
	float bounciness;

	CmpMaterial(float friction, float bounciness)
		: friction(friction), bounciness(bounciness)
	{
	}

	static CmpMaterial GetMaterial(EntityRegistry& e, Entity a, Entity b) {
		if (e.HasComponent<CmpMaterial>(a)) {
			CmpMaterial aMat = e.GetComponent<CmpMaterial>(a);
			if (e.HasComponent<CmpMaterial>(b)) {
				CmpMaterial bMat = e.GetComponent<CmpMaterial>(b);
				return CmpMaterial((aMat.friction + bMat.friction) * 0.5f, (aMat.bounciness + bMat.bounciness) * 0.5f);
			}
			return aMat;
		}
		if (e.HasComponent<CmpMaterial>(b))
			return e.GetComponent<CmpMaterial>(b);
		return CmpMaterial(0.1f, 0);
	}
};
