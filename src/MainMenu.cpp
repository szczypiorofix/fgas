/*
 * For Gold and Sweetrolls
 * Copyright (C) 2020 Piotr Wróblewski <szczypiorofix@o2.pl>
 */

#include "MainMenu.h"
#include "GraphicAssets.h"



MainMenu::MainMenu(State& _state) : state{ _state } {

    this->bigSpriteSheet = nullptr;

    this->mainMenuBackgroundMap = new TiledMap("mm_background.tmx");
    
    this->bigSpriteSheet = GraphicAssets::getAssets()->textures[GraphicAssets::IMAGE_ASSETS_BIG_SPRITESHEET];    

    std::vector<u16> framesTorch = { TS_TORCH1, TS_TORCH2, TS_TORCH3, TS_TORCH4 };
    this->torchAnimation = new Animation(4, 4, framesTorch);

    std::vector<u16> framesDiamond = { 79, 80, 81 };
    this->diamondAnimation = new Animation(4, 3, framesDiamond);

    std::vector<u16> framesCards = { 84, 85, 86, 87, 88 };
    this->cardsAnimation = new Animation(4, 5, framesCards);

    std::string s = "NEW GAME";

    this->mainMenuButtons = {
        new MainMenuButton(s ,260, 200, 168, 32)
    };

}


MainMenu::~MainMenu() {}


void MainMenu::input(SDL_Event& event) {
    switch (event.key.keysym.sym) {
    case SDLK_ESCAPE:
        this->state = State::QUIT;
        break;
    case SDLK_2:
        this->state = State::GAME;
        break;
    }

    for (u16 i = 0; i < this->mainMenuButtons.size(); i++) {
        this->mainMenuButtons.at(i)->input(event);
    }

}


void MainMenu::update() {

    this->torchAnimation->nextFrame();
    this->diamondAnimation->nextFrame();
    this->cardsAnimation->nextFrame();

    for (u16 i = 0; i < this->mainMenuButtons.size(); i++) {
        this->mainMenuButtons.at(i)->update();
    }
    
}


void MainMenu::render() {
    
    for (int l = 0; l < this->mainMenuBackgroundMap->map->layers.size(); l++) {
        for (int i = 0; i < this->mainMenuBackgroundMap->map->width * this->mainMenuBackgroundMap->map->height; i++) {
            int id = (this->mainMenuBackgroundMap->map->layers.at(l)->data.arr[i]) - 1;
            if (id > 0) {
                this->bigSpriteSheet->drawTile(
                    id,
                    (GLfloat)((i % this->mainMenuBackgroundMap->map->layers.at(l)->width) * this->mainMenuBackgroundMap->map->tileWidth),
                    (GLfloat)((i / this->mainMenuBackgroundMap->map->layers.at(l)->width) * this->mainMenuBackgroundMap->map->tileHeight)
                );
            }
        }
    }

    this->bigSpriteSheet->drawTile(this->torchAnimation->getTile(), 200, 250);
    this->bigSpriteSheet->drawTile(this->torchAnimation->getTile(), 600, 250);

    this->bigSpriteSheet->drawTile(this->diamondAnimation->getTile(), 300, 350);
    this->bigSpriteSheet->drawTile(this->cardsAnimation->getTile(), 500, 350);


    for (u16 i = 0; i < this->mainMenuButtons.size(); i++) {
        this->mainMenuButtons.at(i)->draw();
    }

    //this->bigSpriteSheet->drawTile(1665, 100, 100); // mouse cursor

}
