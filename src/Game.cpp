/*
 * For Gold and Sweetrolls
 * Copyright (C) 2020 Piotr Wróblewski <szczypiorofix@o2.pl>
 */

#include "Game.h"


Game::Game() {
	
    this->quit = false;
    this->engine = nullptr;
    this->backgroundTexture = nullptr;
    this->logoTexture = nullptr;

}

void Game::start() {
    
    this->engine = new Engine();
    this->engine->launch();

    GraphicAssets::addToAssets("../res/images/background.png", GraphicAssets::IMAGE_ASSETS_MAIN_MENU_BACKGROUND);
    GraphicAssets::addToAssets("../res/images/logo-title.png", GraphicAssets::IMAGE_ASSETS_LOGO);
    
    this->logoTexture = GraphicAssets::getAssets()->textures[GraphicAssets::IMAGE_ASSETS_LOGO];
    this->backgroundTexture = GraphicAssets::getAssets()->textures[GraphicAssets::IMAGE_ASSETS_MAIN_MENU_BACKGROUND];

    this->engine->loadMusic("menu-music.ogg");
    this->engine->playMusic(0.1f);

    this->shader = new ShaderLoader();
    this->shader->compileShaders("vert_shader.glsl", "frag_shader.glsl");
    this->shader->addAttribute("vertexUV");
    this->shader->linkShaders();

    this->mainLoop();

}


void Game::input(SDL_Event* event) {

    while (SDL_PollEvent(event) != 0) {
        if (event->type == SDL_QUIT) {
            this->quit = true;
        }
        else if (event->type == SDL_KEYDOWN) {
            switch (event->key.keysym.sym) {
            case SDLK_ESCAPE:
                this->quit = true;
                break;
            }
        }
    }
 
}


void Game::update() {

}


void Game::render() {
    glClear(GL_COLOR_BUFFER_BIT);
    glPushMatrix();
    glOrtho(0, engine->settings.screenWidth, engine->settings.screenHeight, 0, -1.0, 1.0); // Set the matrix

    // ================================= Render Start =================================

    TextureRect s = {
        0,
        0,
        928,
        792
    };
    TextureRect d = {
        0,
        0,
        800,
        600
    };

    this->backgroundTexture->draw(s, d);

    s = {
        0,
        0,
        335,
        201
    };
    d = {
        280,
        55,
        250,
        150
    };

    this->logoTexture->draw(s, d);


    glBegin(GL_TRIANGLES); //GL_POINTS, GL_LINES, GL_LINE_STRIP, GL_LINE_LOOP, GL_QUADS, GL_TRIANGLES, GL_POLIGON
        glColor3ub(255, 0, 0);
        glVertex2f(400, 200);
        glColor3ub(0, 255, 0);
        glVertex2f(600, 400);
        glColor3ub(0, 0, 255);
        glVertex2f(200, 400);
    glEnd();

    // ================================== Render End ==================================

    glPopMatrix();
    SDL_GL_SwapWindow(this->engine->window);
}


void Game::mainLoop() {

    SDL_Event event;

    while (!this->quit) {

        this->input(&event);
        this->update();
        this->render();

        SDL_Delay(1000 / 60);

    }

    this->engine->stop();

}


