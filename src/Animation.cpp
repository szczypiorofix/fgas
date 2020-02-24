/*
 * For Gold and Sweetrolls
 * Copyright (C) 2020 Piotr Wróblewski <szczypiorofix@o2.pl>
 */

#include "Animation.h"


Animation::Animation(u16 speed, cu16 size, std::vector<u16> tilesArray) {
	this->size = size;
	this->speed = speed;
	this->counter = 0;
	this->curFrame = 0;
	this->tilesArray = tilesArray;
}


void Animation::nextFrame(void) {
	this->counter++;
	if (this->counter > this->speed) {
		this->counter = 0;
		this->curFrame++;
		if (this->curFrame >= this->size)
			this->curFrame = 0;
	}
}


u16 Animation::getTile(void) {
	u16 res = 0;
	try {
		res = this->tilesArray.at(this->curFrame);
	} catch (std::out_of_range & ex) {
		std::cout << "OUT OF RANGE EXCEPTION !!!" << ex.what() << std::endl;
	}
	return res;
}


u16 Animation::getCurFrame(void) {
	return this->curFrame;
}


void Animation::setCurrentFrame(u16 cf) {
	this->curFrame = cf;
}


Animation::~Animation() {}
