#pragma once
#include "Menu.h"
#include "Entity.h"

class MenuMain : public Menu
{
public:
	MenuMain();

	Entity playerCountText;
	Entity gameoverTimeText;
	Entity currentMusicText;
};
