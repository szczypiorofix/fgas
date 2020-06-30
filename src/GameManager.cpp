/*
 * For Gold and Sweetrolls
 * Copyright (C) 2020 Piotr Wróblewski <szczypiorofix@o2.pl>
 */

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "GameManager.h"
#include "FontAssets.h"


GameManager::GameManager() {

    this->quit = false;

    this->engine = nullptr;

    this->shader = nullptr;

    this->mainMenu = nullptr;
    this->mainGame = nullptr;

    this->state = State::MAIN_MENU;

    this->mX = 200.0f;
    this->mY = -400.0f;

    this->moveX = 0;
    this->moveY = 0;

}


void GameManager::start() {

    this->engine = new Engine();
    this->engine->launch();

    //GraphicAssets::addToAssets("../res/images/spritesheet.png", 32, 32, GraphicAssets::IMAGE_ASSETS_BIG_SPRITESHEET);
    //GraphicAssets::addToAssets("../res/images/mm-gui-button.png", 168, 32, GraphicAssets::IMAGE_ASSETS_MAIN_MENU_BUTTONS);
    //GraphicAssets::addToAssets("../res/fonts/vingue.png", GraphicAssets::IMAGE_ASSETS_VINGUE_FONT);
    //GraphicAssets::addToAssets("../res/images/background.png", GraphicAssets::IMAGE_ASSETS_MAIN_MENU_BACKGROUND);
    GraphicAssets::addToAssets("../res/images/logo-title.png", GraphicAssets::IMAGE_ASSETS_LOGO);    

    //FontAssets::addToAssets("vingue", GraphicAssets::getAssets()->textures[GraphicAssets::IMAGE_ASSETS_VINGUE_FONT], FontAssets::FONT_ASSETS_VINGUE);

    this->engine->loadMusic("menu-music.ogg");
    this->engine->playMusic(0.1f);

    this->shader = new ShaderLoader();
    this->shader->compileShaders("vert.glsl", "frag.glsl");

    this->mainMenu = new MainMenu(this->state);
    this->mainGame = new MainGame(this->state);

    this->mainLoop();

}


void GameManager::input(SDL_Event& event) {

    while (SDL_PollEvent(&event) != 0) {
        if (event.type == SDL_QUIT) {
            this->quit = true;
        }
        if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a) {
                this->moveX = -1;
            }
            if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d) {
                this->moveX = 1;
            }
            if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_w) {
                this->moveY = 1;
            }
            if (event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_s) {
                this->moveY = -1;
            }
        }
        if (event.type == SDL_KEYUP) {
            if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a) {
                this->moveX = 0;
            }
            if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d) {
                this->moveX = 0;
            }
            if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_w) {
                this->moveY = 0;
            }
            if (event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_s) {
                this->moveY = 0;
            }
        }
        
        // ============== GAME STATE ==============

        switch (this->state) {
        case State::SPLASH_SCREEN:
            break;
        case State::MAIN_MENU:
            this->mainMenu->input(event);
            break;
        case State::GAME:
            this->mainGame->input(event);
            break;
        }
        
        // ========================================

    }

}


void GameManager::update() {
 
    switch (this->state) {
    case State::SPLASH_SCREEN:
        break;
    case State::MAIN_MENU:
        this->mainMenu->update();
        break;
    case State::GAME:
        this->mainGame->update();
        break;
    }

    // Check if quit
    if (this->state == State::QUIT) {
        this->quit = true;
    }

    if (this->moveX != 0) {
        if (this->moveX == -1) {
            this->mX -= 1.0f;
        }
        if (this->moveX == 1) {
            this->mX += 1.0f;
        }
    }
    if (this->moveY != 0) {
        if (this->moveY == -1) {
            this->mY -= 1.0f;
        }
        if (this->moveY == 1) {
            this->mY += 1.0f;
        }
    }

}


void GameManager::render() {

    glClear(GL_COLOR_BUFFER_BIT);
    glPushMatrix();
    glOrtho(0, engine->settings.screenWidth, engine->settings.screenHeight, 0.0f, -1.0f, 1.0f); // Set the matrix


    // ================================= Render Start =================================
    
    this->shader->use(GraphicAssets::getAssets()->textures[GraphicAssets::IMAGE_ASSETS_LOGO]->textureId);
    TextureRect s = {
        0,
        0,
        928,
        793
    };
    TextureRect d = {
        this->mX,
        this->mY,
        600,
        450
    };
    
    //GraphicAssets::getAssets()->textures[GraphicAssets::IMAGE_ASSETS_MAIN_MENU_BACKGROUND]->draw(s, d);

    s = {
        0,
        0,
        335,
        201
    };
    d = {
        250,
        -100,
        250,
        200
    };

    GraphicAssets::getAssets()->textures[GraphicAssets::IMAGE_ASSETS_LOGO]->draw(s, d);



    /*this->shader->unuse();*/


    //switch (this->state) {
    //case State::SPLASH_SCREEN:
    //    break;
    //case State::MAIN_MENU:
    //    this->mainMenu->render();
    //    break;
    //case State::GAME:
    //    this->mainGame->render();
    //    break;
    //}


    this->shader->unuse();

    // ================================== Render End ==================================

    glPopMatrix();
    SDL_GL_SwapWindow(this->engine->window);

}


void GameManager::mainLoop() {

    SDL_Event event;

    while (!this->quit) {

        this->input(event);
        this->update();
        this->render();

        SDL_Delay(1000 / 60);

    }

    delete this->shader;
    this->shader = nullptr;

    this->engine->stop(0);

}
