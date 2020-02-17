/*
 * For Gold and Sweetrolls
 * Copyright (C) 2020 Piotr Wróblewski <szczypiorofix@o2.pl>
 */

#pragma once

#include "Engine.h"
#include "GraphicAssets.h"
#include "ShaderLoader.h"


class Game {

public:

	Game();
	
	void start();


private:

	Engine* engine;
	bool quit;
	Texture* backgroundTexture;
	Texture* logoTexture;

	ShaderLoader* shader;

	void mainLoop();

	void input(SDL_Event* e);
	void update();
	void render();

};

