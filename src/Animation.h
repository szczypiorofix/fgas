/*
 * For Gold and Sweetrolls
 * Copyright (C) 2020 Piotr Wr�blewski <szczypiorofix@o2.pl>
 */

#ifndef _ANIMATION_H_
#define _ANIMATION_H_
#pragma once

#include <SDL2/SDL.h>


class Animation {

public:
	Animation(unsigned int textureWidth, unsigned int textureTileWidth, unsigned short speed, unsigned int sw, unsigned int sh, const unsigned short size, const unsigned int* frames);
	Animation(unsigned short speed, const unsigned short size, unsigned int* tilesArray);
	~Animation();
	int nextFrame();
	int nextTile();
	int getTile();
	SDL_Rect getFrame(int index);
	int getCurFrame();
	void setCurrentFrame(int curFrame);

private:
	unsigned short size;
	SDL_Rect* frames;
	unsigned int* tilesArray;
	unsigned short speed;
	int curFrame;
	int counter;

};

#endif
