/*
 * Dungeon Engine
 * Copyright (C) 2020 szczypiorofix <szczypiorofix@o2.pl>
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
	this->textures[IMAGE_ASSETS_TEST_TEXTURE] = NULL;
}


void GraphicAssets::releaseAssets() {
#ifdef _DEBUG 
	printf("Releasing textures ...\n");
#endif
	
	for (int i = 0; i < MAX_SPRITESHEETS; i++) {
		delete GraphicAssets::getAssets()->textures[i];
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
