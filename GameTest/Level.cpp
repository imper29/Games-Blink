#include "stdafx.h"
#include "Level.h"
#include "float4.h"
#include <algorithm>

#include "SysCameraFree.h"
#include "SysCameraFollowPlayers.h"
#include "SysDrawCircles.h"
#include "SysDrawPoints.h"
#include "SysDrawAabbs.h"
#include "SysDrawText.h"
#include "SysPhysicsCollisionCollector.h"
#include "SysPhysicsCollisionResolver.h"
#include "SysPhysicsBodyMover.h"
#include "SysPhysicsGravity.h"
#include "SysSelfDestruct.h"
#include "SysDeathTriggerHandler.h"
#include "SysPlayerCharacterSpawner.h"
#include "SysPlayerMovementController.h"
#include "SysPlayerBlinkController.h"
#include "SysPlayerParticles.h"
#include "SysPlayerBlinkParticles.h"
#include "SysPlayerGunController.h"
#include "SysPlayerAimLineRenderer.h"
#include "SysPlayerGunCooldownRenderer.h"
#include "SysPlayerScoreRenderer.h"
#include "SysTimedEventHandler.h"
#include "SysButtonHandler.h"
#include "SysButtonColorizer.h"

#include "CmpColor.h"
#include "CmpPlayer.h"
#include "CmpTimedEvent.h"
#include "CmpOverlay.h"
#include "CmpPosition.h"
#include "CmpText.h"
#include "CmpShapeAabb.h"
#include "CmpPlayerScoreRenderer.h"
#include "CmpPlayerGunCooldownRenderer.h"
#include "CmpPlayerSpawnpoint.h"
#include "CmpCollider.h"
#include "CmpVelocity.h"
#include "CmpMass.h"
#include "CmpDeath.h"

#include "SceneManager.h"
#include "MenuWinner.h"

static bool SortDescendingBySecond(const std::pair<int, int> a, const std::pair<int, int> b) {
	return a.second > b.second;
}
static Entity CreatePlayer(EntityRegistry& reg, int gamepadId, int spawnpointMask) {
	auto e = reg.CreateEntity();
	reg.AddComponent<CmpColor>(e, CmpPlayer::GetColor(gamepadId));
	reg.AddComponent<CmpPlayer>(e, gamepadId, spawnpointMask);
	return e;
}
static float TryExecuteGameover(Scene& scene, Entity entity, float time, float period) {
	auto r = scene.entities.GetComponentMask<CmpPlayer>();
	auto e = scene.entities.GetEntities(r, 0);
	auto tie = false;
	auto highestScore = 0;
	std::vector<std::pair<int, int>> winnerToLoser;
	for (uint32_t i = 0; i < e.GetEntityCount(); ++i) {
		auto& p = e.GetComponent<CmpPlayer>(i);
		if (highestScore < p.score) {
			highestScore = p.score;
			tie = false;
		}
		else if (highestScore == p.score) {
			tie = true;
		}
		winnerToLoser.push_back(std::make_pair(p.gamepadId, p.score));
	}
	if (!tie) {
		std::stable_sort(winnerToLoser.begin(), winnerToLoser.end(), SortDescendingBySecond);
		SceneManager::LoadScene(std::unique_ptr<Scene>(new MenuWinner(std::move(winnerToLoser))));
		return -1.0f;
	}
	return time + period;
}
static float TryUpdateClock(Scene& scene, Entity entity, float time, float period) {
	Level* level = dynamic_cast<Level*>(&scene);
	if (level != nullptr && scene.entities.HasComponent<CmpText>(entity) && scene.entities.HasComponent<CmpColor>(entity)) {
		auto& txt = scene.entities.GetComponent<CmpText>(entity);
		auto& col = scene.entities.GetComponent<CmpColor>(entity);
		float timeRemaining = level->gameoverTime - time;
		if (timeRemaining < 0.0f) {
			txt.text = "Overtime";
			col.value = float4(1.0f, 0.0f, 0.0f, 1.0f);
		}
		else {
			txt.text = level->clockTimeText;
			snprintf(level->clockTimeText, 6, "%02d:%02d", (int)(timeRemaining / 60.0f), (int)(fmodf(timeRemaining, 60.0f)));
			col.value = float4(1.0f, 1.0f, 1.0f, 1.0f);
			App::PlaySound("Sounds/mixkit-fast-wall-clock-ticking-1063-cut.wav");
		}
	}
	return time + period;
}

Level::Level(float gameoverTime, uint32_t playerCount) : Scene(), gameoverTime(gameoverTime) {
	//Register systems.
	{
		//AddOrGetSystem<SysCameraFree>();
		AddOrGetSystem<SysCameraFollowPlayers>();

		AddOrGetSystem<SysButtonColorizer>();
		AddOrGetSystem<SysButtonHandler>();

		AddOrGetSystem<SysPlayerScoreRenderer>();
		AddOrGetSystem<SysPlayerGunCooldownRenderer>();
		//AddOrGetSystem<SysPlayerAimLineRenderer>();
		AddOrGetSystem<SysDrawCircles>();
		AddOrGetSystem<SysDrawPoints>();
		AddOrGetSystem<SysDrawAabbs>();
		AddOrGetSystem<SysDrawText>();

		AddOrGetSystem<SysSelfDestruct>();
		AddOrGetSystem<SysPhysicsGravity>();
		AddOrGetSystem<SysPhysicsBodyMover>();
		AddOrGetSystem<SysPhysicsCollisionCollector>();
		AddOrGetSystem<SysDeathTriggerHandler>();
		AddOrGetSystem<SysPhysicsCollisionResolver>();
		AddOrGetSystem<SysTimedEventHandler>();
		AddOrGetSystem<SysPlayerParticles>();
		AddOrGetSystem<SysPlayerBlinkParticles>();
		AddOrGetSystem<SysPlayerGunController>();
		AddOrGetSystem<SysPlayerCharacterSpawner>();
		AddOrGetSystem<SysPlayerMovementController>();
		AddOrGetSystem<SysPlayerBlinkController>();
	}
	//Register players.
	{
		if (playerCount > 4)
			playerCount = 4;
		for (uint32_t i = 0; i < playerCount; ++i)
			players.push_back(CreatePlayer(entities, i, 1 << i));
	}
	//Register clock.
	{
		//Create gameover.
		auto gameover = entities.CreateEntity();
		entities.AddComponent<CmpTimedEvent>(gameover, gameoverTime, 0.1f, TryExecuteGameover);
		//Create border.
		auto border = entities.CreateEntity();
		entities.AddComponent<CmpColor>(border, float4(0.2f, 0.2f, 0.2f, 1.0f));
		entities.AddComponent<CmpPosition>(border, float2(50.0f, 96.0f));
		entities.AddComponent<CmpShapeAabb>(border, float2(9.5f, 3.0f));
		entities.AddComponent<CmpOverlay>(border);
		//Create clock.
		auto clock = entities.CreateEntity();
		entities.AddComponent<CmpColor>(clock, float4(1.0f, 1.0f, 1.0f, 1.0f));
		entities.AddComponent<CmpOverlay>(clock);
		entities.AddComponent<CmpPosition>(clock, float2(50.0f, 96.75f));
		entities.AddComponent<CmpText>(clock, clockTimeText, 5.0f);
		entities.AddComponent<CmpTimedEvent>(clock, 1.0f, 1.0f, TryUpdateClock);
		snprintf(clockTimeText, 6, "%02d:%02d", (int)(gameoverTime / 60.0f), (int)(fmodf(gameoverTime, 60.0f)));
	}
	//Register interface.
	{
		float2 positions[4] = {
			float2(10.25f, 96.0f),
			float2(30.25f, 96.0f),
			float2(69.75f, 96.0f),
			float2(89.75f, 96.0f)
		};
		float2 size = float2(9.5f, 3.0f);
		float4 colorBackground = float4(0.2f, 0.2f, 0.4f, 1.0f);
		float4 colorHeader = float4(0.5f, 0.5f, 1.0f, 1.0f);
		float4 colorContent = float4(0.35f, 0.35f, 0.7f, 1.0f);
		float4 colorCooldown = float4(0.5f, 0.5f, 1.0f, 1.0f);
		for (int i = 0; i < playerCount; ++i) {
			//Create background.
			auto background = entities.CreateEntity();
			entities.AddComponent<CmpOverlay>(background);
			entities.AddComponent<CmpColor>(background, CmpPlayer::GetColor(i) * colorBackground);
			entities.AddComponent<CmpPosition>(background, positions[i]);
			entities.AddComponent<CmpShapeAabb>(background, size);

			//Create name background.
			auto nameBackground = entities.CreateEntity();
			entities.AddComponent<CmpOverlay>(nameBackground);
			entities.AddComponent<CmpColor>(nameBackground, CmpPlayer::GetColor(i) * colorHeader);
			entities.AddComponent<CmpPosition>(nameBackground, positions[i] + float2(0.0f, size.y - 1.25f));
			entities.AddComponent<CmpShapeAabb>(nameBackground, float2(size.x - 0.25f, 1.0f));
			//Create name text.
			auto nameText = entities.CreateEntity();
			entities.AddComponent<CmpOverlay>(nameText);
			entities.AddComponent<CmpColor>(nameText, float4(1.0f, 1.0f, 1.0f, 1.0f));
			entities.AddComponent<CmpPosition>(nameText, positions[i] + float2(0.0f, size.y - 1.0f));
			entities.AddComponent<CmpText>(nameText, CmpPlayer::GetName(i), 1.5f);

			//Create score background.
			auto scoreBackground = entities.CreateEntity();
			entities.AddComponent<CmpOverlay>(scoreBackground);
			entities.AddComponent<CmpColor>(scoreBackground, CmpPlayer::GetColor(i) * colorContent);
			entities.AddComponent<CmpPosition>(scoreBackground, positions[i] + float2(size.x - 3.25f, 1.75f - size.y));
			entities.AddComponent<CmpShapeAabb>(scoreBackground, float2(3.0f, 1.5f));
			//Create score text.
			auto scoreText = entities.CreateEntity();
			entities.AddComponent<CmpOverlay>(scoreText);
			entities.AddComponent<CmpColor>(scoreText, float4(1.0f, 1.0f, 1.0f, 1.0f));
			entities.AddComponent<CmpPosition>(scoreText, positions[i] + float2(size.x - 3.25f, 2.0f - size.y));
			entities.AddComponent<CmpPlayerScoreRenderer>(scoreText, players.at(i), playerScoreText + (i * 3), 3);
			entities.AddComponent<CmpText>(scoreText, playerScoreText + (i * 3), 2.0f);

			//Create gun cooldown background.
			float2 gunCooldownBackgroundPos = positions[i] + float2(-3.25f, 1.75f - size.y);
			float2 gunCooldownBackgroundExt = float2(size.x - 3.5f, 1.5f);
			auto gunCooldownBackground = entities.CreateEntity();
			entities.AddComponent<CmpOverlay>(gunCooldownBackground);
			entities.AddComponent<CmpColor>(gunCooldownBackground, CmpPlayer::GetColor(i)* colorContent);
			entities.AddComponent<CmpPosition>(gunCooldownBackground, gunCooldownBackgroundPos);
			entities.AddComponent<CmpShapeAabb>(gunCooldownBackground, gunCooldownBackgroundExt);
			//Create gun cooldown bar.
			auto gunCooldownBar = entities.CreateEntity();
			entities.AddComponent<CmpOverlay>(gunCooldownBar);
			entities.AddComponent<CmpColor>(gunCooldownBar, CmpPlayer::GetColor(i) * colorCooldown);
			entities.AddComponent<CmpPosition>(gunCooldownBar, gunCooldownBackgroundPos);
			entities.AddComponent<CmpShapeAabb>(gunCooldownBar, gunCooldownBackgroundExt - float2(0.1f, 0.1f));
			entities.AddComponent<CmpPlayerGunCooldownRenderer>(gunCooldownBar, players[i],
				gunCooldownBackgroundPos.x - gunCooldownBackgroundExt.x + 0.1f,
				gunCooldownBackgroundPos.x + gunCooldownBackgroundExt.x - 0.1f,
				CmpPlayer::GetColor(i) * colorCooldown);
		}
	}
}

Entity Level::CreateSpawnpoint(float2 pos, int mask, int lives)
{
	Entity e = entities.CreateEntity();
	entities.AddComponent<CmpColor>(e, float4(1.0f, 1.0f, 0.0f, 0.3f));
	entities.AddComponent<CmpPosition>(e, float2(pos.x, pos.y + 0.4f));
	entities.AddComponent<CmpShapeAabb>(e, float2(0.3f, 0.4f));
	entities.AddComponent<CmpPlayerSpawnpoint>(e, mask, lives);
	return e;
}
Entity Level::CreateGround(float2 min, float2 max)
{
	float2 pos = (max + min) * 0.5f;
	float2 ext = (max - min) * 0.5f;
	Entity e = entities.CreateEntity();
	entities.AddComponent<CmpColor>(e, float4(1.0f, 1.0f, 1.0f, 1.0f));
	entities.AddComponent<CmpPosition>(e, pos);
	entities.AddComponent<CmpShapeAabb>(e, ext);
	entities.AddComponent<CmpCollider>(e, ~0, ~0);
	return e;
}
Entity Level::CreateBox(float2 min, float2 max, float mass)
{
	float2 pos = (max + min) * 0.5f;
	float2 ext = (max - min) * 0.5f;
	Entity e = entities.CreateEntity();
	entities.AddComponent<CmpColor>(e, float4(1.0f, 1.0f, 1.0f, 1.0f));
	entities.AddComponent<CmpPosition>(e, pos);
	entities.AddComponent<CmpShapeAabb>(e, ext);
	entities.AddComponent<CmpCollider>(e, ~0, ~0);
	entities.AddComponent<CmpVelocity>(e, float2(0, 0));
	entities.AddComponent<CmpMass>(e, mass);
	return e;
}
Entity Level::CreateDeath(float2 min, float2 max)
{
	float2 pos = (max + min) * 0.5f;
	float2 ext = (max - min) * 0.5f;
	Entity e = entities.CreateEntity();
	entities.AddComponent<CmpColor>(e, float4(1.0f, 0.0f, 0.0f, 0.3f));
	entities.AddComponent<CmpPosition>(e, pos);
	entities.AddComponent<CmpShapeAabb>(e, ext);
	entities.AddComponent<CmpCollider>(e, 15, 0);
	entities.AddComponent<CmpDeath>(e);
	return e;
}
void Level::CreateDeathAround(float2 min, float2 max)
{
	//Bottom
	CreateDeath(float2(min.x - 1000, min.y - 1000), float2(max.x + 1000, min.y));
	//Top
	CreateDeath(float2(min.x - 1000, max.y), float2(max.x + 1000, max.y + 1000));
	//Left
	CreateDeath(float2(min.x - 1000, min.y), float2(min.x, max.y));
	//Right
	CreateDeath(float2(max.x, min.y), float2(max.x + 1000, max.y));
}
