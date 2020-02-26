/*
 * For Gold and Sweetrolls
 * Copyright (C) 2020 Piotr Wróblewski <szczypiorofix@o2.pl>
 */

#include "FontAssets.h"



FontAssets::FontAssets() {
#ifdef _DEBUG 
	std::cout << "Creating new font assets..." << std::endl;
#endif
	
	this->fonts[0] = NULL;
}


FontAssets::~FontAssets() {}


void FontAssets::releaseAssets() {
#ifdef _DEBUG 
	std::cout << "Releasing fonts ... ";
#endif
	
	for (int i = 0; i < MAX_FONTS; i++) {
		delete FontAssets::getAssets()->fonts[i];
	}
	
#ifdef _DEBUG 
	std::cout << "done." << std::endl;
#endif
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
