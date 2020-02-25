/*
 * For Gold and Sweetrolls
 * Copyright (C) 2020 Piotr Wróblewski <szczypiorofix@o2.pl>
 */

#include "Player.h"


Player::Player() : GameObject() {

}


Player::Player(Vector2* _vector, float _width, float _height) : GameObject(_vector, _width, _height) {

}


Player::Player(float _x, float _y, float _width, float _height) : GameObject(new Vector2(_x, _y), _width, _height) {

}


Player::~Player() {}


void Player::update() {

}


void Player::render() {

}

