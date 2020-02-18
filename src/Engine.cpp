/*
 * For Gold and Sweetrolls
 * Copyright (C) 2020 Piotr Wróblewski <szczypiorofix@o2.pl>
 */

#define _CRT_SECURE_NO_WARNINGS

#include "Engine.h"
#include "Defines.h"
#include "GraphicAssets.h"


Engine::Engine() {

	this->window = nullptr;
	this->glContext = nullptr;
	this->currentMusic = nullptr;
	this->cursorIcon = nullptr;
	this->systemCursor = nullptr;

	this->settings = {
		SCREEN_WIDTH,					// Screen width
		SCREEN_HEIGHT,					// Screen height
		MIN_SCALE,						// scale
		0,								// 1 - fullscreen, 0 - window
		0.5f							// music volume
	};

}


void Engine::launch() {
	
	this->init();

}

void Engine::stop() {
#ifdef _DEBUG 
	printf("Shutting down SDL modules.\n");
#endif

	GraphicAssets::getAssets()->releaseAssets();

	SDL_GL_DeleteContext(this->glContext);
	SDL_DestroyWindow(this->window);

	delete this->currentMusic;
	
	if (this->systemCursor != NULL) {
		SDL_FreeCursor(this->systemCursor);
	}
	
	SDL_Quit();
}


void Engine::setSystemCursor() {
#ifdef _DEBUG 
	printf("Initializing system cursor.\n");
#endif
	int m = SDL_SetRelativeMouseMode(SDL_FALSE); // Trap mouse on window
	if (m == -1) {
		printf("Warning! Error while locking mouse pointer to the window.\n");
	}
	SDL_WarpMouseInWindow(this->window, this->settings.screenWidth / 2, this->settings.screenHeight / 2);
	

	GraphicAssets::getAssets()->addToAssets("../res/images/mouse_cursor.png", GraphicAssets::IMAGE_ASSETS_MOUSE_CURSOR);

	Texture* texture = GraphicAssets::getAssets()->textures[GraphicAssets::IMAGE_ASSETS_MOUSE_CURSOR];
	int width = (int)texture->width;
	int height = (int)texture->height;

	// Little endian
	Uint32 rmask = 0x000000ff;
	Uint32 gmask = 0x0000ff00;
	Uint32 bmask = 0x00ff0000;
	Uint32 amask = 0xff000000;

	int depth = texture->bitsPerPixel; // 3 colors & alpha, 8 bytes each
	int pitch = texture->bytesPerPixel * width;

	this->cursorIcon = SDL_CreateRGBSurfaceFrom((void*)texture->data, width, height, depth, pitch, rmask, gmask, bmask, amask);
	if (this->cursorIcon == NULL) {
		printf("Unable to Create RPG surface!\n");
		exit(1);
	}
	else {
		this->systemCursor = SDL_CreateColorCursor(this->cursorIcon, 0, 0);
		if (this->systemCursor == NULL) {
			printf("Unable to create a mouse cursor !\n");
			exit(1);
		}
		else {
			SDL_SetCursor(this->systemCursor);
		}
	}

}

void Engine::init() {
#ifdef _DEBUG 
	printf("Engine initialization.\n");
#endif
	this->initSDL();
	this->initOGL();
	this->initDevIL();
	this->initBASS();
	this->setSystemCursor();
}


void Engine::initSDL(void) {
#ifdef _DEBUG 
	printf("SDL main initialization.\n");
#endif
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		printf("SDL_Init error! %s\n", SDL_GetError());
		exit(1);
	}

	atexit(SDL_Quit);
#ifdef _DEBUG 
	printf("SDL Window initialization.\n");
#endif
	this->window = SDL_CreateWindow("For Gold and Sweetrolls", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, this->settings.screenWidth, this->settings.screenHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	if (this->window == NULL) {
		printf("SDL_CreateWindow error! %s\n", SDL_GetError());
		exit(1);
	}
}


void Engine::initOGL(void) {

#ifdef _DEBUG 
	printf("Creating GL context.\n");
#endif
	// OGL context
	this->glContext = SDL_GL_CreateContext(window);

	// OGL main attributes
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	// Setting OpenGL version to 2.1
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

	// GLEW part
#ifdef _DEBUG 
	printf("GLEW initialization.\n");
#endif
	//glewExperimental = GL_TRUE; - ??
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		printf("ERROR !!! %s\n", glewGetErrorString(err));
		exit(1);
	}
		
	printf("GLEW STATUS: %s\n", glewGetString(GLEW_VERSION));

	if (SDL_GL_SetSwapInterval(1) < 0) { // 1 - VSYNC ON, 0 - VSYNC OFF, -1 - adaptive VSYNC
		printf("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Spacify clear color
	glClearColor(0, 0, 0, 1);

	// Viewport to display
	glViewport(0, 0, this->settings.screenWidth, this->settings.screenHeight);

	// Shader model
	glShadeModel(GL_SMOOTH); // GL_SMOOTH or GL_FLAT

	// 2D rendering
	glMatrixMode(GL_PROJECTION);

	// Save it!
	glLoadIdentity();

	// Disable depth checking
	glDisable(GL_DEPTH_TEST);

}

void Engine::initDevIL(void) {
#ifdef _DEBUG 
	printf("DevIL initialization.\n");
#endif

	ilInit(); /* Initialization of DevIL */
	
}


void Engine::initBASS(void) {
#ifdef _DEBUG 
	printf("Initializing BASS audio module... \n");
#endif

	if (BASS_Init(-1, 44100, 0, 0, NULL) < 0) {
		printf("SDL_mixer BASS_Init() error code: %i.\n", BASS_ErrorGetCode());
	}

	BASS_Start();

}


Music* Engine::getCurrentMusic(void) {
	return this->currentMusic;
}


void Engine::loadMusic(std::string musicFile) {
	currentMusic = new Music(musicFile, 1.0f, true);
}


bool Engine::playMusic(float volume) {
	this->settings.musicVolume = volume;
	return this->currentMusic->playMusic(volume);
}


bool Engine::playMusic(void) {
	return this->currentMusic->playMusic(this->settings.musicVolume);
}


bool Engine::stopMusic(void) {
	return this->currentMusic->stopMusic();
}


bool Engine::pauseMusic(void) {
	return this->currentMusic->pauseMusic();
}


void Engine::releaseMusic(void) {
	delete this->currentMusic;
}

