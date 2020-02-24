/*
 * For Gold and Sweetrolls
 * Copyright (C) 2020 Piotr Wróblewski <szczypiorofix@o2.pl>
 */

#ifndef _ANIMATION_H_
#define _ANIMATION_H_
#pragma once

#include <iostream>
#include <vector>
#include "Defines.h"


class Animation {

public:
	Animation(u16 speed, cu16 size, std::vector<u16> tilesArray);
	~Animation();
	void nextFrame(void);
	u16 getTile(void);
	u16 getCurFrame(void);
	void setCurrentFrame(u16 curFrame);

private:
	u16 size;
	std::vector<u16> tilesArray;
	unsigned short speed;
	u16 curFrame;
	u16 counter;

};

#endif
