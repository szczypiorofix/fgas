/*
 * For Gold and Sweetrolls
 * Copyright (C) 2020 szczypiorofix <szczypiorofix@o2.pl>
 */

#pragma once

#include "Engine.h"
#include "GraphicAssets.h"


class Game {

public:

	Game();
	
	void start();


private:

	Engine* engine;
	bool quit;
	Texture* backgroundTexture;
	Texture* logoTexture;

	void mainLoop();

	void input(SDL_Event* e);
	void update();
	void render();

};

