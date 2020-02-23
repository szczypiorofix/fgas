/*
 * For Gold and Sweetrolls
 * Copyright (C) 2020 Piotr Wróblewski <szczypiorofix@o2.pl>
 */

#include "Animation.h"

Animation::Animation(unsigned short speed, const unsigned short size, CUINT* tilesArray) {
	this->size = size;
	this->speed = speed;
	this->counter = 0;
	this->curFrame = 0;
	this->frames = nullptr;
	this->tilesArray = tilesArray;
}


Animation::Animation(unsigned int textureWidth, unsigned int textureTileWidth, unsigned short speed, unsigned int sw, unsigned int sh, const unsigned short size, const unsigned int* frames) {
	this->size = size;
	this->speed = speed;
	this->counter = 0;
	this->curFrame = 0;
	this->tilesArray = nullptr;
	this->frames = new SDL_Rect[size];

	for (unsigned int i = 0; i < size; i++) {
		int col = textureWidth / textureTileWidth;
		this->frames[i].x = ((frames[i] - 1) % col) * sw;
		this->frames[i].y = ((frames[i] - 1) / col) * sh;
		this->frames[i].w = sw;
		this->frames[i].h = sh;
	}
}


int Animation::nextTile() {
	this->counter++;
	if (this->counter > this->speed) {
		this->counter = 0;
		this->curFrame++;
		if (this->curFrame >= this->size)
			this->curFrame = 0;
	}
	return this->curFrame;
}


int Animation::nextFrame() {
	this->counter++;
	if (this->counter > this->speed) {
		this->counter = 0;
		this->curFrame++;
		if (this->curFrame >= this->size)
			this->curFrame = 0;
	}
	return this->curFrame;
}


int Animation::getTile() {
	return this->tilesArray[this->curFrame];
}


SDL_Rect Animation::getFrame(int index) {
	return this->frames[index];
}


int Animation::getCurFrame() {
	return this->curFrame;
}


void Animation::setCurrentFrame(int cf) {
	this->curFrame = cf;
}


Animation::~Animation() {}
