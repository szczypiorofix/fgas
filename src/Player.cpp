/*
 * For Gold and Sweetrolls
 * Copyright (C) 2020 Piotr Wróblewski <szczypiorofix@o2.pl>
 */

#include "Player.h"


Player::Player() : GameObject() {
    this->name = "default_name";
}


Player::Player(Vector2* _vector, float _width, float _height) : GameObject(_vector, _width, _height) {
    this->name = "default_name";
}


Player::Player(float _x, float _y, float _width, float _height) : GameObject(new Vector2(_x, _y), _width, _height) {
    this->name = "default_name";
}


Player::Player(const char* _name, float _x, float _y, float _width, float _height) : GameObject(new Vector2(_x, _y), _width, _height) {
    this->name = _name;
}


Player::~Player() {}


void Player::update() {

}


void Player::render() {

}

