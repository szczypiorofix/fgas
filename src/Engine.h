/*
 * For Gold and Sweetrolls
 * Copyright (C) 2020 Piotr Wróblewski <szczypiorofix@o2.pl>
 */
#ifndef _ENGINE_H_
#define _ENGINE_H_
#pragma once

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <IL/il.h>
#include <string>

#include "Defines.h"
#include "Music.h"

typedef struct Settings {
	u16 screenWidth;
	u16 screenHeight;
	float scale;
	bool fullScreen;
	bool windowResizable;
	float musicVolume;
} Settings;


class Engine {

public:
	
	Engine();

	void launch(void);
	void stop();
	void stop(s16 _exitCode);

	Settings settings;

	SDL_Window* window;
	SDL_GLContext glContext;
	
	s16 exitCode;

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

	void setSystemCursor(void);

	void init(void);

	void initSDL(void);
	void initOGL(void);
	void initDevIL(void);
	void deviLSettings(void);
	void initBASS(void);

};

#endif
