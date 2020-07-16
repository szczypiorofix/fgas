/*
 * For Gold and Sweetrolls
 * Copyright (C) 2020 Piotr Wróblewski <szczypiorofix@o2.pl>
 */

#ifndef _PLAYER_H_
#define _PLAYER_H_
#pragma once

#include "Engine.h"


class Player : public CE::GameObject {

public:
    Player();
    Player(Vector2* _vector, float _width, float _height);
    Player(float _x, float _y, float _width, float _height);
    Player(const char* _name, float _x, float _y, float _width, float _height);
    ~Player();


    void update() override;
    void render() override;

    const char* name;

private:
    

};


#endif
