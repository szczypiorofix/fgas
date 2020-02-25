/*
 * For Gold and Sweetrolls
 * Copyright (C) 2020 Piotr Wróblewski <szczypiorofix@o2.pl>
 */

#ifndef _PLAYER_H_
#define _PLAYER_H_
#pragma once

#include "GameObject.h"


class Player : public GameObject {

public:
    Player();
    Player(Vector2* _vector, float _width, float _height);
    Player(float _x, float _y, float _width, float _height);
    ~Player();


    void update();
    void render();

};


#endif
