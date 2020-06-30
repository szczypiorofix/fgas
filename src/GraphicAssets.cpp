/*
 * For Gold and Sweetrolls
 * Copyright (C) 2020 Piotr Wróblewski <szczypiorofix@o2.pl>
 */

#include "GraphicAssets.h"
#include "Defines.h"


GraphicAssets::GraphicAssets() {

	debugInfoNl(DEBUG_INFO, "Creating new graphic assets.");
	
	this->textures[IMAGE_ASSETS_MAIN_MENU_BACKGROUND] = nullptr;
	this->textures[IMAGE_ASSETS_LOGO] = nullptr;
	this->textures[IMAGE_ASSETS_MAIN_MENU_BUTTONS] = nullptr;
	this->textures[IMAGE_ASSETS_VINGUE_FONT] = nullptr;
	this->textures[IMAGE_ASSETS_BIG_SPRITESHEET] = nullptr;
	this->textures[IMAGE_ASSETS_MAIN_MENU_BUTTONS] = nullptr;
}


void GraphicAssets::releaseAssets() {

	debugInfoNl(DEBUG_INFO, "Releasing textures.");
	
	for (int i = 0; i < MAX_SPRITESHEETS; i++) {
		if (GraphicAssets::getAssets()->textures[i]) {
			/*printf("Releasing texture pointer %p\n", &GraphicAssets::getAssets()->textures[i]);
			printf("Texture id: %i\n", GraphicAssets::getAssets()->textures[i]->textureId);*/
			delete GraphicAssets::getAssets()->textures[i];
			GraphicAssets::getAssets()->textures[i] = nullptr;
		}
	}
}


GraphicAssets* GraphicAssets::getAssets() {
	if (!instance) {
		instance = new GraphicAssets();
	}
	return instance;
}


void GraphicAssets::addToAssets(std::string fileName, GLfloat tileWidth, GLfloat tileHeight, cu8 imagesEnum) {
	Texture* tempTexture = new Texture(fileName, tileWidth, tileHeight);
	GraphicAssets::getAssets()->textures[imagesEnum] = tempTexture;
}


void GraphicAssets::addToAssets(std::string fileName, cu8 imagesEnum) {
	Texture* tempTexture = new Texture(fileName);
	GraphicAssets::getAssets()->textures[imagesEnum] = tempTexture;
}


// Set instance to null pointer
GraphicAssets* GraphicAssets::instance = nullptr;
