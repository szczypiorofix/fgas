/*
 * For Gold and Sweetrolls
 * Copyright (C) 2020 Piotr Wróblewski <szczypiorofix@o2.pl>
 */

#ifndef _GUIELEMENT_H_
#define _GUIELEMENT_H_
#pragma once

#include <SDL2/SDL_events.h>
#include <string>


class GUIElement {

public:

    GUIElement(float _x, float _y, float _width, float _height);
    GUIElement(std::string& _text, float _x, float _y, float _width, float _height);

    ~GUIElement();

    virtual void input(SDL_Event& event) = 0;
    virtual void update() = 0;
    virtual void draw() = 0;

    typedef struct Listeners {
        bool onFocus;
        bool onFocusLost;
        bool onMouseButtonLeftDown;
        bool onMouseButtonLeftClicked;
        bool onMouseButtonRightDown;
        bool onMouseButtonRightClicked;
    } Listeners;

    Listeners listeners;

protected:
    std::string text;
    float x, y;
    float width, height;

};



#endif
