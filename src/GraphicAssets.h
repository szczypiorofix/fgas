/*
 * For Gold and Sweetrolls
 * Copyright (C) 2020 Piotr Wróblewski <szczypiorofix@o2.pl>
 */

#ifndef _GRAPHICASSETS_H_
#define _GRAPHICASSETS_H_
#pragma once

#include <string>
#include "Texture.h"

constexpr int MAX_SPRITESHEETS = 5;


class GraphicAssets {

public:

	static const int IMAGE_ASSETS_MAIN_MENU_BACKGROUND = 0;
	static const int IMAGE_ASSETS_LOGO = 1;
	static const int IMAGE_ASSETS_MAIN_MENU_BUTTONS = 2;
	static const int IMAGE_ASSETS_VINGUE_FONT = 3;
	static const int IMAGE_ASSETS_TEST_TEXTURE = 4;
	

	static GraphicAssets* getAssets();
	static void addToAssets(std::string fileName, GLfloat spriteWidth, GLfloat spriteHeight, const int imagesEnum);
	static void addToAssets(std::string fileName, const int imagesEnum);
	static void releaseAssets();

	Texture* textures[MAX_SPRITESHEETS];

private:

	static GraphicAssets* instance;

	GraphicAssets();

};

#endif
