/*
 * For Gold and Sweetrolls
 * Copyright (C) 2020 Piotr Wróblewski <szczypiorofix@o2.pl>
 */
#ifndef _GAMEMANAGER_H_
#define _GAMEMANAGER_H_
#pragma once

#include "Engine.h"
#include "GraphicAssets.h"
#include "ShaderLoader.h"
#include "MainMenu.h"
#include "Defines.h"


class GameManager {

public:

	GameManager();
	
	void start();


private:

	Engine* engine;
	bool quit;

	MainMenu* mainMenu;
	
	State state;
	
	//ShaderLoader* shader;

	void mainLoop();

	void input(SDL_Event& e);
	void update();
	void render();

};


#endif
