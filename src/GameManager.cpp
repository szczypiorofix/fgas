/*
 * For Gold and Sweetrolls
 * Copyright (C) 2020 Piotr Wróblewski <szczypiorofix@o2.pl>
 */

#include "GameManager.h"


GameManager::GameManager() {
	
    this->quit = false;
    this->engine = nullptr;

    this->bigSpriteSheet = nullptr;
    this->torchAnimation = nullptr;
    this->lavaAnimation = nullptr;
    this->altarOfBloodAnimation = nullptr;
    this->firePlaceAnimation = nullptr;

    this->mainMenuBackgroundMap = nullptr;

    this->shader = nullptr;
    this->mainMenu = nullptr;

    this->state = State::MAIN_MENU;

}

void GameManager::start() {
    
    this->engine = new Engine();
    this->engine->launch();

    GraphicAssets::addToAssets("../res/images/spritesheet.png", 32, 32, GraphicAssets::IMAGE_ASSETS_BIG_SPRITESHEET);
    
    this->mainMenu = new MainMenu();

    this->bigSpriteSheet = GraphicAssets::getAssets()->textures[GraphicAssets::IMAGE_ASSETS_BIG_SPRITESHEET];

    this->engine->loadMusic("menu-music.ogg");
    this->engine->playMusic(0.1f);

    this->mainMenuBackgroundMap = new TiledMap("mm_background.tmx");



    // Animations

    unsigned int framesTorch[] = { 1359, 1360, 1361, 1362 };

    this->torchAnimation = new Animation(5, 4, framesTorch);
    this->torchAnimation->setCurrentFrame(0);


    unsigned int framesLava[] = { 380, 381, 382, 383 };
    //unsigned int framesLava[] = { 476, 477, 478, 479 };

    this->lavaAnimation = new Animation(8, 4, framesLava);
    this->lavaAnimation->setCurrentFrame(0);


    unsigned int framesAltarOfBlood[] = { 1, 2 };

    this->altarOfBloodAnimation = new Animation(7, 2, framesAltarOfBlood);
    this->altarOfBloodAnimation->setCurrentFrame(0);

    //unsigned int framesFirePlace[] = { 48, 50, 51, 52, 54, 53, 49, 47 };

    //this->firePlaceAnimation = new Animation(6, 8, framesFirePlace);
    //this->firePlaceAnimation->setCurrentFrame(0);


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
 
    this->torchAnimation->nextTile();
    this->lavaAnimation->nextTile();
    this->altarOfBloodAnimation->nextTile();
    //this->firePlaceAnimation->nextTile();

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

    for (int l = 0; l < this->mainMenuBackgroundMap->map.layerCounter; l++) {
    	for (int i = 0; i < this->mainMenuBackgroundMap->map.width * this->mainMenuBackgroundMap->map.height; i++) {
    		int id = (this->mainMenuBackgroundMap->map.layers[l]->data.arr[i]) - 1;
    		if (id > 0) {
                this->bigSpriteSheet->drawTile(
                    id,
                    (GLfloat)((i % this->mainMenuBackgroundMap->map.layers[l]->width) * this->mainMenuBackgroundMap->map.tileWidth),
                    (GLfloat)((i / this->mainMenuBackgroundMap->map.layers[l]->width) * this->mainMenuBackgroundMap->map.tileHeight)
                );
    		}			
    	}
    }


    //this->bigSpriteSheet->drawTile(this->firePlaceAnimation->getTile(), 300, 290);
    //this->bigSpriteSheet->drawTile(this->firePlaceAnimation->getTile(), 500, 290);

    this->bigSpriteSheet->drawTile(this->torchAnimation->getTile(), 200, 250);
    this->bigSpriteSheet->drawTile(this->torchAnimation->getTile(), 600, 250);

    this->bigSpriteSheet->drawTile(this->altarOfBloodAnimation->getTile(), 200, 290);
    this->bigSpriteSheet->drawTile(this->altarOfBloodAnimation->getTile(), 600, 290);


    this->bigSpriteSheet->drawTile(this->lavaAnimation->getTile(), 100, 100);
    this->bigSpriteSheet->drawTile(this->lavaAnimation->getTile(), 132, 100);
    this->bigSpriteSheet->drawTile(this->lavaAnimation->getTile(), 100, 132);
    this->bigSpriteSheet->drawTile(this->lavaAnimation->getTile(), 132, 132);

    

    //this->backgroundTexture->draw(s, d);
    //
   
    //s = {
    //    0,
    //    0,
    //    335,
    //    201
    //};
    //d = {
    //    280,
    //    55,
    //    250,
    //    150
    //};

    //this->logoTexture->draw(s, d);


    //s = {
    //    0,
    //    0,
    //    2048,
    //    3040
    //};
    //d = {
    //    10,
    //    10,
    //    512,
    //    512
    //};

    //this->bigSpriteSheet->draw(s, d);
    //this->bigSpriteSheet->drawTile(1665, 100, 100); // mouse cursor

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


