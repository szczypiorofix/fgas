/*
 * For Gold and Sweetrolls
 * Copyright (C) 2020 Piotr Wróblewski <szczypiorofix@o2.pl>
 */

#include <glm/glm.hpp>
#include "GameManager.h"
#include <iostream>




GameManager::GameManager() {

    this->quit = false;

    this->engine = nullptr;

    this->shader = nullptr;

    this->mainMenu = nullptr;
    this->mainGame = nullptr;

    this->bigAtlas = nullptr;
    this->logoTexture = nullptr;

    this->camera = nullptr;

    this->state = State::MAIN_MENU;

    this->mX = 0.0f;
    this->mY = 0.0f;

    this->moveX = 0;
    this->moveY = 0;

}


void GameManager::start() {

    this->engine = new Engine();
    this->engine->launch();

    //GraphicAssets::addToAssets("../res/images/spritesheet.png", 32, 32, GraphicAssets::IMAGE_ASSETS_BIG_SPRITESHEET);
    //GraphicAssets::addToAssets("../res/images/spritesheet.png", GraphicAssets::IMAGE_ASSETS_BIG_SPRITESHEET);
    //GraphicAssets::addToAssets("../res/images/mm-gui-button.png", 168, 32, GraphicAssets::IMAGE_ASSETS_MAIN_MENU_BUTTONS);
    //GraphicAssets::addToAssets("../res/fonts/vingue.png", GraphicAssets::IMAGE_ASSETS_VINGUE_FONT);
    //GraphicAssets::addToAssets("../res/images/background.png", GraphicAssets::IMAGE_ASSETS_MAIN_MENU_BACKGROUND);
    GraphicAssets::addToAssets("../res/images/test1.png", GraphicAssets::IMAGE_ASSETS_LOGO);    

    this->logoTexture = GraphicAssets::getAssets()->textures[GraphicAssets::IMAGE_ASSETS_LOGO];

    this->bigAtlas = new SpriteSheet(this->logoTexture, 3, 4);



    this->camera = new Camera2D();
    this->camera->init(this->engine->settings.screenWidth, this->engine->settings.screenHeight);

    //FontAssets::addToAssets("vingue", GraphicAssets::getAssets()->textures[GraphicAssets::IMAGE_ASSETS_VINGUE_FONT], FontAssets::FONT_ASSETS_VINGUE);

    this->engine->loadMusic("menu-music.ogg");
    this->engine->playMusic(0.1f);

    this->shader = new ShaderLoader();
    this->shader->compileShaders("vert.glsl", "frag.glsl");

    //this->mainMenu = new MainMenu(this->state);
    //this->mainGame = new MainGame(this->state);

    //glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
    //glm::mat4 trans = glm::mat4(1.0f);
    //trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));
    //vec = trans * vec;
    //std::cout << vec.x << vec.y << vec.z << std::endl;


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

                    glViewport(0, 0, w, h);
            }
        }

        if (event.type == SDL_KEYDOWN) {

            switch (event.key.keysym.sym) {
            case SDLK_LEFT:
            case SDLK_a:
                this->moveX = -1;
                break;
            case SDLK_RIGHT:
            case SDLK_d:
                this->moveX = 1;
                break;
            case SDLK_UP:
            case SDLK_w:
                this->moveY = -1;
                break;
            case SDLK_DOWN:
            case SDLK_s:
                this->moveY = 1;
                break;
            }

        }
        if (event.type == SDL_KEYUP) {

            switch (event.key.keysym.sym) {
            case SDLK_ESCAPE:
                this->state = State::QUIT;
                break;
            case SDLK_LEFT:
            case SDLK_a:
            case SDLK_RIGHT:
            case SDLK_d:
                this->moveX = 0;
                break;
            case SDLK_UP:
            case SDLK_w:
            case SDLK_DOWN:
            case SDLK_s:
                this->moveY = 0;
                break;
            }

        }

        // ZOOM
        if (event.type == SDL_MOUSEWHEEL) {
            if (event.button.x == 1) {
                this->camera->setScale(this->camera->getScale() + SCALE_SPEED);
            } else if (event.button.x == -1) {
                this->camera->setScale(this->camera->getScale() - SCALE_SPEED);
            }
        }
        
        // ============== GAME STATE ==============

        //switch (this->state) {
        //case State::SPLASH_SCREEN:
        //    break;
        //case State::MAIN_MENU:
        //    this->mainMenu->input(event);
        //    break;
        //case State::GAME:
        //    this->mainGame->input(event);
        //    break;
        //}
        
        // ========================================

    }

}


void GameManager::update() {
 
    //switch (this->state) {
    //case State::SPLASH_SCREEN:
    //    break;
    //case State::MAIN_MENU:
    //    this->mainMenu->update();
    //    break;
    //case State::GAME:
    //    this->mainGame->update();
    //    break;
    //}


    // Check if quit
    if (this->state == State::QUIT) {
        this->quit = true;
    }


    if (this->moveX == 1) {
        this->camera->setPosition(this->camera->getPosition() + glm::vec2(CAMERA_SPEED, 0.0f));
    }
    if (this->moveX == -1) {
        this->camera->setPosition(this->camera->getPosition() + glm::vec2(-CAMERA_SPEED, 0.0f));
    }

    if (this->moveY == 1) {
        this->camera->setPosition(this->camera->getPosition() + glm::vec2(0.0f, -CAMERA_SPEED));
    }

    if (this->moveY == -1) {
        this->camera->setPosition(this->camera->getPosition() + glm::vec2(0.0f, CAMERA_SPEED));
    }

    this->camera->update();

}


void GameManager::render() {

    glClear(GL_COLOR_BUFFER_BIT);
    glPushMatrix();
    
    //glOrtho(0, engine->settings.screenWidth, engine->settings.screenHeight, 0.0f, -1.0f, 1.0f); // Set the matrix


    // ================================= Render Start =================================
    
    this->shader->use();

    
    this->logoTexture->draw(this->camera->getCameraMatrix());

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

        //SDL_Delay(1000 / 60);
    }

    delete this->shader;
    this->shader = nullptr;

    this->engine->stop(0);

}
