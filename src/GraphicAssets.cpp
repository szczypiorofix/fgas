/*
 * For Gold and Sweetrolls
 * Copyright (C) 2020 Piotr Wróblewski <szczypiorofix@o2.pl>
 */

#include "GraphicAssets.h"
#include "Defines.h"


GraphicAssets::GraphicAssets() {
#ifdef _DEBUG 
	printf("Creating new graphic assets.\n");
#endif
	
	this->textures[IMAGE_ASSETS_MAIN_MENU_BACKGROUND] = NULL;
	this->textures[IMAGE_ASSETS_LOGO] = NULL;
	this->textures[IMAGE_ASSETS_MAIN_MENU_BUTTONS] = NULL;
	this->textures[IMAGE_ASSETS_VINGUE_FONT] = NULL;
	this->textures[IMAGE_ASSETS_BIG_SPRITESHEET] = NULL;
}


void GraphicAssets::releaseAssets() {
#ifdef _DEBUG 
	printf("Releasing textures ...\n");
#endif
	
	for (int i = 0; i < MAX_SPRITESHEETS; i++) {
		if (GraphicAssets::getAssets()->textures[i] != NULL) {
			delete GraphicAssets::getAssets()->textures[i];
			GraphicAssets::getAssets()->textures[i] = NULL;
		}
	}
}


GraphicAssets* GraphicAssets::getAssets() {
	if (!instance) {
		instance = new GraphicAssets();
	}
	return instance;
}


void GraphicAssets::addToAssets(std::string fileName, GLfloat tileWidth, GLfloat tileHeight, const int imagesEnum) {
	Texture* tempTexture = new Texture(fileName, tileWidth, tileHeight);
	GraphicAssets::getAssets()->textures[imagesEnum] = tempTexture;
}


void GraphicAssets::addToAssets(std::string fileName, const int imagesEnum) {
	Texture* tempTexture = new Texture(fileName);
	GraphicAssets::getAssets()->textures[imagesEnum] = tempTexture;
}


// Set instance to null pointer
GraphicAssets* GraphicAssets::instance = nullptr;
