#pragma once
#include "Scene.h"
#include "Entity.h"
struct CmpTimedEvent
{
	/// <summary>
	/// Represents functions to be called at certain times. Returns the time the function should be called next or negative one to destroy the entity.
	/// </summary>
	typedef float (*Callback)(Scene& scene, Entity entity, float time, float period);

	float time;
	float period;
	Callback callback;

	CmpTimedEvent(float time, float period, const Callback& callback)
		: time(time), period(period), callback(callback)
	{
	}
};
