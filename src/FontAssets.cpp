/*
 * For Gold and Sweetrolls
 * Copyright (C) 2020 Piotr Wr�blewski <szczypiorofix@o2.pl>
 */

#include "FontAssets.h"



FontAssets::FontAssets() {
#ifdef _DEBUG 
	printf("Creating new font assets...\n");
#endif
	
	this->fonts[0] = NULL;
}


FontAssets::~FontAssets() {}


void FontAssets::releaseAssets() {
#ifdef _DEBUG 
	printf("Releasing fonts ...\n");
#endif
	
	for (int i = 0; i < MAX_FONTS; i++) {
		delete FontAssets::getAssets()->fonts[i];
	}
	
}


FontAssets* FontAssets::getAssets() {
	if (!instance) {
		instance = new FontAssets();
	}
	return instance;
}


void FontAssets::addToAssets(std::string fileName, Texture* tex, const int fontEnum) {
	TextFont* tempFont = new TextFont(fileName, tex);
	FontAssets::getAssets()->fonts[fontEnum] = tempFont;
}


FontAssets* FontAssets::instance = nullptr;
