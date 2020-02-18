/*
 * For Gold and Sweetrolls
 * Copyright (C) 2020 Piotr Wróblewski <szczypiorofix@o2.pl>
 */

#pragma once

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <IL/il.h>
#include <string>

#include "Music.h"

typedef struct Settings {
	int screenWidth;
	int screenHeight;
	float scale;
	int fullScreen;
	float musicVolume;
} Settings;


class Engine {

public:
	
	Engine();

	void launch();
	void stop();

	Settings settings;

	SDL_Window* window;
	SDL_GLContext glContext;
	
	Music* getCurrentMusic(void);

	void loadMusic(std::string musicFile);
	bool playMusic(float volume);
	bool playMusic(void);
	bool stopMusic(void);
	bool pauseMusic(void);
	void releaseMusic(void);

private:

	Music* currentMusic;

	SDL_Cursor* systemCursor;
	SDL_Surface* cursorIcon;

	void setSystemCursor();

	void init();

	void initSDL(void);
	void initOGL(void);
	void initDevIL(void);
	void initBASS(void);

};

