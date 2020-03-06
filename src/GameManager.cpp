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
}

void GameManager::start() {
    this->engine = new Engine();
    this->engine->launch();

    GraphicAssets::addToAssets("../res/images/spritesheet.png", 32, 32, GraphicAssets::IMAGE_ASSETS_BIG_SPRITESHEET);
    GraphicAssets::addToAssets("../res/images/mm-gui-button.png", 168, 32, GraphicAssets::IMAGE_ASSETS_MAIN_MENU_BUTTONS);
    GraphicAssets::addToAssets("../res/fonts/vingue.png", GraphicAssets::IMAGE_ASSETS_VINGUE_FONT);
    GraphicAssets::addToAssets("../res/images/logo-title.png", GraphicAssets::IMAGE_ASSETS_LOGO);

    FontAssets::addToAssets("vingue", GraphicAssets::getAssets()->textures[GraphicAssets::IMAGE_ASSETS_VINGUE_FONT], FontAssets::FONT_ASSETS_VINGUE);
    
    this->mainMenu = new MainMenu(this->state);
    this->mainGame = new MainGame(this->state);

    this->engine->loadMusic("menu-music.ogg");
    this->engine->playMusic(0.1f);

    this->shader = new ShaderLoader();
    this->shader->compileShaders("shader.vert", "shader.frag");

    this->mainLoop();
}


void GameManager::input(SDL_Event& event) {

    while (SDL_PollEvent(&event) != 0) {
        if (event.type == SDL_QUIT) {
            this->quit = true;
        }
        else {
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
        }
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

}


void GameManager::render() {
    glClear(GL_COLOR_BUFFER_BIT);
    glPushMatrix();
    glOrtho(0, engine->settings.screenWidth, engine->settings.screenHeight, 0.0f, -1.0f, 1.0f); // Set the matrix


    // ================================= Render Start =================================
    
    
   
    this->shader->use();
    //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (GLvoid*)(sizeof(GLubyte) * 1));
    
    //glDrawArrays(GL_TRIANGLES, 0, 3);
    //this->shader->unuse();



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



    //TextureRect s = {
    //    0,
    //    0,
    //    335,
    //    201
    //};
    //TextureRect d = {
    //    10,
    //    10,
    //    250,
    //    150
    //};

    //GraphicAssets::getAssets()->textures[GraphicAssets::IMAGE_ASSETS_LOGO]->draw(s, d);


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
    this->engine->stop(0);

}
