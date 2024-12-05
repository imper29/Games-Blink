#include "stdafx.h"
#include "MenuMain.h"
#include "MenuControls.h"
#include "SceneManager.h"

#include "Level1.h"
#include "Level2.h"
#include "Level3.h"
#include "Level4.h"
#include "Level5.h"
#include "Level6.h"
#include "Level7.h"
#include "Level8.h"

#define PLAYER_COUNT_TABLE_S 3
static char* playerCountTableStr[PLAYER_COUNT_TABLE_S] = {
	"Players Two  ",
	"Players Three",
	"Players Four "
};
static int playerCountTable[PLAYER_COUNT_TABLE_S] = {
	2,
	3,
	4
};
static int playerCountIndex = 0;
#define GAMEOVER_TIME_TABLE_S 5
static char* gameoverTimeTableStr[GAMEOVER_TIME_TABLE_S] = {
	"Time 00:30",
	"Time 01:00",
	"Time 02:30",
	"Time 05:00",
	"Time 10:00"
};
static float gameoverTimeTable[GAMEOVER_TIME_TABLE_S] = {
	30.0f,
	60.0f,
	150.0f,
	300.0f,
	600.0f
};
static int gameoverTimeIndex = 1;
#define MUSIC_TABLE_S 5
static const char* musicTableStr[MUSIC_TABLE_S] = {
	"Music",
	"'Cheer Up' by OctoSound",
	"'Energetic Indie Rock Upbeat' by Onoychenkomusic",
	"'Having Fun' by OctoSound",
	"'Let's Rock' by Paolo Argento",
};
static const char* musicTable[MUSIC_TABLE_S] = {
	"Music/silence.wav",
	"Music/cheer-up-174791.wav",
	"Music/energetic-indie-rock-upbeat-132700.wav",
	"Music/having-fun-179821.wav",
	"Music/letx27s-rock-165609.wav",
};
static int musicIndex = -1;

static void BtnCyclePlayerCount(Scene& scene, Entity button) {
	MenuMain* menu = dynamic_cast<MenuMain*>(&scene);
	if (menu != nullptr) {
		playerCountIndex++;
		if (playerCountIndex >= PLAYER_COUNT_TABLE_S)
			playerCountIndex = 0;
		scene.entities.GetComponent<CmpText>(menu->playerCountText).text = playerCountTableStr[playerCountIndex];
	}
}
static void BtnCycleGameoverTime(Scene& scene, Entity button) {
	MenuMain* menu = dynamic_cast<MenuMain*>(&scene);
	if (menu != nullptr) {
		gameoverTimeIndex++;
		if (gameoverTimeIndex >= GAMEOVER_TIME_TABLE_S)
			gameoverTimeIndex = 0;
		scene.entities.GetComponent<CmpText>(menu->gameoverTimeText).text = gameoverTimeTableStr[gameoverTimeIndex];
	}
}
static void BtnCycleMusic(Scene& scene, Entity button) {
	MenuMain* menu = dynamic_cast<MenuMain*>(&scene);
	if (menu != nullptr) {
		App::StopSound(musicTable[musicIndex]);
		musicIndex++;
		if (musicIndex >= MUSIC_TABLE_S)
			musicIndex = 0;
		App::PlaySound(musicTable[musicIndex], true);
		scene.entities.GetComponent<CmpText>(menu->currentMusicText).text = musicTableStr[musicIndex];
	}
}
static void BtnControls(Scene& scene, Entity button) {
	SceneManager::LoadScene(std::unique_ptr<Scene>(new MenuControls()));
}
static void BtnQuit(Scene& scene, Entity button) {
	exit(0);
}

static void BtnLoadLevel1(Scene& scene, Entity button) {
	MenuMain* menu = dynamic_cast<MenuMain*>(&scene);
	SceneManager::LoadScene(std::unique_ptr<Scene>(new Level1(gameoverTimeTable[gameoverTimeIndex], playerCountTable[playerCountIndex])));
}
static void BtnLoadLevel2(Scene& scene, Entity button) {
	MenuMain* menu = dynamic_cast<MenuMain*>(&scene);
	SceneManager::LoadScene(std::unique_ptr<Scene>(new Level2(gameoverTimeTable[gameoverTimeIndex], playerCountTable[playerCountIndex])));
}
static void BtnLoadLevel3(Scene& scene, Entity button) {
	MenuMain* menu = dynamic_cast<MenuMain*>(&scene);
	SceneManager::LoadScene(std::unique_ptr<Scene>(new Level3(gameoverTimeTable[gameoverTimeIndex], playerCountTable[playerCountIndex])));
}
static void BtnLoadLevel4(Scene& scene, Entity button) {
	MenuMain* menu = dynamic_cast<MenuMain*>(&scene);
	SceneManager::LoadScene(std::unique_ptr<Scene>(new Level4(gameoverTimeTable[gameoverTimeIndex], playerCountTable[playerCountIndex])));
}
static void BtnLoadLevel5(Scene& scene, Entity button) {
	MenuMain* menu = dynamic_cast<MenuMain*>(&scene);
	SceneManager::LoadScene(std::unique_ptr<Scene>(new Level5(gameoverTimeTable[gameoverTimeIndex], playerCountTable[playerCountIndex])));
}
static void BtnLoadLevel6(Scene& scene, Entity button) {
	MenuMain* menu = dynamic_cast<MenuMain*>(&scene);
	SceneManager::LoadScene(std::unique_ptr<Scene>(new Level6(gameoverTimeTable[gameoverTimeIndex], playerCountTable[playerCountIndex])));
}
static void BtnLoadLevel7(Scene& scene, Entity button) {
	MenuMain* menu = dynamic_cast<MenuMain*>(&scene);
	SceneManager::LoadScene(std::unique_ptr<Scene>(new Level7(gameoverTimeTable[gameoverTimeIndex], playerCountTable[playerCountIndex])));
}
static void BtnLoadLevel8(Scene& scene, Entity button) {
	MenuMain* menu = dynamic_cast<MenuMain*>(&scene);
	SceneManager::LoadScene(std::unique_ptr<Scene>(new Level8(gameoverTimeTable[gameoverTimeIndex], playerCountTable[playerCountIndex])));
}

MenuMain::MenuMain() : Menu(), playerCountText(), gameoverTimeText(), currentMusicText() {
	//Setup music.
	{
		if (musicIndex == -1) {
			musicIndex = 0;
			App::PlaySound(musicTable[musicIndex], true);
		}
	}
	//Setup misc buttons.
	{
		float2 position = float2(50.0f, 32.5f);
		float2 size = float2(10.0f, 2.5f);
		float2 h = float2(20.0f, 0.0f);
		float2 v = float2(0.0f, -5.0f);
		playerCountText = CreateButton(position + h * -1.0f, size, playerCountTableStr[playerCountIndex], BtnCyclePlayerCount).first;
		gameoverTimeText = CreateButton(position + h * 0.0f, size, gameoverTimeTableStr[gameoverTimeIndex], BtnCycleGameoverTime).first;
		currentMusicText = CreateButton(position + v * 1.0f, size + h, musicTableStr[musicIndex], BtnCycleMusic).first;
		CreateButton(position + v * 2.0f, size + h, "Controls", BtnControls);
		CreateButton(position + h * 1.0f, size, "Quit", BtnQuit);
	}
	//Setup level buttons.
	{
		float2 position = float2(20.0f, 75.0f);
		float2 size = float2(10.0f, 10.0f);
		float2 h = float2(20.0f, 0.0f);
		float2 v = float2(0.0f, -20.0f);
		CreateButton(position + h * 0.0f + v * 0.0f, size, "Level 1", BtnLoadLevel1);
		CreateButton(position + h * 1.0f + v * 0.0f, size, "Level 2", BtnLoadLevel2);
		CreateButton(position + h * 2.0f + v * 0.0f, size, "Level 3", BtnLoadLevel3);
		CreateButton(position + h * 3.0f + v * 0.0f, size, "Level 4", BtnLoadLevel4);
		CreateButton(position + h * 0.0f + v * 1.0f, size, "Level 5", BtnLoadLevel5);
		CreateButton(position + h * 1.0f + v * 1.0f, size, "Level 6", BtnLoadLevel6);
		CreateButton(position + h * 2.0f + v * 1.0f, size, "Level 7", BtnLoadLevel7);
		CreateButton(position + h * 3.0f + v * 1.0f, size, "Level 8", BtnLoadLevel8);
	}
}
