/*
 * For Gold and Sweetrolls
 * Copyright (C) 2020 Piotr Wr�blewski <szczypiorofix@o2.pl>
 */


#include "MainMenu.h"
#include "GraphicAssets.h"
#include "Tileset.h"


MainMenu::MainMenu(State& _state, Engine* _engine) : state{ _state }, engine{ _engine } {

    this->bigSpriteSheet = nullptr;

    this->mainMenuBackgroundMap = new TiledMap("mm_background.tmx");
    this->bigSpriteSheet = GraphicAssets::getAssets()->textures[GraphicAssets::IMAGE_ASSETS_BIG_SPRITESHEET];
    
    std::vector<CUINT> framesTorch = { TS_TORCH1, TS_TORCH2, TS_TORCH3, TS_TORCH4 };
    this->torchAnimation = new Animation(4, 4, framesTorch);

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

}




void MainMenu::render() {
    
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

    this->bigSpriteSheet->drawTile(this->torchAnimation->getTile(), 200, 250);

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

