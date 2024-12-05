#include "stdafx.h"
#include "SysPlayerMovementController.h"
#include "App/app.h"

#include "CmpVelocity.h"
#include "CmpPlayerCharacter.h"
#include "Gamepad.h"
#include "GLFW/glfw3.h"

static float JUMP_GRACE_TIME = 0.2f;
static float clamp(float min, float max, float value) {
	if (value < min) return min;
	if (value > max) return max;
	return value;
}
static void UpdateFlags(EvtCollision& c, CmpPlayerCharacter& p, float time, bool flipNormal) {
	float2 normal;
	if (flipNormal)
		normal = -c.normal;
	else
		normal = c.normal;

	//Player is on ground?
	if (float2::dot(normal, float2(0.0f, -1.0f)) > 0.8f) {
		p.onGround = time + JUMP_GRACE_TIME;
		p.jumpWallPrevL = false;
		p.jumpWallPrevR = false;
		p.jumpCount = p.jumpCountLimit;
	}
	//Player is on wall left?
	if (float2::dot(normal, float2(-1.0f, 0.0f)) > 0.8f)
		p.onWallL = time + JUMP_GRACE_TIME;
	//Player is on wall right?
	if (float2::dot(normal, float2(1.0f, 0.0f)) > 0.8f)
		p.onWallR = time + JUMP_GRACE_TIME;
}

void SysPlayerMovementController::Update(float time, float timestep)
{
	auto r = scene.entities.GetComponentMask<CmpPlayerCharacter>()
		   | scene.entities.GetComponentMask<CmpVelocity>();
	auto e = scene.entities.GetEntities(r, 0);

	//Update flags.
	for (auto& c : physics.collisionEvents) {
		if (scene.entities.Exists(c.a) && scene.entities.HasComponent<CmpPlayerCharacter>(c.a))
			UpdateFlags(c, scene.entities.GetComponent<CmpPlayerCharacter>(c.a), time, false);
		if (scene.entities.Exists(c.b) && scene.entities.HasComponent<CmpPlayerCharacter>(c.b))
			UpdateFlags(c, scene.entities.GetComponent<CmpPlayerCharacter>(c.b), time, true);
	}

	//Handle movement.
	for (uint32_t i = 0; i < e.GetEntityCount(); ++i) {
		auto& character = e.GetComponent<CmpPlayerCharacter>(i);
		auto& velocity = e.GetComponent<CmpVelocity>(i);
		auto& controller = Gamepad::gamepads[character.gamepadId];

		//Handle walking.
		{
			float walkStrength = character.walkStrength;
			if (character.onGround < time)
				walkStrength = character.walkStrengthAir;
			float max = clamp(-character.walkSpeed, character.walkSpeed, velocity.value.x + walkStrength * timestep);
			float min = clamp(-character.walkSpeed, character.walkSpeed, velocity.value.x - walkStrength * timestep);
			velocity.value.x = clamp(min, max, character.walkSpeed * controller.axes[GLFW_GAMEPAD_AXIS_LEFT_X]);
		}
		//Handle jumping.
		{
			bool jump = controller.axes[GLFW_GAMEPAD_AXIS_LEFT_Y] < -0.5f;
			if (jump && !character.jumpPrev) {
				if (character.onGround < time && character.onWallL >= time /* && !character.jumpWallPrevL*/) {
					//Jump from left wall.
					velocity.value = float2(character.jumpStrength * 0.5f, character.jumpStrength);
					character.onWallL = 0.0f;
					character.jumpWallPrevL = true;
					character.jumpWallPrevR = false;
				}
				else if (character.onGround < time && character.onWallR >= time /* && !character.jumpWallPrevR*/) {
					//Jump from right wall.
					velocity.value = float2(-character.jumpStrength * 0.5f, character.jumpStrength);
					character.onWallR = 0.0f;
					character.jumpWallPrevL = false;
					character.jumpWallPrevR = true;
				}
				else if (character.jumpCount > 0) {
					//Jump from ground.
					velocity.value.y = character.jumpStrength;
					character.jumpCount--;
				}
			}
			character.jumpPrev = jump;
		}
		//Handle wall sliding.
		{
			//Cannot wall slide when moving up.
			if (velocity.value.y < 0.0f) {
				float dir = controller.axes[GLFW_GAMEPAD_AXIS_LEFT_X];
				if (dir > 0.0f && character.onWallR >= time) {
					//Slide on right wall.
					velocity.value.y -= velocity.value.y * dir * timestep * character.slideStrength;
				}
				else if (dir < 0.0f && character.onWallL >= time) {
					//Slide on left wall.
					velocity.value.y += velocity.value.y * dir * timestep * character.slideStrength;
				}
			}
		}
	}
}
