/*
 * For Gold and Sweetrolls
 * Copyright (C) 2020 Piotr Wróblewski <szczypiorofix@o2.pl>
 */

#pragma once

#include "Engine.h"
#include "GraphicAssets.h"
#include "ShaderLoader.h"
#include "MainMenu.h"
#include "TiledMap.h"
#include "Animation.h"


enum class State {
	SPLASH_SCREEN,
	MAIN_MENU,
	GAME,
};


class GameManager {

public:

	GameManager();
	
	void start();


private:

	Engine* engine;
	bool quit;

	Texture* bigSpriteSheet;
	MainMenu* mainMenu;
	TiledMap* mainMenuBackgroundMap;

	State state;
	
	Animation* torchAnimation;
	Animation* lavaAnimation;
	Animation* altarOfBloodAnimation;
	Animation* firePlaceAnimation;


	ShaderLoader* shader;

	void mainLoop();

	void input(SDL_Event& e);
	void update();
	void render();

};

