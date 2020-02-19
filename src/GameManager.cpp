/*
 * For Gold and Sweetrolls
 * Copyright (C) 2020 Piotr Wróblewski <szczypiorofix@o2.pl>
 */

#include "GameManager.h"


GameManager::GameManager() {
	
    this->quit = false;
    this->engine = nullptr;
    
    this->backgroundTexture = nullptr;
    this->logoTexture = nullptr;
    this->bigSpriteSheet = nullptr;

    this->shader = nullptr;
    this->mainMenu = nullptr;

    this->state = State::MAIN_MENU;


}

void GameManager::start() {
    
    this->engine = new Engine();
    this->engine->launch();

    GraphicAssets::addToAssets("../res/images/background.png", GraphicAssets::IMAGE_ASSETS_MAIN_MENU_BACKGROUND);
    GraphicAssets::addToAssets("../res/images/logo-title.png", GraphicAssets::IMAGE_ASSETS_LOGO);
    GraphicAssets::addToAssets("../res/images/ProjectUtumno_full.png", 32, 32, GraphicAssets::IMAGE_ASSETS_BIG_SPRITESHEET); // 64 x 95 tiles
    
    this->mainMenu = new MainMenu();

    this->logoTexture = GraphicAssets::getAssets()->textures[GraphicAssets::IMAGE_ASSETS_LOGO];
    this->backgroundTexture = GraphicAssets::getAssets()->textures[GraphicAssets::IMAGE_ASSETS_MAIN_MENU_BACKGROUND];
    this->bigSpriteSheet = GraphicAssets::getAssets()->textures[GraphicAssets::IMAGE_ASSETS_BIG_SPRITESHEET];

    this->engine->loadMusic("menu-music.ogg");
    this->engine->playMusic(0.1f);

    this->shader = new ShaderLoader();
    //this->shader->compileShaders("vert_shader.glsl", "frag_shader.glsl");
    //this->shader->addAttribute("LVertexPos2D");
    //this->shader->linkShaders();

    this->mainLoop();

}


void GameManager::input(SDL_Event& event) {

    while (SDL_PollEvent(&event) != 0) {
        if (event.type == SDL_QUIT) {
            this->quit = true;
        }
        else if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
            case SDLK_ESCAPE:
                this->quit = true;
                break;
            case SDLK_SPACE:
                GraphicAssets::getAssets()->releaseAssets();
                break;
            }
        }

        switch (this->state) {
        case State::SPLASH_SCREEN:
            break;
        case State::MAIN_MENU:
            this->mainMenu->input(event);
            break;
        case State::GAME:
            break;
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
        break;
    }

}


void GameManager::render() {
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


    s = {
        0,
        0,
        2048,
        3040
    };
    d = {
        10,
        10,
        512,
        512
    };

    this->bigSpriteSheet->drawTile(1665, 100, 100); // mouse cursor

    //glBegin(GL_TRIANGLES); //GL_POINTS, GL_LINES, GL_LINE_STRIP, GL_LINE_LOOP, GL_QUADS, GL_TRIANGLES, GL_POLIGON
    //    glColor3ub(255, 0, 0);
    //    glVertex2f(400, 200);
    //    glColor3ub(0, 255, 0);
    //    glVertex2f(600, 400);
    //    glColor3ub(0, 0, 255);
    //    glVertex2f(200, 400);
    //glEnd();


    switch (this->state) {
    case State::SPLASH_SCREEN:
        break;
    case State::MAIN_MENU:
        this->mainMenu->render();
        break;
    case State::GAME:
        break;
    }



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

    this->engine->stop();

}


