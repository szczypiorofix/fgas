/*
 * For Gold and Sweetrolls
 * Copyright (C) 2020 Piotr Wróblewski <szczypiorofix@o2.pl>
 */

#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_
#pragma once


#include "Defines.h"


class GameObject {

public:
    GameObject();
    GameObject(Vector2* _vector, float _width, float _height);
    ~GameObject();

    virtual void update() = 0;
    virtual void render() = 0;

    int getTileX(u16 tileWidth);
    int getTileY(u16 tileHeight);

    float width, height;
    Vector2* vector;

protected:

};


#endif
