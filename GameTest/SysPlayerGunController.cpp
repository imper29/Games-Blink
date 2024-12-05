#include "stdafx.h"
#include "SysPlayerGunController.h"
#include <vector>

#include "CmpPlayer.h"
#include "CmpPlayerCharacter.h"
#include "CmpPosition.h"
#include "CmpVelocity.h"
#include "CmpShapeAabb.h"
#include "CmpGravityScale.h"
#include "CmpPlayerBullet.h"
#include "CmpCollider.h"
#include "CmpColor.h"
#include "CmpSelfDestructTime.h"
#include "CmpShapePoint.h"
#include "Gamepad.h"

static bool IsShooting(const Gamepad& controller) {
	return controller.buttons[GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER];
}
static float2 randRange(float2 min, float2 max) {
	return float2(FRAND_RANGE(min.x, max.x), FRAND_RANGE(min.y, max.y));
}
static void GenerateParticles(EntityRegistry& reg, Entity e, int count, float time) {
	if (!reg.HasComponent<CmpVelocity>(e) || !reg.HasComponent<CmpPlayerCharacter>(e) || !reg.HasComponent<CmpShapeAabb>(e))
		return;

	auto& ext = reg.GetComponent<CmpShapeAabb>(e);
	auto& pos = reg.GetComponent<CmpPosition>(e);
	auto& vel = reg.GetComponent<CmpVelocity>(e);
	for (int i = 0; i < count; ++i) {
		Entity p = reg.CreateEntity();
		reg.AddComponent<CmpShapePoint>(p);
		reg.AddComponent<CmpSelfDestructTime>(p, time + FRAND_RANGE(0.3f, 0.6f));
		reg.AddComponent<CmpGravityScale>(p, 0.0f);
		reg.AddComponent<CmpPosition>(p, pos.value + randRange(-ext.extents, ext.extents));
		reg.AddComponent<CmpVelocity>(p, randRange(float2(-1.0f, -1.0f), float2(1.0f, 1.0f)));
		reg.AddComponent<CmpColor>(p, reg.GetComponentOrDefault<CmpColor>(e, CmpColor(float4(1, 1, 1, 1)).value));
	}
}
static void BulletHit(EntityRegistry& reg, const Entity& bullet, const Entity& other, std::vector<Entity>& toDestroy, float time) {
	if (reg.HasComponent<CmpPlayerBullet>(bullet)) {
		toDestroy.push_back(bullet);
		App::PlaySoundW("Sounds/mixkit-body-impact-falling-into-the-sand-2498-cut-tiny.wav");
		if (reg.HasComponent<CmpPlayerCharacter>(other)) {
			toDestroy.push_back(other);
			App::PlaySound("Sounds/mixkit-body-impact-falling-into-the-sand-2498-cut.wav");
			GenerateParticles(reg, other, 100, time);

			auto bulletPlayer = reg.GetComponent<CmpPlayerBullet>(bullet).player;
			if (reg.Exists(bulletPlayer) && reg.HasComponent<CmpPlayer>(bulletPlayer)) {
				auto& playerInfo = reg.GetComponent<CmpPlayer>(bulletPlayer);
				playerInfo.score++;
			}
		}
	}
}
static void ProcessGun(EntityRegistry& reg, EntityGroup& grp, uint32_t i, float time, float timestep) {
	auto& character = grp.GetComponent<CmpPlayerCharacter>(i);
	auto& controller = Gamepad::gamepads[character.gamepadId];
	//Recharge gun.
	if (character.gunOverheated)
		character.gunCharge += timestep * character.gunChargeRateOverheated;
	else if (!IsShooting(controller))
		character.gunCharge += timestep * character.gunChargeRate;
	if (character.gunCharge >= character.gunChargeLimit) {
		character.gunCharge = character.gunChargeLimit;
		if (character.gunOverheated) {
			character.gunOverheated = false;
			App::PlaySound("Sounds/mixkit-sci-fi-plasma-gun-power-up-1679-cut.wav");
		}
	}
	//Shoot gun validation.
	if (!IsShooting(controller))
		return;
	if (character.gunOverheated)
		return;
	if (character.gunFireTimeout > time)
		return;
	if (character.gunCharge < 0.0f)
		return;
	float2 direction;
	if (float2::normalizeSafe(float2(controller.axes[GLFW_GAMEPAD_AXIS_RIGHT_X], -controller.axes[GLFW_GAMEPAD_AXIS_RIGHT_Y]), direction)) {
		//Discharge gun.
		character.gunCharge -= character.gunFireTimeoutLimit;
		if (character.gunCharge <= 0.0f) {
			character.gunCharge = 0.0f;
			character.gunOverheated = true;
			App::PlaySound("Sounds/mixkit-laser-game-whip-1514-cut.wav");
		}
		//Shoot gun.
		character.gunFireTimeout = time + character.gunFireTimeoutLimit;
		App::PlaySound("Sounds/mixkit-short-laser-gun-shot-1670-cut.wav");
		//Collect bullet info.
		auto& position = grp.GetComponent<CmpPosition>(i);
		auto& velocity = grp.GetComponent<CmpVelocity>(i);
		auto color = float4(1, 1, 1, 1);
		if (grp.HasComponent<CmpColor>(i))
			color = grp.GetComponent<CmpColor>(i).value;

		//Calculate bullet velocity.
		auto aimHlp = 0.3f;
		auto aimDir = atan2f(direction.y, direction.x);
		if (aimDir > -aimHlp && aimDir < aimHlp)
			aimDir = 0.2f;
		if (aimDir > PI - aimHlp && aimDir < PI + aimHlp)
			aimDir = PI - 0.2f;
		auto angle = aimDir + FRAND_RANGE(-PI * 0.025f, PI * 0.025f);
		auto bulletVelocity = float2(cosf(angle), sinf(angle)) * 20.0f;
		auto bulletMask = 31 ^ (1 << character.gamepadId);

		//Shoot bullet.
		Entity e = reg.CreateEntity();
		reg.AddComponent<CmpPosition>(e, position.value);
		reg.AddComponent<CmpVelocity>(e, bulletVelocity);
		reg.AddComponent<CmpColor>(e, color);
		reg.AddComponent<CmpPlayerBullet>(e, character.player);
		reg.AddComponent<CmpGravityScale>(e, 0.5f);
		reg.AddComponent<CmpCollider>(e, bulletMask, bulletMask);
		reg.AddComponent<CmpShapeAabb>(e, float2(0.075f, 0.075f));
		reg.AddComponent<CmpSelfDestructTime>(e, time + 5.0f);
	}
}

void SysPlayerGunController::Update(float time, float timestep) {
	//Bullet shooting.
	auto r = scene.entities.GetComponentMask<CmpPlayerCharacter>()
		   | scene.entities.GetComponentMask<CmpVelocity>()
		   | scene.entities.GetComponentMask<CmpPosition>();
	auto e = scene.entities.GetEntities(r, 0);
	for (uint32_t i = 0; i < e.GetEntityCount(); ++i)
		ProcessGun(scene.entities, e, i, time, timestep);
	//Bullet landing.
	std::vector<Entity> toDestroy;
	for (auto& t : physics.triggerEvents) {
		BulletHit(scene.entities, t.a, t.b, toDestroy, time);
		BulletHit(scene.entities, t.b, t.a, toDestroy, time);
	}
	for (auto& e : toDestroy)
		scene.entities.DestroyEntity(e);
}
