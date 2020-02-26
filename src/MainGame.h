/*
 * For Gold and Sweetrolls
 * Copyright (C) 2020 Piotr Wróblewski <szczypiorofix@o2.pl>
 */

#ifndef _MAINGAME_H_
#define _MAINGAME_H_
#pragma once

#include <SDL2/SDL_events.h>
#include "Defines.h"
#include "Player.h"


class MainGame {

public:

    MainGame(State& _state);
    ~MainGame();

    void update();
    void input(SDL_Event& event);
    void render();

private:

    State& state;
    Player* player;

};

#endif
