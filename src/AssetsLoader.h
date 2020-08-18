/*
 * For Gold and Sweetrolls
 * Copyright (C) 2020 Piotr Wróblewski <szczypiorofix@o2.pl>
 */

#ifndef _ASSETSLOADER_H_
#define _ASSETSLOADER_H_
#pragma once

#include "Defines.h"
#include "Texture.h"
//#include "TextFont.h"


constexpr u8 MAX_SPRITESHEETS = 6;
constexpr int MAX_FONTS = 1;



class GraphicAssets {
public:
	static cu8 IMAGE_ASSETS_MOUSE_CURSOR = 0;
	static cu8 IMAGE_ASSETS_MAIN_MENU_BACKGROUND = 1;
	static cu8 IMAGE_ASSETS_LOGO = 2;
	static cu8 IMAGE_ASSETS_MAIN_MENU_BUTTONS = 3;
	static cu8 IMAGE_ASSETS_VINGUE_FONT = 4;
	static cu8 IMAGE_ASSETS_BIG_SPRITESHEET = 5;
	static GraphicAssets* getAssets(void);
	static void addToAssets(std::string fileName, cu8 imagesEnum);
	static void releaseAssets(void);
	Texture* textures[MAX_SPRITESHEETS];
private:
	static GraphicAssets* instance;
	GraphicAssets(void);
};


//class FontAssets {
//public:
//	static const int FONT_ASSETS_VINGUE = 0;
//	static FontAssets* getAssets();
//	static void addToAssets(std::string fileName, Texture* tex, const int fontEnum);
//	static void releaseAssets();
//	TextFont* fonts[MAX_FONTS];
//private:
//	static FontAssets* instance;
//	FontAssets(void);
//	~FontAssets(void);
//};


class AssetsLoader {
public:
	AssetsLoader();
	~AssetsLoader();
	



private:


};

#endif
