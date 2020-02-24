/*
 * For Gold and Sweetrolls
 * Copyright (C) 2020 Piotr Wróblewski <szczypiorofix@o2.pl>
 */

#ifndef _GRAPHICASSETS_H_
#define _GRAPHICASSETS_H_
#pragma once

#include <string>
#include "Defines.h"
#include "Texture.h"


constexpr u8 MAX_SPRITESHEETS						= 6;


class GraphicAssets {

public:
	static cu8 IMAGE_ASSETS_MOUSE_CURSOR			= 0;
	static cu8 IMAGE_ASSETS_MAIN_MENU_BACKGROUND	= 1;
	static cu8 IMAGE_ASSETS_LOGO					= 2;
	static cu8 IMAGE_ASSETS_MAIN_MENU_BUTTONS		= 3;
	static cu8 IMAGE_ASSETS_VINGUE_FONT				= 4;
	static cu8 IMAGE_ASSETS_BIG_SPRITESHEET			= 5;
	

	static GraphicAssets* getAssets();
	static void addToAssets(std::string fileName, GLfloat spriteWidth, GLfloat spriteHeight, cu8 imagesEnum);
	static void addToAssets(std::string fileName, cu8 imagesEnum);
	static void releaseAssets();

	Texture* textures[MAX_SPRITESHEETS];

private:

	static GraphicAssets* instance;

	GraphicAssets();

};

#endif
