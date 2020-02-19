/*
 * For Gold and Sweetrolls
 * Copyright (C) 2020 Piotr Wróblewski <szczypiorofix@o2.pl>
 */

#pragma once

#include "Engine.h"
#include "GraphicAssets.h"
#include "ShaderLoader.h"
#include "MainMenu.h"


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
	Texture* backgroundTexture;
	Texture* logoTexture;
	Texture* bigSpriteSheet;
	MainMenu* mainMenu;

	State state;

	ShaderLoader* shader;

	void mainLoop();

	void input(SDL_Event& e);
	void update();
	void render();

};

