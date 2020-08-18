/*
 * For Gold and Sweetrolls
 * Copyright (C) 2020 Piotr Wróblewski <szczypiorofix@o2.pl>
 */

#ifndef _MAINMENUBUTTON_H_
#define _MAINMENUBUTTON_H_
#pragma once

#include "GUIElement.h"
#include "Texture.h"
//#include "TextFont.h"


class MainMenuButton : public GUIElement {

public:

    MainMenuButton(std::string _text, float _x, float _y, float _width, float _height);

    void input(SDL_Event& event) override;
    void update() override;
    void draw() override;
    

private:

    Texture* buttonTexture;
    //TextFont* textFont;
};


#endif