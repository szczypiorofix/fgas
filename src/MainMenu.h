/*
 * For Gold and Sweetrolls
 * Copyright (C) 2020 Piotr Wróblewski <szczypiorofix@o2.pl>
 */
#ifndef _MAINMENU_H_
#define _MAINMENU_H_
#pragma once

#include <SDL2/SDL_events.h>
#include "TiledMap.h"
#include "Animation.h"
#include "Texture.h"
#include "MainMenuButton.h"


class MainMenu {

public:
	MainMenu(State& _state);
	~MainMenu();

	void update();
	void input(SDL_Event& event);
	void render();


private:

	State& state;
	
	std::vector<MainMenuButton*> mainMenuButtons;

	Texture* bigSpriteSheet;
	
	TiledMap* mainMenuBackgroundMap;

	Animation* torchAnimation;
	Animation* diamondAnimation;
	Animation* cardsAnimation;

};

#endif
