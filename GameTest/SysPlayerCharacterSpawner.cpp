#include "stdafx.h"
#include "SysPlayerCharacterSpawner.h"

#include "CmpPosition.h"
#include "CmpVelocity.h"
#include "CmpCollider.h"
#include "CmpMaterial.h"
#include "CmpShapeAabb.h"

#include "CmpColor.h"
#include "CmpPlayer.h"
#include "CmpPlayerCharacter.h"
#include "CmpPlayerSpawnpoint.h"

static Entity SpawnPlayerCharacter(EntityRegistry& reg, Entity player, Entity spawnpoint) {
	auto e = reg.CreateEntity();
	auto& p = reg.GetComponent<CmpPlayer>(player);
	auto position = reg.GetComponent<CmpPosition>(spawnpoint).value;
	position.y -= 0.05f;
	auto extents = reg.GetComponent<CmpShapeAabb>(spawnpoint).extents;
	extents.x -= 0.05f;
	extents.y -= 0.025f;

	reg.AddComponent<CmpColor>(e, reg.GetComponent<CmpColor>(player).value);
	reg.AddComponent<CmpPosition>(e, position);
	reg.AddComponent<CmpShapeAabb>(e, extents);
	reg.AddComponent<CmpCollider>(e, 1 << p.gamepadId, 1 << p.gamepadId);
	reg.AddComponent<CmpPlayerCharacter>(e, player, p.gamepadId);
	reg.AddComponent<CmpVelocity>(e, float2(0, 0));
	reg.AddComponent<CmpMaterial>(e, 0.01f, 0.2f);

	reg.GetComponent<CmpPlayerSpawnpoint>(spawnpoint).lives--;
	p.characterExistedPrev = true;
	return e;
}
static float GetDistanceToNearestCharacter(EntityRegistry& reg, float2 pos) {
	auto r = reg.GetComponentMask<CmpPlayerCharacter>()
		    | reg.GetComponentMask<CmpPosition>();
	auto e = reg.GetEntities(r, 0);

	auto distance = std::numeric_limits<float>::infinity();
	for (uint32_t i = 0; i < e.GetEntityCount(); ++i) {
		auto& chrPos = e.GetComponent<CmpPosition>(i);
		auto chrOff = pos - chrPos.value;
		auto chrDst = float2::length(chrOff);
		if (distance > chrDst) {
			distance = chrDst;
		}
	}
	return distance;
}
static Entity GetSpawnpoint(EntityRegistry& reg, int mask) {
	auto rs = reg.GetComponentMask<CmpPlayerSpawnpoint>()
			| reg.GetComponentMask<CmpPosition>();
	auto es = reg.GetEntities(rs, 0);

	float best = -std::numeric_limits<float>::infinity();
	Entity result = Entity();
	for (uint32_t i = 0; i < es.GetEntityCount(); ++i) {
		auto& spawn = es.GetComponent<CmpPlayerSpawnpoint>(i);
		if (spawn.lives != 0 && (spawn.mask & mask)) {
			auto& spawnPos = es.GetComponent<CmpPosition>(i);
			auto distance = GetDistanceToNearestCharacter(reg, spawnPos.value);
			if (best < distance) {
				best = distance;
				result = es.GetEntity(i);
			}
		}
	}
	return result;
}

void SysPlayerCharacterSpawner::Update(float time, float timestep)
{
	auto rp = scene.entities.GetComponentMask<CmpPlayer>()
			| scene.entities.GetComponentMask<CmpColor>();
	auto ep = scene.entities.GetEntities(rp, 0);
	for (uint32_t i = 0; i < ep.GetEntityCount(); ++i) {
		auto& player = ep.GetComponent<CmpPlayer>(i);
		if (!scene.entities.Exists(player.character)) {
			if (player.characterExistedPrev) {
				player.respawnTimeout = player.respawnTimeoutLimit + time;
				player.characterExistedPrev = false;
			}
			if (player.respawnTimeout < time) {
				auto spawnpoint = GetSpawnpoint(scene.entities, player.spawnpointMask);
				if (scene.entities.Exists(spawnpoint)) {
					player.character = SpawnPlayerCharacter(scene.entities, ep.GetEntity(i), spawnpoint);
				}
			}
		}
	}
}
