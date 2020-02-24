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

}


MainMenu::~MainMenu() {}


void MainMenu::input(SDL_Event& event) {
    switch (event.key.keysym.sym) {
    case SDLK_ESCAPE:
        this->state = State::QUIT;
        break;
    }
}


void MainMenu::update() {

    this->torchAnimation->nextFrame();
    this->diamondAnimation->nextFrame();
    this->cardsAnimation->nextFrame();

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


    //this->bigSpriteSheet->drawTile(1665, 100, 100); // mouse cursor

    //glBegin(GL_TRIANGLES); //GL_POINTS, GL_LINES, GL_LINE_STRIP, GL_LINE_LOOP, GL_QUADS, GL_TRIANGLES, GL_POLIGON
    //    glColor3ub(255, 0, 0);
    //    glVertex2f(400, 200);
    //    glColor3ub(0, 255, 0);
    //    glVertex2f(600, 400);
    //    glColor3ub(0, 0, 255);
    //    glVertex2f(200, 400);
    //glEnd();

}
