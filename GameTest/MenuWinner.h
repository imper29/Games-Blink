#pragma once
#include "Menu.h"

class MenuWinner : public Menu
{
public:
	MenuWinner(std::vector<std::pair<int, int>>&& winnerToLoser);

	char playerScoreText[3 * 4];
};
