/*
 * For Gold and Sweetrolls
 * Copyright (C) 2020 Piotr Wr�blewski <szczypiorofix@o2.pl>
 */

#include "FontAssets.h"
#include "Defines.h"


FontAssets::FontAssets() {

	debugInfoNl(DEBUG_INFO, "Creating new font assets.");
	
	this->fonts[0] = nullptr;
}


FontAssets::~FontAssets() {}


void FontAssets::releaseAssets() {

	debugInfoNl(DEBUG_INFO, "Releasing fonts.");
	
	for (int i = 0; i < MAX_FONTS; i++) {
		delete FontAssets::getAssets()->fonts[i];
		FontAssets::getAssets()->fonts[i] = nullptr;
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
