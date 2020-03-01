/*
 * For Gold and Sweetrolls
 * Copyright (C) 2020 Piotr Wróblewski <szczypiorofix@o2.pl>
 */

#include "MainMenuButton.h"
#include "GraphicAssets.h"
#include "FontAssets.h"


//MainMenuButton::MainMenuButton() : GUIElement() {
//    
//    this->buttonTexture = nullptr;
//    this->listeners = { false };
//    this->textFont = FontAssets::getAssets()->fonts[FontAssets::FONT_ASSETS_VINGUE];
//
//}


MainMenuButton::MainMenuButton(std::string _text, float _x, float _y, float _width, float _height) : GUIElement(_text ,_x, _y, _width, _height) {
    
    this->buttonTexture = GraphicAssets::getAssets()->textures[GraphicAssets::IMAGE_ASSETS_MAIN_MENU_BUTTONS];
    this->listeners = { false };
    this->textFont = FontAssets::getAssets()->fonts[FontAssets::FONT_ASSETS_VINGUE];

}


void MainMenuButton::input(SDL_Event& event) {
    
    if (event.type == SDL_MOUSEMOTION) {
        if (event.motion.x > this->x&& event.motion.x < this->x + this->width && event.motion.y > this->y&& event.motion.y < this->y + this->height) {
            this->listeners.onFocus = true;
        } else {
            this->listeners.onFocus = false;
        }
    }
    if (this->listeners.onFocus) {
        if (event.button.type == SDL_MOUSEBUTTONDOWN) {
            if (event.button.button == SDL_BUTTON_LEFT) {
                this->listeners.onMouseButtonLeftDown = true;
            }
            if (event.button.button == SDL_BUTTON_RIGHT) {
                this->listeners.onMouseButtonRightDown = true;
            }
        } else if (event.button.type == SDL_MOUSEBUTTONUP) {
            if (event.button.button == SDL_BUTTON_LEFT) {
                this->listeners.onMouseButtonLeftDown = false;
                this->listeners.onMouseButtonLeftClicked = true;
            }
            if (event.button.button == SDL_BUTTON_RIGHT) {
                this->listeners.onMouseButtonRightDown = false;
                this->listeners.onMouseButtonRightClicked = true;
            }
        }
    } else if (event.button.type == SDL_MOUSEBUTTONUP) {
        if (event.button.button == SDL_BUTTON_LEFT) {
            this->listeners.onMouseButtonLeftDown = false;
        }
        if (event.button.button == SDL_BUTTON_RIGHT) {
            this->listeners.onMouseButtonRightDown = false;
        }
    }
 
}


void MainMenuButton::update() {
    this->listeners.onMouseButtonLeftClicked = false;
    this->listeners.onMouseButtonRightClicked = false;
}


void MainMenuButton::draw() {
    
    float yOffset = 0.0f;
    int tile = 0;
    if (this->listeners.onFocus) {
        tile = 1;
        yOffset = 1.0f;
        if (this->listeners.onMouseButtonLeftDown) {
            tile = 2;
            yOffset = 3.0f;
        }
    }
    this->buttonTexture->drawTile(tile, this->x, this->y);
    this->textFont->draw(this->text.c_str(), this->x + 28.0f, this->y + 8.0f + yOffset, 0.25f);

}


