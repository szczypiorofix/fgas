/*
 * For Gold and Sweetrolls
 * Copyright (C) 2020 Piotr Wróblewski <szczypiorofix@o2.pl>
 */

#include "GameObject.h"


GameObject::GameObject(void) {
	this->vector = new Vector2();
	this->width = 0.0f;
	this->height = 0.0f;
	this->name = "default_name";
}


GameObject::GameObject(Vector2* _vector, float _width, float _height) {
	this->vector = _vector;
	this->width = _width;
	this->height = _height;
	this->name = "default_name";
}


GameObject::GameObject(const char* _name, float _x, float _y, float _width, float _height) {
	this->vector = new Vector2(_x, _y);
	this->width = _width;
	this->height = _height;
	this->name = std::string(_name);
}


GameObject::~GameObject(void) {
	delete this->vector;
	this->vector = nullptr;
	this->width = 0.0f;
	this->height = 0.0f;
	this->name = nullptr;
}


void GameObject::update(void) {

}


void GameObject::render(void) {

}


int GameObject::getTileX(u16 tileWidth) {
	return (int)((this->vector->x + (this->width / 2)) / tileWidth);
}


int GameObject::getTileY(u16 tileHeight) {
	return (int)((this->vector->y + (this->height / 2)) / tileHeight);
}
