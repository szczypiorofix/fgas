/*
 * For Gold and Sweetrolls
 * Copyright (C) 2020 Piotr Wróblewski <szczypiorofix@o2.pl>
 */

#include "MainMenu.h"
#include "Engine.h"



MainMenu::MainMenu(State& _state) : state{ _state } {

    this->x = 10;
    this->y = 10;

    this->moveX = 0;
    this->moveY = 0;

    this->mapMoveDirectionX = -MAP_SPEED_X;
    this->mapMoveDirectionY = -MAP_SPEED_Y;

    this->bigSpriteSheet = nullptr;
    this->logoTexture = nullptr;
    this->backgroundTexture = nullptr;

    //this->mainMenuBackgroundMap = new TiledMap("mm_background.tmx");
    
    this->bigSpriteSheet = CE::GraphicAssets::getAssets()->textures[CE::GraphicAssets::IMAGE_ASSETS_BIG_SPRITESHEET];
    this->logoTexture = CE::GraphicAssets::getAssets()->textures[CE::GraphicAssets::IMAGE_ASSETS_LOGO];
    this->backgroundTexture = CE::GraphicAssets::getAssets()->textures[CE::GraphicAssets::IMAGE_ASSETS_MAIN_MENU_BACKGROUND];

    std::vector<u16> framesTorch = { TS_TORCH1, TS_TORCH2, TS_TORCH3, TS_TORCH4 };
    this->torchAnimation = new CE::Animation(4, 4, framesTorch);

    std::vector<u16> framesDiamond = { 79, 80, 81 };
    this->diamondAnimation = new CE::Animation(4, 3, framesDiamond);

    std::vector<u16> framesCards = { 84, 85, 86, 87, 88 };
    this->cardsAnimation = new CE::Animation(4, 5, framesCards);

    this->mainMenuButtons = {
        new MainMenuButton("NEW GAME",  340, 240, 168, 32),
        new MainMenuButton("QUIT GAME", 340, 300, 168, 32)
    };


}


MainMenu::~MainMenu() {}


void MainMenu::input(SDL_Event& event) {
    
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
        case SDLK_RIGHT:
        case SDLK_a:
        case SDLK_d:
            this->moveX = 0;
            break;
        case SDLK_UP:
        case SDLK_DOWN:
        case SDLK_w:
        case SDLK_s:
            this->moveY = 0;
            break;
        }
    }

    for (u16 i = 0; i < this->mainMenuButtons.size(); i++) {
        this->mainMenuButtons.at(i)->input(event);
    }

}


void MainMenu::update() {

    //this->x += this->mapMoveDirectionX;
    //this->y += this->mapMoveDirectionY;

    //if (this->x > 0 || this->y > 0) {
    //    this->mapMoveDirectionX = -MAP_SPEED_X;
    //    this->mapMoveDirectionY = -MAP_SPEED_Y;
    //}
    //if ( this->x < -( (this->mainMenuBackgroundMap->map->width * this->mainMenuBackgroundMap->map->tileWidth) - SCREEN_WIDTH ) ||
    //    this->y < -( (this->mainMenuBackgroundMap->map->height * this->mainMenuBackgroundMap->map->tileHeight) - SCREEN_HEIGHT )
    //    ) {
    //    this->mapMoveDirectionX = MAP_SPEED_X;
    //    this->mapMoveDirectionY = MAP_SPEED_Y;
    //}
    
    if (this->moveX == 1) {
        this->x += 5.0f;
    }
    if (this->moveX == -1) {
        this->x -= 5.0f;
    }

    if (this->moveY == 1) {
        this->y += 5.0f;
    }

    if (this->moveY == -1) {
        this->y -= 5.0f;
    }


    //this->torchAnimation->nextFrame();
    //this->diamondAnimation->nextFrame();
    //this->cardsAnimation->nextFrame();

    //if (this->mainMenuButtons.at(0)->listeners.onMouseButtonLeftClicked) {
    //    this->state = State::GAME;
    //}

    //if (this->mainMenuButtons.at(1)->listeners.onMouseButtonLeftClicked) {
    //    this->state = State::QUIT;
    //}


    // This need to be the last thing to update !
    //for (u16 i = 0; i < this->mainMenuButtons.size(); i++) {
    //    this->mainMenuButtons.at(i)->update();
    //}
    
}


void MainMenu::render() {
    
    //for (int l = 0; l < this->mainMenuBackgroundMap->map->layers.size(); l++) {
    //    for (int i = 0; i < this->mainMenuBackgroundMap->map->width * this->mainMenuBackgroundMap->map->height; i++) {
    //        int id = (this->mainMenuBackgroundMap->map->layers.at(l)->data.arr[i]) - 1;
    //        if (id > 0) {
    //            this->bigSpriteSheet->drawTile(
    //                id,
    //                this->x + (GLfloat)((i % this->mainMenuBackgroundMap->map->layers.at(l)->width) * this->mainMenuBackgroundMap->map->tileWidth),
    //                this->y + (GLfloat)((i / this->mainMenuBackgroundMap->map->layers.at(l)->width) * this->mainMenuBackgroundMap->map->tileHeight)
    //            );
    //        }
    //    }
    //}

    //TextureRect s = {
    //    0,
    //    0,
    //    928,
    //    793
    //};
    //TextureRect d = {
    //    0,
    //    0,
    //    800,
    //    600
    //};
    //this->backgroundTexture->draw();

    //s = {
    //    0,
    //    0,
    //    335,
    //    201
    //};
    //d = {
    //    240,
    //    10,
    //    335,
    //    201
    //};

    this->logoTexture->draw(this->x, this->y, 80.0f, 60.0f);

    //this->bigSpriteSheet->draw();

    //this->bigSpriteSheet->drawTile(this->torchAnimation->getTile(), 280, 250);
    //this->bigSpriteSheet->drawTile(this->torchAnimation->getTile(), 520, 250);

    //this->bigSpriteSheet->drawTile(this->diamondAnimation->getTile(), 280, 350);
    //this->bigSpriteSheet->drawTile(this->cardsAnimation->getTile(), 520, 350);

    //for (u16 i = 0; i < this->mainMenuButtons.size(); i++) {
    //    this->mainMenuButtons.at(i)->draw();
    //}

}
