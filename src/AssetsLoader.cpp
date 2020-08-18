/*
 * For Gold and Sweetrolls
 * Copyright (C) 2020 Piotr Wróblewski <szczypiorofix@o2.pl>
 */

#include "AssetsLoader.h"


AssetsLoader::AssetsLoader() {}


AssetsLoader::~AssetsLoader() {}



GraphicAssets::GraphicAssets(void) {
	debugInfoNl(DEBUG_INFO, "Creating new graphic assets.");

	this->textures[IMAGE_ASSETS_MAIN_MENU_BACKGROUND] = nullptr;
	this->textures[IMAGE_ASSETS_LOGO] = nullptr;
	this->textures[IMAGE_ASSETS_MAIN_MENU_BUTTONS] = nullptr;
	this->textures[IMAGE_ASSETS_VINGUE_FONT] = nullptr;
	this->textures[IMAGE_ASSETS_BIG_SPRITESHEET] = nullptr;
	this->textures[IMAGE_ASSETS_MAIN_MENU_BUTTONS] = nullptr;
}


void GraphicAssets::releaseAssets(void) {
	debugInfoNl(DEBUG_INFO, "Releasing textures.");
	for (int i = 0; i < MAX_SPRITESHEETS; i++) {
		if (GraphicAssets::getAssets()->textures[i] != NULL) {
			delete GraphicAssets::getAssets()->textures[i];
			GraphicAssets::getAssets()->textures[i] = nullptr;
		}
	}
}


GraphicAssets* GraphicAssets::getAssets(void) {
	if (!instance) {
		instance = new GraphicAssets();
	}
	return instance;
}


void GraphicAssets::addToAssets(std::string fileName, cu8 imagesEnum) {
	Texture* tempTexture = new Texture(fileName, 0, 0, 800, 600);
	GraphicAssets::getAssets()->textures[imagesEnum] = tempTexture;
}


// Set instance to null pointer
GraphicAssets* GraphicAssets::instance = nullptr;







//FontAssets::FontAssets(void) {
//	debugInfoNl(DEBUG_INFO, "Creating new font assets.");
//	this->fonts[0] = nullptr;
//}
//
//
//FontAssets::~FontAssets(void) {}
//
//
//void FontAssets::releaseAssets(void) {
//
//	debugInfoNl(DEBUG_INFO, "Releasing fonts.");
//
//	for (int i = 0; i < MAX_FONTS; i++) {
//		delete FontAssets::getAssets()->fonts[i];
//		FontAssets::getAssets()->fonts[i] = nullptr;
//	}
//
//}
//
//
//FontAssets* FontAssets::getAssets(void) {
//	if (!instance) {
//		instance = new FontAssets();
//	}
//	return instance;
//}
//
//
//void FontAssets::addToAssets(std::string fileName, Texture* tex, const int fontEnum) {
//	TextFont* tempFont = new TextFont(fileName, tex);
//	FontAssets::getAssets()->fonts[fontEnum] = tempFont;
//}
//
//
//// Set instance to null pointer
//FontAssets* FontAssets::instance = nullptr;
