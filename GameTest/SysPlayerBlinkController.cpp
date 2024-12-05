#pragma once
#include "stdafx.h"
#include "SysPlayerBlinkController.h"
#include "App/app.h"

#include "CmpPlayerCharacter.h"
#include "CmpPlayerBlink.h"
#include "CmpPosition.h"
#include "CmpVelocity.h"
#include "CmpCollider.h"
#include "CmpShapeCircle.h"
#include "CmpColor.h"
#include "CmpSelfDestructTime.h"
#include "Gamepad.h"

static float BLINK_SPEED = 30.0f;
static float BLINK_LIFETIME = 10.0f;
static int BLINK_MASK_BASE = 1 << 5;

static void BlinkShoot(EntityRegistry& reg, const Entity& entity, const CmpPlayerCharacter& character, const float2& playerVelocity, const float2& playerPosition, const float2& direction, float time) {
	Entity e = reg.CreateEntity();
	reg.AddComponent<CmpPlayerBlink>(e, entity, BLINK_MASK_BASE << character.gamepadId);
	reg.AddComponent<CmpShapeCircle>(e, BLINK_RADIUS);
	reg.AddComponent<CmpColor>(e, reg.GetComponentOrDefault<CmpColor>(entity, CmpColor(float4(1, 1, 1, 1)).value));
	reg.AddComponent<CmpPosition>(e, playerPosition);
	reg.AddComponent<CmpVelocity>(e, direction * BLINK_SPEED);
	reg.AddComponent<CmpCollider>(e, BLINK_MASK_BASE << character.gamepadId, BLINK_MASK_BASE << character.gamepadId);
	reg.AddComponent<CmpSelfDestructTime>(e, time + BLINK_LIFETIME);
}
static void BlinkLand(EntityRegistry& reg, Entity blink, int mask, std::vector<Entity>& toDestroy) {
	if (reg.Exists(blink) && reg.HasComponent<CmpPlayerBlink>(blink) && reg.HasComponent<CmpPosition>(blink))
	{
		toDestroy.push_back(blink);
		auto& blk = reg.GetComponent<CmpPlayerBlink>(blink);
		auto& blkPos = reg.GetComponent<CmpPosition>(blink);
		if (blk.mask & mask) {
			if (reg.Exists(blk.playerCharacter) && reg.HasComponent<CmpPosition>(blk.playerCharacter) && reg.HasComponent<CmpVelocity>(blk.playerCharacter)) {
				auto& chrPos = reg.GetComponent<CmpPosition>(blk.playerCharacter);
				auto& chrVel = reg.GetComponent<CmpVelocity>(blk.playerCharacter);
				chrPos.value = blkPos.value;
				chrVel.value = float2(0, 0);
			}
		}
	}
}

void SysPlayerBlinkController::Update(float time, float timestep) {
	//Blink shooting.
	auto r = scene.entities.GetComponentMask<CmpPlayerCharacter>()
		   | scene.entities.GetComponentMask<CmpVelocity>()
		   | scene.entities.GetComponentMask<CmpPosition>();
	auto e = scene.entities.GetEntities(r, 0);
	for (uint32_t i = 0; i < e.GetEntityCount(); ++i) {
		auto& position = e.GetComponent<CmpPosition>(i);
		auto& velocity = e.GetComponent<CmpVelocity>(i);
		auto& character = e.GetComponent<CmpPlayerCharacter>(i);
		auto& controller = Gamepad::gamepads[character.gamepadId];

		if (character.blinkTimeout < time && controller.buttons[GLFW_GAMEPAD_BUTTON_LEFT_BUMPER]) {
			character.blinkTimeout = time + character.blinkTimeoutLimit;
			float2 direction;
			if (float2::normalizeSafe(float2(controller.axes[GLFW_GAMEPAD_AXIS_RIGHT_X], -controller.axes[GLFW_GAMEPAD_AXIS_RIGHT_Y]), direction)) {
				BlinkShoot(scene.entities, e.GetEntity(i), character, velocity.value, position.value, direction, time);
			}
		}
	}
	//Blink landing.
	std::vector<Entity> toDestroy;
	for (auto& t : physics.triggerEvents) {
		BlinkLand(scene.entities, t.a, t.mask, toDestroy);
		BlinkLand(scene.entities, t.b, t.mask, toDestroy);
	}
	for (auto& e : toDestroy)
		scene.entities.DestroyEntity(e);
}
