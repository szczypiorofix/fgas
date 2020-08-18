/*
 * For Gold and Sweetrolls
 * Copyright (C) 2020 Piotr Wróblewski <szczypiorofix@o2.pl>
 */
#ifndef _MAINMENU_H_
#define _MAINMENU_H_
#pragma once

#include <SDL2/SDL_events.h>
#include <Box2D/Box2D.h>
#include "MainMenuButton.h"
#include "TiledMap.h"
#include "Animation.h"
//#include "Camera2D.h"


class MainMenu {

public:
	MainMenu(State& _state);
	~MainMenu();

	void update();
	void input(SDL_Event& event);
	void render();

private:
	
	//const float CAMERA_SPEED = 10.0f;
	//const float SCALE_SPEED = 5.0f;

	GLfloat x, y;
	s8 moveX, moveY;
	State& state;
	
	std::vector<MainMenuButton*> mainMenuButtons;

	//Camera2D* camera;

	Texture* bigSpriteSheet;
	Texture* logoTexture;
	Texture* backgroundTexture;
	
	TiledMap* mainMenuBackgroundMap;

	Animation* torchAnimation;
	Animation* diamondAnimation;
	Animation* cardsAnimation;

	// Box2D
	b2Vec2 gravity;
	b2World world;
	
	b2BodyDef groundBodyDef;
	b2Body* groundBody;
	b2PolygonShape groundBox;

	b2BodyDef bodyDef;
	b2Body* body;
	b2PolygonShape dynamicBox;
	b2FixtureDef fixtureDef;

	float timeStep;
	int32 velocityIterations = 6;
	int32 positionIterations = 2;

};

#endif
