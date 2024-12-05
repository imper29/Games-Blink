#pragma once
#include "Entity.h"
class Scene;

using CmpButtonCallback = void(*)(Scene& scene, Entity entity);

struct CmpButton
{
	bool isEntered;
	bool isEnteredPrev;
	bool isPressed;
	bool isPressedPrev;
	CmpButtonCallback onPressed;
	CmpButtonCallback onReleased;

	CmpButton(const CmpButtonCallback& onPressed, const CmpButtonCallback& onReleased)
		: isEntered(false), isEnteredPrev(false), isPressed(false), isPressedPrev(false), onPressed(onPressed), onReleased(onReleased)
	{
	}
};
