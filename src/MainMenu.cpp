/*
 * For Gold and Sweetrolls
 * Copyright (C) 2020 Piotr Wróblewski <szczypiorofix@o2.pl>
 */

#include "MainMenu.h"
#include "GraphicAssets.h"



MainMenu::MainMenu(State& _state) : state{ _state } {

    this->x = 0.0f;
    this->y = 0.0f;
    this->mapMoveDirectionX = -MAP_SPEED_X;
    this->mapMoveDirectionY = -MAP_SPEED_Y;

    this->bigSpriteSheet = nullptr;

    this->mainMenuBackgroundMap = new TiledMap("mm_background.tmx");
    
    this->bigSpriteSheet = GraphicAssets::getAssets()->textures[GraphicAssets::IMAGE_ASSETS_BIG_SPRITESHEET];
    this->logoTexture = GraphicAssets::getAssets()->textures[GraphicAssets::IMAGE_ASSETS_LOGO];

    std::vector<u16> framesTorch = { TS_TORCH1, TS_TORCH2, TS_TORCH3, TS_TORCH4 };
    this->torchAnimation = new Animation(4, 4, framesTorch);

    std::vector<u16> framesDiamond = { 79, 80, 81 };
    this->diamondAnimation = new Animation(4, 3, framesDiamond);

    std::vector<u16> framesCards = { 84, 85, 86, 87, 88 };
    this->cardsAnimation = new Animation(4, 5, framesCards);

  

    this->mainMenuButtons = {
        new MainMenuButton("NEW GAME" ,332, 200, 168, 32),
        new MainMenuButton("QUIT GAME" ,332, 260, 168, 32)
    };


}


MainMenu::~MainMenu() {}


void MainMenu::input(SDL_Event& event) {
    
    if (event.type == SDL_KEYUP) {
        switch (event.key.keysym.sym) {
        case SDLK_ESCAPE:
            this->state = State::QUIT;
            break;
        }
    }

    for (u16 i = 0; i < this->mainMenuButtons.size(); i++) {
        this->mainMenuButtons.at(i)->input(event);
    }

}


void MainMenu::update() {

    this->x += this->mapMoveDirectionX;
    this->y += this->mapMoveDirectionY;

    if (this->x > 0 || this->y > 0) {
        this->mapMoveDirectionX = -MAP_SPEED_X;
        this->mapMoveDirectionY = -MAP_SPEED_Y;
    }
    if ( this->x < -( (this->mainMenuBackgroundMap->map->width * this->mainMenuBackgroundMap->map->tileWidth) - SCREEN_WIDTH ) ||
        this->y < -( (this->mainMenuBackgroundMap->map->height * this->mainMenuBackgroundMap->map->tileHeight) - SCREEN_HEIGHT )
        ) {
        this->mapMoveDirectionX = MAP_SPEED_X;
        this->mapMoveDirectionY = MAP_SPEED_Y;
    }
    
    

    this->torchAnimation->nextFrame();
    this->diamondAnimation->nextFrame();
    this->cardsAnimation->nextFrame();

    if (this->mainMenuButtons.at(0)->listeners.onMouseButtonLeftClicked) {
        this->state = State::GAME;
    }

    if (this->mainMenuButtons.at(1)->listeners.onMouseButtonLeftClicked) {
        this->state = State::QUIT;
    }



    // This need to be the last thing to update !
    for (u16 i = 0; i < this->mainMenuButtons.size(); i++) {
        this->mainMenuButtons.at(i)->update();
    }
    
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

    //this->bigSpriteSheet->drawTile(this->torchAnimation->getTile(), 280, 250);
    //this->bigSpriteSheet->drawTile(this->torchAnimation->getTile(), 520, 250);

    //this->bigSpriteSheet->drawTile(this->diamondAnimation->getTile(), 300, 350);
    //this->bigSpriteSheet->drawTile(this->cardsAnimation->getTile(), 500, 350);


    //for (u16 i = 0; i < this->mainMenuButtons.size(); i++) {
    //    this->mainMenuButtons.at(i)->draw();
    //}

    TextureRect s = {
        0,
        0,
        335,
        201
    };
    TextureRect d = {
        280,
        25,
        250,
        150
    };
    
    this->logoTexture->draw(s, d);

    //this->bigSpriteSheet->drawTile(1665, 100, 100); // mouse cursor

}
