#pragma once
#include "Entity.h"
#include "App/app.h"

#define PLAYER_INPUT_JUMP XINPUT_GAMEPAD_LEFT_SHOULDER
#define PLAYER_INPUT_BLINK XINPUT_GAMEPAD_RIGHT_SHOULDER
#define PLAYER_INPUT_SHOOT 

struct CmpPlayerCharacter
{
	Entity player;
	int gamepadId;

	bool gunOverheated;
	float gunChargeLimit;
	float gunChargeRateOverheated;
	float gunChargeRate;
	float gunCharge;
	
	float gunFireTimeoutLimit;
	float gunFireTimeout;

	float blinkTimeoutLimit;
	float blinkTimeout;

	float walkStrengthAir;
	float walkStrength;
	float walkSpeed;

	float jumpStrength;
	int jumpCountLimit;
	int jumpCount;
	bool jumpPrev;
	bool jumpWallPrevL;
	bool jumpWallPrevR;

	float slideStrength;

	float onGround;
	float onWallL;
	float onWallR;

	CmpPlayerCharacter(Entity player, int gamepadId)
		: player(player)
		, gamepadId(gamepadId)

		, gunOverheated(false)
		, gunChargeLimit(1.0f)
		, gunChargeRateOverheated(0.25f)
		, gunChargeRate(0.5f)
		, gunCharge(gunChargeLimit)
		, gunFireTimeoutLimit(0.025f)
		, gunFireTimeout(0.0f)
		
		, blinkTimeoutLimit(0.5f)
		, blinkTimeout(0.0f)
		
		, walkStrengthAir(40.0f)
		, walkStrength(50.0f)
		, walkSpeed(10.0f)

		, jumpStrength(12.5f)
		, jumpCountLimit(2)
		, jumpCount(0)
		, jumpPrev(false)
		, jumpWallPrevL(false)
		, jumpWallPrevR(false)

		, slideStrength(4.0f)

		, onGround()
		, onWallL()
		, onWallR()
	{

	}
};
