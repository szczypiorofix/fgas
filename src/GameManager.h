/*
 * For Gold and Sweetrolls
 * Copyright (C) 2020 Piotr Wróblewski <szczypiorofix@o2.pl>
 */

#ifndef _GAMEMANAGER_H_
#define _GAMEMANAGER_H_
#pragma once

#include "Engine.h"
#include "MainMenu.h"
#include "MainGame.h"
#include "Defines.h"
#include "ShaderLoader.h"
#include "Camera2D.h"
#include "SpriteSheet.h"


class GameManager {

public:
	GameManager();
	void start();
	Camera2D* camera;
	const float CAMERA_SPEED = 10.0f;
	const float SCALE_SPEED = 10.0f;
private:
	Engine* engine;
	bool quit;
	MainMenu* mainMenu;
	MainGame* mainGame;
	ShaderLoader* shader;
	Texture* logoTexture;
	SpriteSheet* bigAtlas;
	State state;
	void mainLoop();
	void input(SDL_Event& e);
	void update();
	void render();
	GLfloat mX, mY;
	s16 moveX, moveY;
};


#endif
