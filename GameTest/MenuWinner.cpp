#include "stdafx.h"
#include "MenuWinner.h"
#include "SceneManager.h"
#include "MenuMain.h"

#include "CmpPlayer.h"

static void BtnMenu(Scene& scene, Entity entity) {
	SceneManager::LoadScene(std::unique_ptr<Scene>(new MenuMain()));
}
static void BtnQuit(Scene& scene, Entity entity) {
	exit(0);
}

MenuWinner::MenuWinner(std::vector<std::pair<int, int>>&& winnerToLoser) : Menu(), playerScoreText()
{
	//Register scoreboard.
	float2 size = float2(9.5f, 3.0f);
	{
		float2 positions[4] = {
			float2(50, 90.0f),
			float2(50, 79.0f),
			float2(50, 68.0f),
			float2(50, 57.0f)
		};
		float4 colorBackground = float4(0.2f, 0.2f, 0.4f, 1.0f);
		float4 colorHeader = float4(0.5f, 0.5f, 1.0f, 1.0f);
		float4 colorContent = float4(0.35f, 0.35f, 0.7f, 1.0f);
		float4 colorCooldown = float4(0.5f, 0.5f, 1.0f, 1.0f);
		for (int i = 0; i < winnerToLoser.size(); ++i) {
			auto playerId = winnerToLoser.at(i).first;
			auto playerScore = winnerToLoser.at(i).second;

			//Create background.
			auto background = entities.CreateEntity();
			entities.AddComponent<CmpOverlay>(background);
			entities.AddComponent<CmpColor>(background, CmpPlayer::GetColor(playerId) * colorBackground);
			entities.AddComponent<CmpPosition>(background, positions[i]);
			entities.AddComponent<CmpShapeAabb>(background, size);
			//Create name background.
			auto nameBackground = entities.CreateEntity();
			entities.AddComponent<CmpOverlay>(nameBackground);
			entities.AddComponent<CmpColor>(nameBackground, CmpPlayer::GetColor(playerId) * colorHeader);
			entities.AddComponent<CmpPosition>(nameBackground, positions[i] + float2(0.0f, size.y - 1.25f));
			entities.AddComponent<CmpShapeAabb>(nameBackground, float2(size.x - 0.25f, 1.0f));
			//Create name text.
			auto nameText = entities.CreateEntity();
			entities.AddComponent<CmpOverlay>(nameText);
			entities.AddComponent<CmpColor>(nameText, float4(1.0f, 1.0f, 1.0f, 1.0f));
			entities.AddComponent<CmpPosition>(nameText, positions[i] + float2(0.0f, size.y - 1.0f));
			entities.AddComponent<CmpText>(nameText, CmpPlayer::GetName(playerId), 1.5f);

			//Create score background.
			auto scoreBackground = entities.CreateEntity();
			entities.AddComponent<CmpOverlay>(scoreBackground);
			entities.AddComponent<CmpColor>(scoreBackground, CmpPlayer::GetColor(playerId) * colorContent);
			entities.AddComponent<CmpPosition>(scoreBackground, positions[i] + float2(0.0f, 1.75f - size.y));
			entities.AddComponent<CmpShapeAabb>(scoreBackground, float2(size.x - 0.25f, 1.5f));
			//Create score text.
			auto scoreText = entities.CreateEntity();
			entities.AddComponent<CmpOverlay>(scoreText);
			entities.AddComponent<CmpColor>(scoreText, float4(1.0f, 1.0f, 1.0f, 1.0f));
			entities.AddComponent<CmpPosition>(scoreText, positions[i] + float2(0.0f, 2.0f - size.y));
			entities.AddComponent<CmpText>(scoreText, playerScoreText + (i * 3), 2.0f);
			snprintf(playerScoreText + (i * 3), 3, "%02d", playerScore);
		}
	}
	//Register buttons.
	{
		CreateButton(float2(50.0f - size.x - 2.0f, 46.0f), size, "Menu", BtnMenu);
		CreateButton(float2(50.0f + size.x + 2.0f, 46.0f), size, "Quit", BtnQuit);
	}
}
