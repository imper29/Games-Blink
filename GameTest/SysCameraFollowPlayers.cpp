#include "stdafx.h"
#include "App/app.h"
#include "SysCameraFollowPlayers.h"

#include "CmpPosition.h"
#include "CmpPlayerCharacter.h"

static float SPEED = 2;
static float BORDER_X = 10;
static float BORDER_Y = 6;
static float HEIGHT_MIN = 10;

static float2 GetMin(float2 a, float2 b) {
	return float2(fminf(a.x, b.x), fminf(a.y, b.y));
}
static float2 GetMax(float2 a, float2 b) {
	return float2(fmaxf(a.x, b.x), fmaxf(a.y, b.y));
}

void SysCameraFollowPlayers::Update(float time, float timestep)
{
	//Find the minimum bounds that contains all players.
	float2 targetMin = float2(100000, 100000);
	float2 targetMax = float2(-100000, -100000);
	auto r = scene.entities.GetComponentMask<CmpPosition>()
		   | scene.entities.GetComponentMask<CmpPlayerCharacter>();
	auto e = scene.entities.GetEntities(r, 0);
	for (uint32_t i = 0; i < e.GetEntityCount(); ++i) {
		auto& pos = e.GetComponent<CmpPosition>(i);
		targetMin = GetMin(targetMin, pos.value);
		targetMax = GetMax(targetMax, pos.value);
	}
	targetMin = targetMin - float2(BORDER_X, BORDER_Y);
	targetMax = targetMax + float2(BORDER_X, BORDER_Y);

	//Find current bounds.
	float2 currentMin = scene.GetWorldViewMin();
	float2 currentMax = scene.GetWorldViewMax();
	float2 currentSize = currentMax - currentMin;

	//Find target bounds.
	float2 targetPos = (targetMax + targetMin) * 0.5f;
	float2 deltaPos = targetPos - scene.viewWorldPosition;
	float targetHeight = fmaxf((targetMax.y - targetMin.y), (targetMax.x - targetMin.x) * currentSize.y / currentSize.x) * 0.5f;
	float deltaHeight = targetHeight - scene.viewWorldHeight;

	scene.viewWorldPosition = scene.viewWorldPosition + deltaPos * timestep * SPEED;
	scene.viewWorldHeight = fmaxf(scene.viewWorldHeight + deltaHeight * timestep * SPEED, HEIGHT_MIN);
}
