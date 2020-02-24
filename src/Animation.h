/*
 * For Gold and Sweetrolls
 * Copyright (C) 2020 Piotr Wróblewski <szczypiorofix@o2.pl>
 */

#ifndef _ANIMATION_H_
#define _ANIMATION_H_
#pragma once

#include <iostream>
#include <vector>
#include "Tileset.h"


class Animation {

public:
	Animation(unsigned short speed, const unsigned short size, std::vector<CUINT> tilesArray);
	~Animation();
	int nextFrame();
	int getTile();
	int getCurFrame();
	void setCurrentFrame(int curFrame);

private:
	unsigned short size;
	std::vector<CUINT> tilesArray;
	unsigned short speed;
	int curFrame;
	int counter;

};

#endif
