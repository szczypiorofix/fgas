/*
 * For Gold and Sweetrolls
 * Copyright (C) 2020 Piotr Wróblewski <szczypiorofix@o2.pl>
 */

#include "MainGame.h"
#include "GraphicAssets.h"
#include "LuaHandler.h"


MainGame::MainGame(State& _state) : state{ _state } {
    

    LuaHandler* lua = new LuaHandler("script.lua");
    
    //int value = 0;
    //lua->getInt("value", value);
    //printf("Value from LUA script: %i\n", value);

    this->player = lua->getPlayer();
    if (this->player) {
	    std::cout << "Object 'Player' was found. Player name: " << this->player->name << std::endl;
	    std::cout << "X: " << this->player->vector->x << ", Y: " << this->player->vector->y << ", player width: " << this->player->width << ", height: " << this->player->height << std::endl;
    } else {
	    std::cout << "Cannot read object 'Player'." << std::endl;
    }
    delete lua;


}


MainGame::~MainGame() {}


void MainGame::update() {

}


void MainGame::input(SDL_Event& event) {
    if (event.type == SDL_KEYUP) {
        switch (event.key.keysym.sym) {
        case SDLK_ESCAPE:
            this->state = State::MAIN_MENU;
            break;
        }
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

