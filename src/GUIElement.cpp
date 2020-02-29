/*
 * For Gold and Sweetrolls
 * Copyright (C) 2020 Piotr Wróblewski <szczypiorofix@o2.pl>
 */

#include "GUIElement.h"



GUIElement::GUIElement(float _x, float _y, float _width, float _height) : x(_x), y(_y), width(_width), height(_height) {
    this->text = "";
    this->listeners = { false };
}


GUIElement::GUIElement(std::string _text, float _x, float _y, float _width, float _height) : text(_text), x(_x), y(_y), width(_width), height(_height) {
    this->listeners = { false };
}


GUIElement::~GUIElement() {}
