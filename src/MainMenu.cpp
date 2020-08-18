/*
 * For Gold and Sweetrolls
 * Copyright (C) 2020 Piotr Wróblewski <szczypiorofix@o2.pl>
 */

#include "MainMenu.h"
#include "AssetsLoader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


MainMenu::MainMenu(State& _state) : state{ _state },
    gravity(0.0f, -10.0f),
    world(gravity),
    groundBody(),
    groundBodyDef(),
    groundBox(),
    bodyDef(),
    body(),
    dynamicBox(),
    fixtureDef()
{

    //this->camera = new Camera2D();

    //this->camera->init(800, 600);

    this->x = 0;
    this->y = 0;

    this->moveX = 0;
    this->moveY = 0;

    this->bigSpriteSheet = nullptr;
    this->logoTexture = nullptr;
    this->backgroundTexture = nullptr;

    //this->mainMenuBackgroundMap = new TiledMap("mm_background.tmx");
    
    this->bigSpriteSheet = GraphicAssets::getAssets()->textures[GraphicAssets::IMAGE_ASSETS_BIG_SPRITESHEET];
    this->logoTexture = GraphicAssets::getAssets()->textures[GraphicAssets::IMAGE_ASSETS_LOGO];
    this->backgroundTexture = GraphicAssets::getAssets()->textures[GraphicAssets::IMAGE_ASSETS_MAIN_MENU_BACKGROUND];

    std::vector<u16> framesTorch = { TS_TORCH1, TS_TORCH2, TS_TORCH3, TS_TORCH4 };
    this->torchAnimation = new Animation(4, 4, framesTorch);

    std::vector<u16> framesDiamond = { 79, 80, 81 };
    this->diamondAnimation = new Animation(4, 3, framesDiamond);

    std::vector<u16> framesCards = { 84, 85, 86, 87, 88 };
    this->cardsAnimation = new Animation(4, 5, framesCards);

    this->mainMenuButtons = {
        new MainMenuButton("NEW GAME",  340, 240, 168, 32),
        new MainMenuButton("QUIT GAME", 340, 300, 168, 32)
    };

    // Box2D

    this->groundBodyDef.position.Set(0.0f, -10.0f);
    
    this->groundBody = world.CreateBody(&groundBodyDef);

    this->groundBox.SetAsBox(50.0f, 10.f);

    this->groundBody->CreateFixture(&this->groundBox, 0.0f);

    this->bodyDef.type = b2_dynamicBody;
    this->bodyDef.position.Set(0.0f, 4.0f);
    this->body = this->world.CreateBody(&this->bodyDef);

    this->dynamicBox.SetAsBox(1.0f, 1.0f);
    this->fixtureDef.shape = &this->dynamicBox;
    this->fixtureDef.density = 1.0f;
    this->fixtureDef.friction = 0.75f;
    this->fixtureDef.restitution = 0.5f;

    this->body->CreateFixture(&this->fixtureDef);

    this->timeStep = 1.0f / 60.0f;

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
        case SDLK_SPACE:
            b2Vec2 lv(0.0f, 4.0f);
            // JUMP !!
            this->body->SetLinearVelocity(lv);
            break;
        }
    }
    // ZOOM
    if (event.type == SDL_MOUSEWHEEL) {
        if (event.button.x == 1) {
            //this->camera->setScale(this->camera->getScale() + SCALE_SPEED);
        }
        else if (event.button.x == -1) {
            //this->camera->setScale(this->camera->getScale() - SCALE_SPEED);
        }
    }

    for (u16 i = 0; i < this->mainMenuButtons.size(); i++) {
        this->mainMenuButtons.at(i)->input(event);
    }
}


void MainMenu::update() {
    
    if (this->moveX == 1) {
        //this->x += 5.0f;
        //b2Vec2 lv(1.0f, this->body->GetLinearVelocity().y);
        //this->body->SetLinearVelocity(lv);

        //this->camera->setPosition(this->camera->getPosition() + glm::vec2(CAMERA_SPEED, 0.0f));
    }
    if (this->moveX == -1) {
        //this->x -= 5.0f;
        //b2Vec2 lv(-1.0f, this->body->GetLinearVelocity().y);
        //this->body->SetLinearVelocity(lv);

        //this->camera->setPosition(this->camera->getPosition() + glm::vec2(-CAMERA_SPEED, 0.0f));
    }

    if (this->moveY == 1) {
        //this->y += 5.0f;

        //this->camera->setPosition(this->camera->getPosition() + glm::vec2(0.0f, -CAMERA_SPEED));
    }

    if (this->moveY == -1) {
        //this->y -= 5.0f;

        //this->camera->setPosition(this->camera->getPosition() + glm::vec2(0.0f, CAMERA_SPEED));
    }


    //this->camera->update();

    // Box2D

    this->world.Step(this->timeStep, this->velocityIterations, this->positionIterations);

    //b2Vec2 position = this->body->GetPosition();
    //float angle = this->body->GetAngle();
    //printf("%4.2f %4.2f %4.2f\n", position.x, position.y, angle);


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
    
    b2Vec2 position = this->body->GetPosition();
    //Vector2 position = Vector2(this->x, this->y);
    
    //this->logoTexture->draw(position.x * 180.0f, 400.0f - (position.y * 100.0f), 400.0f, 300.0f);

    //this->logoTexture->draw(this->camera->getPositionX(), this->camera->getPositionY(), 400.0f, 300.0f);

    // HIGHLY EXPERIMENTAL !!!
    //glm::vec3 v(position.x, position.y, 0.0f);
    //glm::mat4 r = glm::translate(this->camera->getCameraMatrix(), v);

    
    //this->backgroundTexture->draw(this->camera->getCameraMatrix());

    //glm::mat2 tm = glm::mat2(0.0f);
    //tm[0][0] = 1.0f;
    //tm[1][1] = 1.0f;
    //tm[0][1] = 0.0f;
    //tm[1][0] = 0.0f;
    

    //this->logoTexture->draw(this->camera->getCameraMatrix());





    //this->bigSpriteSheet->draw();

    //this->bigSpriteSheet->drawTile(this->torchAnimation->getTile(), 280, 250);
    //this->bigSpriteSheet->drawTile(this->torchAnimation->getTile(), 520, 250);

    //this->bigSpriteSheet->drawTile(this->diamondAnimation->getTile(), 280, 350);
    //this->bigSpriteSheet->drawTile(this->cardsAnimation->getTile(), 520, 350);

    //for (u16 i = 0; i < this->mainMenuButtons.size(); i++) {
    //    this->mainMenuButtons.at(i)->draw();
    //}

}
