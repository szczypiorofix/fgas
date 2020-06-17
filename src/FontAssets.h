/*
 * For Gold and Sweetrolls
 * Copyright (C) 2020 Piotr Wróblewski <szczypiorofix@o2.pl>
 */

#ifndef _FONT_ASSETS_H_
#define _FONT_ASSETS_H_
#pragma once

#include <string>
#include "TextFont.h"

constexpr int MAX_FONTS = 1;


class FontAssets {

public:

	static const int FONT_ASSETS_VINGUE = 0;

	static FontAssets* getAssets();
	static void addToAssets(std::string fileName, Texture* tex, const int fontEnum);
	static void releaseAssets();

	TextFont* fonts[MAX_FONTS];

private:
	static FontAssets* instance;

	FontAssets();
	~FontAssets();

};

#endif
