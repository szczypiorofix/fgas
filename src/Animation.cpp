/*
 * For Gold and Sweetrolls
 * Copyright (C) 2020 Piotr Wróblewski <szczypiorofix@o2.pl>
 */

#include "Animation.h"


Animation::Animation(unsigned short speed, const unsigned short size, std::vector<CUINT> tilesArray) {
	this->size = size;
	this->speed = speed;
	this->counter = 0;
	this->curFrame = 0;
	this->tilesArray = tilesArray;
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
	int res = 0;
	try {
		res = this->tilesArray.at(this->curFrame);
	} catch (std::out_of_range & ex) {
		std::cout << "OUT OF RANGE EXCEPTION !!!" << ex.what() << std::endl;
	}
	return res;
}


int Animation::getCurFrame() {
	return this->curFrame;
}


void Animation::setCurrentFrame(int cf) {
	this->curFrame = cf;
}


Animation::~Animation() {}
