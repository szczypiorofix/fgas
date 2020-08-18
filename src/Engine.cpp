/*
 * For Gold and Sweetrolls
 * Copyright (C) 2020 Piotr Wróblewski <szczypiorofix@o2.pl>
 */

#define _CRT_SECURE_NO_WARNINGS

#include "Engine.h"
#include <iostream>
#include "Player.h"
//#include "XMLHelper.h"


Engine::Engine() {
	this->window = nullptr;
	this->glContext = nullptr;
	this->currentMusic = nullptr;
	this->cursorIcon = nullptr;
	this->systemCursor = nullptr;
	this->iniParser = nullptr;

	this->settings = {
		DEFAULT_SCREEN_WIDTH,					// Screen width
		DEFAULT_SCREEN_HEIGHT,					// Screen height
		MIN_SCALE,						// current scale
		false,							// is full screen?
		true,							// is window resizable?
		0.5f							// music volume
	};

	this->exitCode = 0;
}


void Engine::launch(void) {
	this->init();
}


void Engine::stop(s16 _exitCode) {
	debugInfoNl(DEBUG_INFO, "Engine stop.");

	// Save settings to INI file.
	this->iniParser->writeFile("fgas-output.ini", this->settings.items);
	delete this->iniParser;
	this->iniParser = nullptr;

	GraphicAssets::getAssets()->releaseAssets();

	//FontAssets::getAssets()->releaseAssets();

	SDL_GL_DeleteContext(this->glContext);
	SDL_DestroyWindow(this->window);

	delete this->currentMusic;
	this->currentMusic = nullptr;
	
	if (this->systemCursor != nullptr) {
		SDL_FreeCursor(this->systemCursor);
	}
	SDL_Quit();
	//printf("EXIT\n");
	exit(_exitCode);
}


void Engine::stop(void) {
	this->stop(this->exitCode);
}


void Engine::setSystemCursor(void) {
	debugInfoNl(DEBUG_INFO, "Initializing system cursor.");
	int m = SDL_SetRelativeMouseMode(SDL_FALSE); // Trap mouse on window
	if (m == -1) {
		debugInfoNl(DEBUG_WARNING, "Warning! Error while locking mouse pointer to the window.");
	}

	SDL_WarpMouseInWindow(this->window, this->settings.screenWidth / 2, this->settings.screenHeight / 2);
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
	if (this->cursorIcon == nullptr) {
		debugInfoNl(DEBUG_ERROR, "Unable to create RPG surface!");
		exit(1);
	} else {
		this->systemCursor = SDL_CreateColorCursor(this->cursorIcon, 0, 0);
		if (this->systemCursor == nullptr) {
			debugInfoNl(DEBUG_WARNING, "Unable to create mouse cursor.");
			exit(1);
		} else {
			SDL_SetCursor(this->systemCursor);
		}
		SDL_FreeSurface(this->cursorIcon);
		this->cursorIcon = nullptr;
	}
}

void Engine::init(void) {

	debugInfoNl(DEBUG_INFO, "Engine initialization.");

	this->iniFileSetup();
	this->initSDL();
	this->initOGL();
	this->initDevIL();
	this->initBASS();

	GraphicAssets::getAssets()->addToAssets("../res/images/mouse_cursor.png", GraphicAssets::IMAGE_ASSETS_MOUSE_CURSOR);
	
	this->deviLSettings();
	//this->setSystemCursor();

}


void Engine::iniFileSetup(void) {
	this->iniParser = new IniParser();
	if (iniParser->readFile("fgas.ini")) {
		this->settings.fullScreen = this->iniParser->getInteger("FULLSCREEN");
		this->settings.screenWidth = this->iniParser->getInteger("SCREEN_WIDTH");
		this->settings.screenHeight = this->iniParser->getInteger("SCREEN_HEIGHT");
		this->settings.musicVolume = this->iniParser->getDecimal("MUSIC_VOLUME");
		this->settings.scale = this->iniParser->getDecimal("MIN_SCALE");
		this->settings.windowResizable = this->iniParser->getInteger("RESIZABLE_WINDOW");
		this->settings.items = this->iniParser->getItems();
	}
	
	//std::vector<KeyValue*> items;
	//IniType t = { 0 };
	//t.string = new std::string("nazwa_druga");
	//KeyValue* kv1 = new KeyValue(std::string("name1"), t, ValueType::STRING);
	//items.push_back(kv1);
	//iniParser->writeFile("fgas-output.ini", items);

	//delete iniParser;
	//iniParser = nullptr;
}


void Engine::initSDL(void) {

	debugInfoNl(DEBUG_INFO, "SDL main initialization.");

	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		printf("SDL_Init error! %s\n", SDL_GetError());
		exit(1);
	}

	atexit(SDL_Quit);

	debugInfoNl(DEBUG_INFO, "SDL Window initialization.");
	
	Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;
	if (this->settings.windowResizable) {
		flags |= SDL_WINDOW_RESIZABLE;
	}
	if (this->settings.fullScreen) {
		flags = SDL_WINDOW_FULLSCREEN | SDL_WINDOW_OPENGL;
	}

	this->window = SDL_CreateWindow("For Gold and Sweetrolls", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, this->settings.screenWidth, this->settings.screenHeight, flags);
	if (this->window == nullptr) {
		debugInfoNl(DEBUG_ERROR, "SDL_CreateWindow error! " + std::string(SDL_GetError()));
		this->stop(1);
	}
}


void Engine::initOGL(void) {

	debugInfoNl(DEBUG_INFO, "Creating GL context.");

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
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	// Setting OpenGL version to 3.3
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	int verMin = 0, verMaj = 0, accel = 0;
	SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &verMaj);
	SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &verMin);
	SDL_GL_GetAttribute(SDL_GL_ACCELERATED_VISUAL, &accel);
	
	debugInfoNl(DEBUG_INFO, "OpenGL version " + std::to_string(verMaj) + ":" + std::to_string(verMin) + ".");

	debugInfoNl(DEBUG_INFO, accel == 1 ? "Accelerated (hardware) renderer." : "Forced software renderer.");

	//printf("%s.\n", accel == 1 ? "Accelerated (hardware) renderer" : "Forced software renderer");

	// GLEW part
	debugInfoNl(DEBUG_INFO, "GLEW initialization.");

	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		printf("ERROR !!! %s\n", glewGetErrorString(err));
		exit(1);
	}
	
	debugInfoNl(DEBUG_INFO, "GLEW status " + std::string((char*)glewGetString(GLEW_VERSION)) + ".");

	if (SDL_GL_SetSwapInterval(1) < 0) { // 1 - VSYNC ON, 0 - VSYNC OFF, -1 - adaptive VSYNC
		printf("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
	}


	/**
	int profile = 0;
	SDL_GL_GetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, &profile);
	printf("OpenGL profile: ");
	if (profile == SDL_GL_CONTEXT_PROFILE_ES)
		printf("OpenGL ES profile - only a subset of the base OpenGL functionality is available\n");  // OpenGL ES 2.0
	if (profile == SDL_GL_CONTEXT_PROFILE_COMPATIBILITY)
		printf("OpenGL compatibility profile - deprecated functions are allowed\n");  // Compatibility mode
	if (profile == SDL_GL_CONTEXT_PROFILE_CORE)
		printf("OpenGL core profile - deprecated functions are disabled\n");  // OpenGL 2.1
	*/


	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	
	// Clear only part of the screen
	/*glEnable(GL_SCISSOR_TEST);*/


	// Spacify clear color
	glClearColor(0.3f, 0.4f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// Viewport to display
	glViewport(0, 0, this->settings.screenWidth, this->settings.screenHeight);

	glOrtho(0, this->settings.screenWidth, this->settings.screenHeight, 0.0f, -1.0f, 1.0f); // Set the matrix

	// Shader model
	glShadeModel(GL_SMOOTH); // GL_SMOOTH or GL_FLAT

	// 2D rendering
	glMatrixMode(GL_PROJECTION); // GL_MODELVIEW or GL_PROJECTION

	// Save it!
	glLoadIdentity();

	glEnable(GL_TEXTURE_2D);

	// Disable depth checking
	glDisable(GL_DEPTH_TEST);

}

void Engine::initDevIL(void) {

	debugInfoNl(DEBUG_INFO, "DevIL initialization.");

	ilInit(); /* Initialization of DevIL */

}

void Engine::deviLSettings(void) {
	ilEnable(IL_ORIGIN_SET);
	ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
}


void Engine::initBASS(void) {

	debugInfoNl(DEBUG_INFO, "Initializing BASS audio module.");

	if (BASS_Init(-1, 44100, 0, 0, nullptr) < 0) {
		printf("SDL_mixer BASS_Init() error code: %i.\n", BASS_ErrorGetCode());
		exit(1);
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
	this->currentMusic = nullptr;
}
