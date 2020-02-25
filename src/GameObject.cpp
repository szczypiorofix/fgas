/*
 * For Gold and Sweetrolls
 * Copyright (C) 2020 Piotr Wróblewski <szczypiorofix@o2.pl>
 */

#include <stdio.h>
#include "GameObject.h"


GameObject::GameObject() {
    this->vector = new Vector2();
    this->width = 0.0f;
    this->height = 0.0f;
}


GameObject::GameObject(Vector2* _vector, float _width, float _height) {
    this->vector = _vector;
    this->width = _width;
    this->height = _height;
}


GameObject::~GameObject() {}


int GameObject::getTileX(u16 tileWidth) {
    return (int)((this->vector->x + (this->width / 2)) / tileWidth);
}


int GameObject::getTileY(u16 tileHeight) {
    return (int)((this->vector->y + (this->height / 2)) / tileHeight);
}






