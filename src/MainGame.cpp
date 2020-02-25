/*
 * For Gold and Sweetrolls
 * Copyright (C) 2020 Piotr Wróblewski <szczypiorofix@o2.pl>
 */

#include "MainGame.h"
#include "GraphicAssets.h"

MainGame::MainGame(State& _state) : state{ _state } {
    this->player = new Player();
}


MainGame::~MainGame() {}


void MainGame::update() {

}


void MainGame::input(SDL_Event& event) {
    switch (event.key.keysym.sym) {
    case SDLK_1:
        this->state = State::MAIN_MENU;
        break;
    }
}


void MainGame::render() {
    glBegin(GL_TRIANGLES); //GL_POINTS, GL_LINES, GL_LINE_STRIP, GL_LINE_LOOP, GL_QUADS, GL_TRIANGLES, GL_POLIGON
        glColor3ub(255, 0, 0);
        glVertex2f(400, 200);
        glColor3ub(0, 255, 0);
        glVertex2f(600, 400);
        glColor3ub(0, 0, 255);
        glVertex2f(200, 400);
    glEnd();
}

