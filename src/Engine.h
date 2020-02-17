/*
 * For Gold and Sweetrolls
 * Copyright (C) 2020 szczypiorofix <szczypiorofix@o2.pl>
 */

#pragma once

#define GLEW_STATIC
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
//#include <GL/GLU.h>
#include <SDL2/SDL_image.h>
#include <string>


class Engine {

public:
	
	static const int SCREEN_WIDTH = 800;
	static const int SCREEN_HEIGHT = 600;

	Engine();

	void launch();
	void stop();

	SDL_Window* window;
	SDL_GLContext glContext;

private:
	
	void init();

	void initSDL(void);
	void initOGL(void);

};

