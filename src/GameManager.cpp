/*
 * For Gold and Sweetrolls
 * Copyright (C) 2020 Piotr Wróblewski <szczypiorofix@o2.pl>
 */

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "GameManager.h"
#include <iostream>


GameManager::GameManager() {

    this->quit = false;

    this->engine = nullptr;

    this->shader = nullptr;

    this->mainMenu = nullptr;
    this->mainGame = nullptr;

    this->state = State::MAIN_MENU;

    this->mX = 0.0f;
    this->mY = 0.0f;

    this->moveX = 0;
    this->moveY = 0;

}


void GameManager::start() {

    this->engine = new CE::Engine();
    this->engine->launch();

    //CE::GraphicAssets::addToAssets("../res/images/spritesheet.png", 32, 32, CE::GraphicAssets::IMAGE_ASSETS_BIG_SPRITESHEET);
    //CE::GraphicAssets::addToAssets("../res/images/spritesheet.png", CE::GraphicAssets::IMAGE_ASSETS_BIG_SPRITESHEET);
    //CE::GraphicAssets::addToAssets("../res/images/mm-gui-button.png", 168, 32, CE::GraphicAssets::IMAGE_ASSETS_MAIN_MENU_BUTTONS);
    //CE::GraphicAssets::addToAssets("../res/fonts/vingue.png", CE::GraphicAssets::IMAGE_ASSETS_VINGUE_FONT);
    //CE::GraphicAssets::addToAssets("../res/images/background.png", CE::GraphicAssets::IMAGE_ASSETS_MAIN_MENU_BACKGROUND);
    CE::GraphicAssets::addToAssets("../res/images/logo-title.png", CE::GraphicAssets::IMAGE_ASSETS_LOGO);    

    CE::FontAssets::addToAssets("vingue", CE::GraphicAssets::getAssets()->textures[CE::GraphicAssets::IMAGE_ASSETS_VINGUE_FONT], CE::FontAssets::FONT_ASSETS_VINGUE);

    this->engine->loadMusic("menu-music.ogg");
    this->engine->playMusic(0.1f);

    this->shader = new CE::ShaderLoader();
    this->shader->compileShaders("vert.glsl", "frag.glsl");

    this->mainMenu = new MainMenu(this->state);
    this->mainGame = new MainGame(this->state);


    glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));
    vec = trans * vec;
    std::cout << vec.x << vec.y << vec.z << std::endl;


    this->mainLoop();

}


void GameManager::input(SDL_Event& event) {

    while (SDL_PollEvent(&event) != 0) {
        if (event.type == SDL_QUIT) {
            this->quit = true;
        }

        if (event.type == SDL_WINDOWEVENT) {
            switch (event.window.event) {
                case SDL_WINDOWEVENT_RESIZED:
                    
                    GLuint w = event.window.data1, h = event.window.data2;

                    GLint oldViewport[4];
                    glGetIntegerv(GL_VIEWPORT, oldViewport);
                    GLint oldX = oldViewport[0], oldY = oldViewport[1], oldW = oldViewport[2], oldH = oldViewport[3];
                    printf("Old viewport: %i : %i   %i : %i\n", oldX, oldY, oldW, oldH);
                    SDL_Log("New window : %dx%d", w, h);
                    
                    float ratio = (float)oldW / (float)oldH;

                    SDL_Log("Aspect ratio (%i:%i): %.5f \n", w, h, ratio);

                    //if (w != oldW) {
                    //    w = event.window.data1;
                    //    h = oldH * w / oldW;
                    //}
                    //

                    glViewport( 0, 0 , w, h);
               

            }
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
    
    //glOrtho(0, engine->settings.screenWidth, engine->settings.screenHeight, 0.0f, -1.0f, 1.0f); // Set the matrix


    // ================================= Render Start =================================
    
    this->shader->use();

    
    switch (this->state) {
    case State::SPLASH_SCREEN:
        break;
    case State::MAIN_MENU:
        this->mainMenu->render();
        break;
    case State::GAME:
        this->mainGame->render();
        break;
    }

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

        //SDL_Delay(1000 / 60);
    }

    delete this->shader;
    this->shader = nullptr;

    this->engine->stop(0);

}
