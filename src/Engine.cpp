/*
 * For Gold and Sweetrolls
 * Copyright (C) 2020 Piotr Wróblewski <szczypiorofix@o2.pl>
 */

#define _CRT_SECURE_NO_WARNINGS

#include "Engine.h"
#include <sstream>
#include <fstream>
#include <iostream>
#include "Player.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



CE::Engine::Engine() {

	this->window = nullptr;
	this->glContext = nullptr;
	this->currentMusic = nullptr;
	this->cursorIcon = nullptr;
	this->systemCursor = nullptr;

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


void CE::Engine::launch(void) {
	this->init();
}


void CE::Engine::stop(s16 _exitCode) {

	debugInfoNl(DEBUG_INFO, "Engine stop.");

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
	
	exit(_exitCode);

}


void CE::Engine::stop() {
	this->stop(this->exitCode);
}


void CE::Engine::setSystemCursor() {
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

void CE::Engine::init() {

	debugInfoNl(DEBUG_INFO, "Engine initialization.");

	this->initSDL();
	this->initOGL();
	this->initDevIL();
	this->initBASS();

	GraphicAssets::getAssets()->addToAssets("../res/images/mouse_cursor.png", GraphicAssets::IMAGE_ASSETS_MOUSE_CURSOR);
	//this->setSystemCursor();
	this->deviLSettings();

}


void CE::Engine::initSDL(void) {

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


void CE::Engine::initOGL(void) {

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

void CE::Engine::initDevIL(void) {

	debugInfoNl(DEBUG_INFO, "DevIL initialization.");

	ilInit(); /* Initialization of DevIL */

}

void CE::Engine::deviLSettings(void) {
	ilEnable(IL_ORIGIN_SET);
	ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
}


void CE::Engine::initBASS(void) {

	debugInfoNl(DEBUG_INFO, "Initializing BASS audio module.");

	if (BASS_Init(-1, 44100, 0, 0, nullptr) < 0) {
		printf("SDL_mixer BASS_Init() error code: %i.\n", BASS_ErrorGetCode());
		exit(1);
	}

	BASS_Start();

}


CE::Music* CE::Engine::getCurrentMusic(void) {
	return this->currentMusic;
}


void CE::Engine::loadMusic(std::string musicFile) {
	currentMusic = new CE::Music(musicFile, 1.0f, true);
}


bool CE::Engine::playMusic(float volume) {
	this->settings.musicVolume = volume;
	return this->currentMusic->playMusic(volume);
}


bool CE::Engine::playMusic(void) {
	return this->currentMusic->playMusic(this->settings.musicVolume);
}


bool CE::Engine::stopMusic(void) {
	return this->currentMusic->stopMusic();
}


bool CE::Engine::pauseMusic(void) {
	return this->currentMusic->pauseMusic();
}


void CE::Engine::releaseMusic(void) {
	delete this->currentMusic;
	this->currentMusic = nullptr;
}


CE::ShaderLoader::ShaderLoader(void) : programID(0), vertexShaderID(0), fragmentShaderID(0), colAttrib(0), posAttrib(0) {
}


CE::ShaderLoader::~ShaderLoader(void) {
	debugInfoNl(DEBUG_INFO, "Delete shaders.");
	glDeleteProgram(this->programID);
	this->programID = 0;
}


void CE::ShaderLoader::compileShaders(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath) {

	// Create and compile the vertex shader
	this->vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	this->compileShaderFile(vertexShaderFilePath, this->vertexShaderID);

	// Create and compile the fragment shader
	this->fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	this->compileShaderFile(fragmentShaderFilePath, this->fragmentShaderID);

	// Link the vertex and fragment shader into a shader program
	this->programID = glCreateProgram();
	glAttachShader(this->programID, this->vertexShaderID);
	glAttachShader(this->programID, this->fragmentShaderID);
	//glBindFragDataLocation(this->programID, 0, "outColor");
	glLinkProgram(this->programID);

	GLint isLinked = 0;
	glGetProgramiv(this->programID, GL_LINK_STATUS, (int*)&isLinked);
	if (isLinked == GL_FALSE) {
		GLint maxLength = 0;
		glGetProgramiv(this->programID, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(this->programID, maxLength, &maxLength, &infoLog[0]);

		glDeleteProgram(this->programID);
		glDeleteShader(this->vertexShaderID);
		glDeleteShader(this->fragmentShaderID);

		printf("%s\n", &(infoLog[0]));
		printf("Shaders failed to link.\n");
	}

	glDetachShader(this->programID, this->vertexShaderID);
	glDetachShader(this->programID, this->fragmentShaderID);

	glDeleteShader(this->vertexShaderID);
	glDeleteShader(this->fragmentShaderID);

}


void CE::ShaderLoader::compileShaderFile(const std::string& filePath, GLuint& shaderId) {

	std::string shaderCode;
	std::ifstream shaderFile;
	// ensure ifstream objects can throw exceptions:
	shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		shaderFile.open(DIR_RES_SHADERS + filePath);
		std::stringstream shaderStream;
		shaderStream << shaderFile.rdbuf();
		shaderFile.close();
		shaderCode = shaderStream.str();
	}
	catch (std::ifstream::failure e) {
		debugInfoNl(DEBUG_ERROR, "Shader file not successfully read");
	}

	const char* contentsPtr = shaderCode.c_str();
	glShaderSource(shaderId, 1, &contentsPtr, NULL);
	glCompileShader(shaderId);

	GLint success = 0;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE) {
		GLint maxLength = 0;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<char> errorLog(maxLength);
		glGetShaderInfoLog(shaderId, maxLength, &maxLength, &errorLog[0]);
		glDeleteShader(shaderId);
		debugInfo(DEBUG_ERROR, "Shader " + filePath + " failed to compile: ");
		printf("%s\n", &(errorLog[0]));
		return;
	}
	debugInfoNl(DEBUG_INFO, "Shader " + filePath + " compiled successfuly.");
}


void CE::ShaderLoader::setBool(const std::string& name, bool value) const {
	glUniform1i(glGetUniformLocation(this->programID, name.c_str()), (int)value);
}


void CE::ShaderLoader::setInt(const std::string& name, int value) const {
	glUniform1i(glGetUniformLocation(this->programID, name.c_str()), value);
}


void CE::ShaderLoader::setFloat(const std::string& name, float value) const {
	glUniform1f(glGetUniformLocation(this->programID, name.c_str()), value);
}


void CE::ShaderLoader::use(void) {
	glUseProgram(this->programID);
}


void CE::ShaderLoader::unuse(void) {
	glUseProgram(0);
}


// Graphics Assets

CE::GraphicAssets::GraphicAssets() {

	debugInfoNl(DEBUG_INFO, "Creating new graphic assets.");

	this->textures[IMAGE_ASSETS_MAIN_MENU_BACKGROUND] = nullptr;
	this->textures[IMAGE_ASSETS_LOGO] = nullptr;
	this->textures[IMAGE_ASSETS_MAIN_MENU_BUTTONS] = nullptr;
	this->textures[IMAGE_ASSETS_VINGUE_FONT] = nullptr;
	this->textures[IMAGE_ASSETS_BIG_SPRITESHEET] = nullptr;
	this->textures[IMAGE_ASSETS_MAIN_MENU_BUTTONS] = nullptr;
}


void CE::GraphicAssets::releaseAssets() {
	debugInfoNl(DEBUG_INFO, "Releasing textures.");
	for (int i = 0; i < MAX_SPRITESHEETS; i++) {
		if (CE::GraphicAssets::getAssets()->textures[i] != NULL) {
			delete CE::GraphicAssets::getAssets()->textures[i];
			CE::GraphicAssets::getAssets()->textures[i] = nullptr;
		}
	}
}


CE::GraphicAssets* CE::GraphicAssets::getAssets() {
	if (!instance) {
		instance = new CE::GraphicAssets();
	}
	return instance;
}


void CE::GraphicAssets::addToAssets(std::string fileName, cu8 imagesEnum) {
	Texture* tempTexture = new Texture(fileName, 0, 0, 800, 600);
	CE::GraphicAssets::getAssets()->textures[imagesEnum] = tempTexture;
}


// Set instance to null pointer
CE::GraphicAssets* CE::GraphicAssets::instance = nullptr;


CE::Texture::Texture(std::string _fileName, GLfloat _x, GLfloat _y, u16 _screenWidth, u16 _screenHeight) {
	this->x = _x;
	this->y = _y;
	this->screenWidth = _screenWidth;
	this->screenHeight = _screenHeight;
	this->imageId = 0;
	this->data = nullptr;
	this->bytesPerPixel = 0;
	this->bitsPerPixel = 0;
	this->depth = 0;
	this->format = 0;
	this->vao = 0;
	this->vbo = 0;
	this->ebo = 0;
	this->textureId = loadTexture(_fileName);
	this->tileWidth = 0.0f;
	this->tileHeight = 0.0f;
	this->columns = 0;

	debugInfoNl(DEBUG_INFO, "Texture " + _fileName + " loaded.");
	printf("Texture width:  %.3f , height:  %.3f\n", this->width, height);

	if (this->textureId != 0) {

		float vertices[] = {
			// positions (x, y)																				// colors (r,g,b)		// texture coords

			// upper left
			this->coordToFloatX(this->x), this->coordToFloatY(-this->y),									1.0f, 1.0f, 1.0f,		0, 1,

			// upper right
			this->coordToFloatX(this->x + this->width), this->coordToFloatY(-this->y),						1.0f, 1.0f, 1.0f,		1, 1,

			// lower right
			this->coordToFloatX(this->x + this->width), this->coordToFloatY(-this->y - this->height),		1.0f, 1.0f, 1.0f,		1, 0,

			// lower left
			this->coordToFloatX(this->x), this->coordToFloatY(-this->y - this->height),						1.0f, 1.0f, 1.0f,		0, 0

		};

		//for (int i = 0; i < 32; i++)
		//	printf("KOORD: %f\n", vertices[i]);

		unsigned int indices[] = {
			0, 1, 3, // first triangle
			1, 2, 3  // second triangle
		};

		glGenVertexArrays(1, &this->vao);
		glGenBuffers(1, &this->vbo);
		glGenBuffers(1, &this->ebo);

		glBindVertexArray(this->vao);

		glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);

		// position attribute
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// color attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(2 * sizeof(float)));
		glEnableVertexAttribArray(1);
		// texture coord attribute
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(5 * sizeof(float)));
		glEnableVertexAttribArray(2);
	}
}



GLuint CE::Texture::loadTexture(std::string _fileName) {

	ilGenImages(1, &this->imageId); /* Generation of one image name */
	ilBindImage(this->imageId); /* Binding of image name */

	ILboolean success = ilLoadImage((wchar_t*)_fileName.c_str()); /* Loading of image "image.jpg" */
	if (success) {
		success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE); /* Convert every colour component into
		  unsigned byte. If your image contains alpha channel you can replace IL_RGB with IL_RGBA */
		if (!success) {
			printf("Unable to convert image into bytes.\n");
			exit(1);
		}
		glGenTextures(1, &this->textureId); /* Texture name generation */
		glBindTexture(GL_TEXTURE_2D, this->textureId); /* Binding of texture name */
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		float borderColor[] = { 1.0f, 0.0f, 0.0f, 1.0f }; // Red border color
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

		glTexEnvf(GL_TEXTURE_2D, GL_TEXTURE_ENV_MODE, GL_DECAL);

		this->width = (GLfloat)ilGetInteger(IL_IMAGE_WIDTH);
		this->height = (GLfloat)ilGetInteger(IL_IMAGE_HEIGHT);
		this->data = ilGetData();
		this->format = ilGetInteger(IL_IMAGE_FORMAT);
		this->bytesPerPixel = ilGetInteger(IL_IMAGE_BYTES_PER_PIXEL);
		this->bitsPerPixel = ilGetInteger(IL_IMAGE_BITS_PER_PIXEL);

		debugInfoNl(DEBUG_INFO, "Loading texture " + std::to_string(this->textureId) + ".");

		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			ilGetInteger(IL_IMAGE_BPP),
			ilGetInteger(IL_IMAGE_WIDTH),
			ilGetInteger(IL_IMAGE_HEIGHT),
			0,
			ilGetInteger(IL_IMAGE_FORMAT),
			GL_UNSIGNED_BYTE,
			ilGetData()
		); /* Texture specification */

		glGenerateMipmap(GL_TEXTURE_2D); // Generating mipmap
	}
	else {
		printf("Unable to load image %s !!!\n", _fileName.c_str());
		exit(1);
	}

	ilDeleteImages(1, &imageId); /* Because we have already copied image data into texture data we can release memory used by image. */

	return this->textureId;
}


CE::Texture::~Texture() {

	if (this->textureId != 0)
		debugInfoNl(DEBUG_INFO, "Releasing texture " + std::to_string(this->textureId) + " from memory.");

	if (glIsTexture(this->textureId)) {
		glDeleteTextures(1, &this->textureId);
	}

	//debugInfoNl(DEBUG_INFO, "Releasing vertex array VAO: " + std::to_string(this->vao));
	glDeleteVertexArrays(1, &this->vao);
	//debugInfoNl(DEBUG_INFO, "Releasing buffers: VBO: " + std::to_string(this->vbo) + " EBO: " + std::to_string(this->ebo));
	glDeleteBuffers(1, &this->vbo);
	glDeleteBuffers(1, &this->ebo);

	this->imageId = 0;
	this->data = nullptr;
	this->bytesPerPixel = 0;
	this->bitsPerPixel = 0;
	this->depth = 0;
	this->format = 0;
	this->textureId = 0;
	this->tileWidth = 0.0f;
	this->tileHeight = 0.0f;
	this->columns = 0;
}


GLfloat CE::Texture::coordToFloatX(GLfloat _x) {
	return (GLfloat)(_x * 2.0 / this->screenWidth - 0.5f);
}


GLfloat CE::Texture::coordToFloatY(GLfloat _y) {
	return (GLfloat)(_y * 2.0 / this->screenHeight + 0.5f);
}


GLfloat CE::Texture::coordToFloat(GLfloat _i, u16 length) {
	return (GLfloat)(_i * 2.0 / length + 1.0f);
}


//void CE::Texture::drawTile(int _id, GLfloat _dx, GLfloat _dy) {
//
//	if (this->tileWidth > 0 && this->tileHeight > 0) {
//
//		int sx = (int)((_id % this->columns) * this->tileWidth);
//		int sy = (int)((_id / this->columns) * this->tileHeight);
//
//		TextureRect s = { std::floor((_id % this->columns) * this->tileWidth), std::floor((_id / this->columns) * this->tileHeight), this->tileWidth, this->tileHeight };
//		TextureRect d = { _dx, _dy, this->tileWidth, this->tileHeight };
//
//		this->draw(s, d);
//
//	}
//}


//void CE::Texture::drawTile(int _id, GLfloat _dx, GLfloat _dy, GLfloat _scale) {
//
//	if (this->tileWidth > 0 && this->tileHeight > 0) {
//
//		int sx = (int)((_id % this->columns) * this->tileWidth);
//		int sy = (int)((_id / this->columns) * this->tileHeight);
//
//		TextureRect s = { std::floor((_id % this->columns) * this->tileWidth), std::floor((_id / this->columns) * this->tileHeight), this->tileWidth, this->tileHeight };
//		TextureRect d = { _dx, _dy, this->tileWidth * _scale, this->tileHeight * _scale };
//
//		this->draw(s, d);
//
//	}
//}


void CE::Texture::draw(GLfloat _x, GLfloat _y) {
	if (this->textureId != 0 && this->vao != 0) {
		GLint id = 0;
		glGetIntegerv(GL_CURRENT_PROGRAM, &id);

		int vertexColorLocation = glGetUniformLocation(id, "newPos");
		//printf("newColor location: %i\n", vertexColorLocation);
		//glUniform2f(vertexColorLocation, this->coordToFloatX(x + (this->screenWidth / 2)), this->coordToFloatY(-y - (this->screenHeight / 2)));
		glUniform2f(vertexColorLocation, this->coordToFloatX( _x ), this->coordToFloatY( -_y ));

		glBindTexture(GL_TEXTURE_2D, this->textureId);
		glBindVertexArray(this->vao);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo); // binding VAO automatically binds EBO
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
	else {
		printf("TextureId = 0 or VAO = 0 !!!\n");
	}
}


void CE::Texture::draw(GLfloat _x, GLfloat _y, GLfloat _width, GLfloat _height) {
	GLint id = 0;
	glGetIntegerv(GL_CURRENT_PROGRAM, &id);
	int vertexColorLocation = glGetUniformLocation(id, "position");
	glUniform2f(vertexColorLocation, this->coordToFloatX( _x ), this->coordToFloatY( -_y ));
	
	glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::scale(trans, glm::vec3( _width / this->width , _height / this->height, 1.0));

	unsigned int transformLoc = glGetUniformLocation(id, "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

	glBindTexture(GL_TEXTURE_2D, this->textureId);
	glBindVertexArray(this->vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}


//void CE::Texture::draw(TextureRect _src, TextureRect _dest) {
//	if (this->textureId != 0) {
//
//		float tx = (_src.x / this->width);
//		float ty = 1.0f - ((_src.y + _src.h) / this->height);
//		float tw = _src.w / this->width;
//		float th = _src.h / this->height;
//
//		float vertices[] = {
//			// positions (x, y)																	// colors (r,g,b,a)			// texture coords
//			this->coordToFloatX(_dest.x),			this->coordToFloatY(-_dest.y),				0.0f, 1.0f, 1.0f, 1.0f,		tx, ty + th,
//			this->coordToFloatX(_dest.x + _dest.w),	this->coordToFloatY(-_dest.y),				0.0f, 1.0f, 1.0f, 1.0f,		tx + tw, ty + th,
//			this->coordToFloatX(_dest.x + _dest.w),	this->coordToFloatY(-_dest.y - _dest.h),	0.0f, 1.0f, 1.0f, 1.0f,		tx + tw, ty,
//			this->coordToFloatX(_dest.x),			this->coordToFloatY(-_dest.y - _dest.h),	0.0f, 1.0f, 1.0f, 1.0f,		tx,      ty
//
//		};
//		unsigned int indices[] = {
//			0, 1, 3, // first triangle
//			1, 2, 3  // second triangle
//		};
//
//		glGenVertexArrays(1, &this->vao);
//		glGenBuffers(1, &this->vbo);
//		glGenBuffers(1, &this->ebo);
//
//		glBindVertexArray(this->vao);
//
//		glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
//		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//
//		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
//		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
//
//		// position attribute
//		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
//		glEnableVertexAttribArray(0);
//		// color attribute
//		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
//		glEnableVertexAttribArray(1);
//		// texture coord attribute
//		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
//		glEnableVertexAttribArray(2);
//
//		glEnable(GL_TEXTURE_2D);
//		glBindTexture(GL_TEXTURE_2D, this->textureId);
//		glBindVertexArray(this->vao);
//		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
//		glDisable(GL_TEXTURE_2D);
//	}
//}


int CE::XMLHelper::xmlCharToInt(const xmlChar* a) {
	int c = 0;
	int sign = 0;
	int offset = 0;
	int n = 0;
	if (a[0] == '-') {
		sign = -1;
	}
	if (sign == -1) {
		offset = 1;
	}
	else {
		offset = 0;
	}
	for (c = offset; a[c] != '\0'; c++) {
		n = n * 10 + a[c] - '0';
	}
	if (sign == -1) {
		n = -n;
	}
	return n;
}


short CE::XMLHelper::xmlCharToShort(const xmlChar* a) {
	short c = 0;
	short sign = 0;
	short offset = 0;
	short n = 0;
	if (a[0] == '-') {
		sign = -1;
	}
	if (sign == -1) {
		offset = 1;
	}
	else {
		offset = 0;
	}
	for (c = offset; a[c] != '\0'; c++) {
		n = n * 10 + a[c] - '0';
	}
	if (sign == -1) {
		n = -n;
	}
	return n;
}


int CE::XMLHelper::readPropInt(xmlNodePtr node, const xmlChar* prop) {
	xmlChar* c = xmlGetProp(node, prop);
	int i = 0;
	if (c != NULL) {
		i = xmlCharToInt(c);
		xmlFree(c);
	}
	return i;
}


short CE::XMLHelper::readPropShort(xmlNodePtr node, const xmlChar* prop) {
	xmlChar* c = xmlGetProp(node, prop);
	short s = 0;
	if (c != NULL) {
		s = xmlCharToShort(c);
		xmlFree(c);
	}
	return s;
}



CE::TextFont::TextFont(std::string fn, Texture* tex) {
	this->charsCount = 0;
	this->fontWidth = 0;
	this->fontHeight = 0;
	this->fontSpace = 0;
	this->fontName = fn;
	this->fontImage = tex;
	this->fontItems = {};
	this->parseXML(fn);
}


CE::TextFont::~TextFont() {}


void CE::TextFont::parseXML(std::string xmlFileName) {

	xmlDocPtr doc;
	xmlNodePtr cur;

	std::string xmlFile = DIR_RES_FONTS + xmlFileName + ".xml";
	doc = xmlParseFile(xmlFile.c_str());
	if (!doc) {
		std::cout << "Document cannot be parsed successfully." << std::endl;
		exit(1);
	}

	cur = xmlDocGetRootElement(doc);
	if (!cur) {
		std::cout << "Empty document." << std::endl;
		exit(1);
	}

	debugInfoNl(DEBUG_INFO, "Reading " + xmlFile + " file.");

	if (xmlStrcmp(cur->name, (const xmlChar*)"font")) {
		std::cout << "Document of the wrong type. Root node must be 'font'." << std::endl;
		xmlFreeDoc(doc);
		exit(1);
	}

	this->fontWidth = XMLHelper::readPropShort(cur, (const xmlChar*)"width");
	this->fontHeight = XMLHelper::readPropShort(cur, (const xmlChar*)"height");
	this->fontSpace = XMLHelper::readPropShort(cur, (const xmlChar*)"space");

	cur = cur->xmlChildrenNode;

	xmlNodePtr mainItems = cur;

	while (mainItems != nullptr) {
		if (!xmlStrcmp(mainItems->name, (const xmlChar*)"item")) {
			this->charsCount++;
		}
		mainItems = mainItems->next;
	}

	this->fontItems.clear();
	this->fontItems.reserve(this->charsCount);

	mainItems = cur;
	while (mainItems != nullptr) {
		if (!xmlStrcmp(mainItems->name, (const xmlChar*)"item")) {
			FontItem* tempItem = new FontItem();
			tempItem->ascii = XMLHelper::readPropShort(mainItems, (const xmlChar*)"ascii");
			tempItem->ucode = XMLHelper::readPropShort(mainItems, (const xmlChar*)"ucode");
			tempItem->top = XMLHelper::readPropShort(mainItems, (const xmlChar*)"top");
			tempItem->bottom = XMLHelper::readPropShort(mainItems, (const xmlChar*)"bottom");
			tempItem->x = XMLHelper::readPropShort(mainItems, (const xmlChar*)"x");
			tempItem->y = XMLHelper::readPropShort(mainItems, (const xmlChar*)"y");
			tempItem->width = XMLHelper::readPropShort(mainItems, (const xmlChar*)"width");
			tempItem->height = XMLHelper::readPropShort(mainItems, (const xmlChar*)"height");
			tempItem->leading = XMLHelper::readPropShort(mainItems, (const xmlChar*)"leading");
			tempItem->trailing = XMLHelper::readPropShort(mainItems, (const xmlChar*)"trailing");

			this->fontItems.push_back(tempItem);
		}
		mainItems = mainItems->next;
	}

	xmlFreeDoc(doc);
	xmlCleanupMemory();
	xmlCleanupParser();

}


float CE::TextFont::getWithOfFontString(const char* text) {
	float c = 0.0f;
	for (int i = 0; text[i] != 0; i++) {
		for (int j = 0; j < this->charsCount; j++) {
			if (text[i] == this->fontItems.at(j)->ascii) {
				c += this->fontItems.at(j)->width;
			}
		}
	}
	return c;
}


void CE::TextFont::draw(const char* text, GLfloat x, GLfloat y, float size) {

	int c = 0;
	while (text[c] != 0) {
		for (int i = 0; i < this->charsCount; i++) {
			if (text[c] == this->fontItems.at(i)->ascii) {

				TextureRect dest = {
					x + fontItems.at(i)->trailing + fontItems.at(i)->leading + (int)(c * fontWidth * size),
					(GLfloat)y,  // +fontItems[i]->top;
					(GLfloat)(fontWidth * size),
					(GLfloat)(fontHeight * size)
				};

				TextureRect src = {
					(GLfloat)fontItems.at(i)->x,
					(GLfloat)fontItems.at(i)->y,
					(GLfloat)fontItems.at(i)->width,
					(GLfloat)fontItems.at(i)->height
				};

				//this->fontImage->draw(src, dest);

			}
		}
		c++;
	}
}


CE::FontAssets::FontAssets() {

	debugInfoNl(DEBUG_INFO, "Creating new font assets.");

	this->fonts[0] = nullptr;
}


CE::FontAssets::~FontAssets() {}


void CE::FontAssets::releaseAssets() {

	debugInfoNl(DEBUG_INFO, "Releasing fonts.");

	for (int i = 0; i < MAX_FONTS; i++) {
		delete CE::FontAssets::getAssets()->fonts[i];
		CE::FontAssets::getAssets()->fonts[i] = nullptr;
	}

}


CE::FontAssets* CE::FontAssets::getAssets() {
	if (!instance) {
		instance = new FontAssets();
	}
	return instance;
}


void CE::FontAssets::addToAssets(std::string fileName, Texture* tex, const int fontEnum) {
	CE::TextFont* tempFont = new CE::TextFont(fileName, tex);
	CE::FontAssets::getAssets()->fonts[fontEnum] = tempFont;
}


CE::FontAssets* CE::FontAssets::instance = nullptr;


CE::TiledMap::TiledMap(std::string fileName) {

	this->map = new Map();

	xmlDocPtr doc;
	xmlNodePtr cur;
	std::string fn = DIR_RES_IMAGES + fileName;

	debugInfoNl(DEBUG_INFO, "Parsing tmx file: " + fn + ".");

	doc = xmlParseFile(fn.c_str());
	if (doc == NULL) {
		std::cout << "Unable to parse the document: " << fn << std::endl;
		exit(1);
	}

	cur = xmlDocGetRootElement(doc);
	if (cur == NULL) {
		std::cout << "Empty document" << std::endl;
		exit(1);
	}

	if (xmlStrcmp(cur->name, (const xmlChar*)"map")) {
		std::cout << "Document of wrong type, root node should be 'map' !" << std::endl;
		xmlFreeDoc(doc);
		exit(1);
	}

	this->map->width = XMLHelper::readPropInt(cur, (const xmlChar*)"width");
	this->map->height = XMLHelper::readPropInt(cur, (const xmlChar*)"height");
	this->map->tileWidth = XMLHelper::readPropInt(cur, (const xmlChar*)"tilewidth");
	this->map->tileHeight = XMLHelper::readPropInt(cur, (const xmlChar*)"tileheight");

	cur = cur->xmlChildrenNode;
	xmlNodePtr firstForCounter = cur;

	u16 tileSetCounter = 0;
	u16 layerCounter = 0;
	u16 objectGroupCounter = 0;

	while (firstForCounter != NULL) {
		if ((!xmlStrcmp(firstForCounter->name, (const xmlChar*)"tileset")))		tileSetCounter++;
		if ((!xmlStrcmp(firstForCounter->name, (const xmlChar*)"layer")))		layerCounter++;
		if ((!xmlStrcmp(firstForCounter->name, (const xmlChar*)"objectgroup")))	objectGroupCounter++;
		firstForCounter = firstForCounter->next;
	}

	this->map->tileSets.clear();
	this->map->tileSets.reserve(tileSetCounter);

	this->map->layers.clear();
	this->map->layers.reserve(layerCounter);

	this->map->objectGroups.clear();
	this->map->objectGroups.reserve(objectGroupCounter);

	while (cur != NULL) {
		if ((!xmlStrcmp(cur->name, (const xmlChar*)"tileset"))) {
			this->map->tileSets.push_back(new TileSet());
			this->map->tileSets.back()->firstGid = XMLHelper::readPropInt(cur, (const xmlChar*)"firstgid");
			TileSetSource* tss = this->getTileSetSource((char*)xmlGetProp(cur, (const xmlChar*)"source"));
			this->map->tileSets.back()->source = tss;
		}
		if ((!xmlStrcmp(cur->name, (const xmlChar*)"layer"))) {
			this->map->layers.push_back(new Layer());
			this->map->layers.back()->id = XMLHelper::readPropInt(cur, (const xmlChar*)"id");
			this->map->layers.back()->name = (char*)xmlGetProp(cur, (const xmlChar*)"name");
			this->map->layers.back()->width = XMLHelper::readPropInt(cur, (const xmlChar*)"width");
			this->map->layers.back()->height = XMLHelper::readPropInt(cur, (const xmlChar*)"height");
			this->map->layers.back()->data = this->parseData(doc, cur);
		}
		if ((!xmlStrcmp(cur->name, (const xmlChar*)"objectgroup"))) {
			this->map->objectGroups.push_back(new ObjectGroup());
			this->map->objectGroups.back()->id = XMLHelper::readPropInt(cur, (const xmlChar*)"id");
			this->map->objectGroups.back()->name = (char*)xmlGetProp(cur, (const xmlChar*)"name");

			// Counting objects in a group
			xmlNodePtr objNode = cur->xmlChildrenNode;
			int objCounter = 0;
			while (objNode != NULL) {
				if ((!xmlStrcmp(objNode->name, (const xmlChar*)"object"))) {
					objCounter++;
				}
				objNode = objNode->next;
			}

			this->map->objectGroups.back()->objectsCount = objCounter;

			// Getting objects from object groups
			//this->map->objectGroups.back()->objects = this->getObjects(cur, this->map.objectGroups[ogc]->objectsCount);
		}
		cur = cur->next;
	}

	xmlFreeDoc(doc);
	xmlCleanupMemory();
	xmlCleanupParser();

}


std::vector<CE::TiledObject*> CE::TiledMap::getObjects(xmlNodePtr cur, int objectCount) {
	std::vector<CE::TiledObject*> objects = {};

	cur = cur->xmlChildrenNode;
	while (cur != NULL) {
		if ((!xmlStrcmp(cur->name, (const xmlChar*)"object"))) {

			objects.push_back(new CE::TiledObject());

			objects.back()->id = XMLHelper::readPropInt(cur, (const xmlChar*)"id");
			objects.back()->name = (char*)xmlGetProp(cur, (const xmlChar*)"name");
			objects.back()->type = (char*)xmlGetProp(cur, (const xmlChar*)"type");
			objects.back()->templateFile = (char*)xmlGetProp(cur, (const xmlChar*)"template");
			objects.back()->x = XMLHelper::readPropInt(cur, (const xmlChar*)"x");
			objects.back()->y = XMLHelper::readPropInt(cur, (const xmlChar*)"y");

			// Parse template file
			xmlDocPtr txDoc;
			xmlNodePtr txCurNode;

			std::string fn = DIR_RES_IMAGES + charArrayToString(objects.back()->templateFile);

			debugInfoNl(DEBUG_INFO, "Parsing tx file: " + fn + ".");

			txDoc = xmlParseFile(fn.c_str());
			if (txDoc == NULL) {
				std::cout << "Unable to parse document " << fn << std::endl;
				exit(1);
			}
			txCurNode = xmlDocGetRootElement(txDoc);
			if (txCurNode == NULL) {
				std::cout << "Empty document" << std::endl;
				xmlFreeDoc(txDoc);
				exit(0);
			}
			if (xmlStrcmp(txCurNode->name, (const xmlChar*)"template")) {
				std::cout << "Document of wrong type, root node should be 'template' !" << std::endl;
				xmlFreeDoc(txDoc);
				exit(1);
			}
			txCurNode = txCurNode->xmlChildrenNode;

			while (txCurNode != NULL) {
				if (!(xmlStrcmp(txCurNode->name, (const xmlChar*)"tileset"))) {
					objects.back()->source = (char*)xmlGetProp(txCurNode, (const xmlChar*)"source");
					objects.back()->firstGid = XMLHelper::readPropInt(txCurNode, (const xmlChar*)"firstgid");
				}
				if (!(xmlStrcmp(txCurNode->name, (const xmlChar*)"object"))) {
					objects.back()->gid = XMLHelper::readPropInt(txCurNode, (const xmlChar*)"gid");
					objects.back()->width = XMLHelper::readPropInt(txCurNode, (const xmlChar*)"width");
					objects.back()->height = XMLHelper::readPropInt(txCurNode, (const xmlChar*)"height");
				}
				txCurNode = txCurNode->next;
			}

			xmlFreeDoc(txDoc);
		}
		cur = cur->next;
	}

	return objects;
}


CE_ArrayInt CE::TiledMap::parseData(xmlDocPtr doc, xmlNodePtr cur) {
	CE_ArrayInt arr;
	arr.size = this->map->width * this->map->height;

	xmlChar* key;
	cur = cur->xmlChildrenNode;
	while (cur != NULL) {
		if ((!xmlStrcmp(cur->name, (const xmlChar*)"data"))) {
			key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			std::string str = charArrayToString((char*)key);
			arr = *stringToArrayOfInt(str);

			//std::cout << "Array size: " << a->size << std::endl;
			//for (unsigned int i = 0; i < a->size; i++) {
			//	std::cout << a->arr[i] << ",";
			//}
			//std::cout << std::endl;

			xmlFree(key);
		}
		cur = cur->next;
	}

	return arr;
}


CE::TileSetSource* CE::TiledMap::getTileSetSource(std::string tsxFileName) {
	CE::TileSetSource* tss = new CE::TileSetSource();

	xmlDocPtr tsxDoc;
	xmlNodePtr tsxCurNode;

	std::string fn = DIR_RES_IMAGES + tsxFileName;

	debugInfoNl(DEBUG_INFO, "Parsing tsx file: " + fn + ".");

	tsxDoc = xmlParseFile(fn.c_str());
	if (tsxDoc == NULL) {
		std::cout << "Unable to parse document " << fn << std::endl;
		exit(1);
	}

	tsxCurNode = xmlDocGetRootElement(tsxDoc);
	if (tsxCurNode == NULL) {
		std::cout << "Empty document: " << fn << std::endl;
		xmlFreeDoc(tsxDoc);
		exit(1);
	}

	if (xmlStrcmp(tsxCurNode->name, (const xmlChar*)"tileset")) {
		std::cout << "Document of the wrong type, root node != tileset !!! " << fn << std::endl;
		xmlFreeDoc(tsxDoc);
		exit(1);
	}

	tss->name = (char*)xmlGetProp(tsxCurNode, (const xmlChar*)"name");
	tss->tileWidth = XMLHelper::readPropInt(tsxCurNode, (const xmlChar*)"tilewidth");
	tss->tileHeight = XMLHelper::readPropInt(tsxCurNode, (const xmlChar*)"tileheight");
	tss->tileCount = XMLHelper::readPropInt(tsxCurNode, (const xmlChar*)"tilecount");
	tss->columns = XMLHelper::readPropInt(tsxCurNode, (const xmlChar*)"columns");

	tsxCurNode = tsxCurNode->xmlChildrenNode;
	while (tsxCurNode != NULL) {
		if ((!xmlStrcmp(tsxCurNode->name, (const xmlChar*)"image"))) {
			tss->imageSource = (char*)xmlGetProp(tsxCurNode, (const xmlChar*)"source");
			tss->width = XMLHelper::readPropInt(tsxCurNode, (const xmlChar*)"width");
			tss->height = XMLHelper::readPropInt(tsxCurNode, (const xmlChar*)"height");
		}
		tsxCurNode = tsxCurNode->next;
	}

	xmlFreeDoc(tsxDoc);
	return tss;
}


CE::TiledMap::~TiledMap() {}


CE::LuaHandler::LuaHandler(const std::string& fileName) {
	std::string fn = DIR_RES_SCRIPTS + fileName;
	this->luaState = luaL_newstate();
	luaL_openlibs(this->luaState);
	if (!loadFile(fn)) {
		std::cout << "Cannot read file: " << fn.c_str() << std::endl;
	}
}


CE::LuaHandler::~LuaHandler() {
	close();
}


void CE::LuaHandler::close() {
	std::cout << "Shutting down lua state." << std::endl;
	if (!this->luaState)
		return;
	lua_close(this->luaState);
	this->luaState = nullptr;
}


bool CE::LuaHandler::loadFile(const std::string fileName) {
	if (this->luaState && (fileName.length() > 0)) {
		luaL_loadfile(this->luaState, fileName.c_str());
		if (lua_pcall(this->luaState, 0, 0, 0) == LUA_OK)
			return true;
	}
	std::cout << lua_tostring(this->luaState, -1) << std::endl;
	return false;
}


bool CE::LuaHandler::getInt(const char* name, int& value) {
	bool res = false;
	if (!this->getGlobal(name))
		return res;
	if (lua_isnumber(this->luaState, -1) == false) {
		lua_pop(this->luaState, 1);
		return res;
	}
	value = (int)(lua_tointeger(this->luaState, -1));
	lua_pop(this->luaState, 1);
	res = true;
	return res;
}


bool CE::LuaHandler::getString(const char* name, std::string& value) {
	if (!this->getGlobal(name))
		return false;
	if (lua_type(this->luaState, -1) != LUA_TSTRING) {
		lua_pop(this->luaState, 1);
		return false;
	}
	value = lua_tostring(this->luaState, -1);
	lua_pop(this->luaState, 1);
	return true;
}



CE::GameObject* CE::LuaHandler::getObject(const char* _objectName) {
	if (!this->getGlobal(_objectName))
		return nullptr;

	lua_getglobal(this->luaState, _objectName);
	lua_pushstring(this->luaState, "name");
	lua_gettable(this->luaState, -2);
	const char* playerName = lua_tostring(this->luaState, -1);
	lua_Number playerX = CE::LuaHandler::getFieldInt(_objectName, "x");
	lua_Number playerY = CE::LuaHandler::getFieldInt(_objectName, "y");
	lua_Number playerWidth = CE::LuaHandler::getFieldInt(_objectName, "width");
	lua_Number playerHeight = CE::LuaHandler::getFieldInt(_objectName, "height");

	return new Player(playerName, (float)playerX, (float)playerY, (float)playerWidth, (float)playerHeight);
}




lua_Number CE::LuaHandler::getFieldInt(const char* objectName, const char* propertyName) {
	lua_getglobal(this->luaState, objectName);
	lua_getfield(this->luaState, -1, propertyName);
	lua_Number r = lua_tonumber(this->luaState, -1);
	lua_pop(this->luaState, -1);
	return r;
}


bool CE::LuaHandler::getGlobal(const char* name) {
	if ((!this->luaState) || (!name))
		return false;
	lua_getglobal(this->luaState, name);
	return true;
}



// ############################ BASS ############################

//BASS_Init(-1, 44100, 0, 0, NULL);
//std::cout << "ERROR CODE: " << BASS_ErrorGetCode() << std::endl;

// ############### FOR MOD & XM #####################
//HMUSIC music = BASS_MusicLoad(FALSE, "../res/music/ex-aws_cave.xm", 0, 0, 0, 0);
//std::cout << "ERROR CODE: " << BASS_ErrorGetCode() << std::endl;

//BASS_ChannelPlay(music, TRUE);

// ########### SAMPLE ############
//HSAMPLE sample = BASS_SampleLoad(FALSE, "../res/music/menu-music.ogg", 0, 0, 1, 0);
//HCHANNEL channel = BASS_SampleGetChannel(sample, TRUE);
//std::cout << "Sample: " << sample << std::endl;
//std::cout << "ERROR CODE: " << BASS_ErrorGetCode() << std::endl;
//BASS_Start();
//std::cout << "ERROR CODE: " << BASS_ErrorGetCode() << std::endl;
//
//BASS_ChannelPlay(channel, TRUE);
//std::cout << "ERROR CODE: " << BASS_ErrorGetCode() << std::endl;




CE::Music::Music(const std::string& musicFile, float volume, bool loop) {

	this->volume = volume;
	this->loop = loop;
	this->musicFileName = DIR_RES_MUSIC + musicFile;

	int flag = 0;
	if (loop) {
		flag = BASS_SAMPLE_LOOP;
	}

	this->stream = BASS_StreamCreateFile(FALSE, this->musicFileName.c_str(), 0, 0, flag);
	if (this->stream == 0) {
		printf("Failed to load stream! BASS_StreamCreateFile Error: %i\n", BASS_ErrorGetCode());
		exit(1);
	}

}


CE::Music::~Music() {

	debugInfoNl(DEBUG_INFO, "Releasing music.");

	this->musicFileName = "";
	BASS_StreamFree(this->stream);

}


bool CE::Music::playMusic() {
	if (!BASS_ChannelSetAttribute(this->stream, BASS_ATTRIB_VOL, this->volume)) {
		printf("Failed to change channel volume! BASS_ChannelSetAttribute Error: %i\n", BASS_ErrorGetCode());
	}

	debugInfoNl(DEBUG_INFO, "Playing " + this->musicFileName + ".");

	bool r = BASS_ChannelPlay(this->stream, TRUE);
	if (!r) {
		printf("Failed to play channel! BASS_ChannelPlay Error: %i\n", BASS_ErrorGetCode());
	}
	return r;
}


bool CE::Music::playMusic(float _volume) {
	this->volume = _volume;
	if (!BASS_ChannelSetAttribute(this->stream, BASS_ATTRIB_VOL, this->volume)) {
		printf("Failed to change channel volume! BASS_ChannelSetAttribute Error: %i\n", BASS_ErrorGetCode());
	}

	debugInfoNl(DEBUG_INFO, "Playing " + this->musicFileName + ".");

	bool r = BASS_ChannelPlay(this->stream, TRUE);
	if (!r) {
		printf("Failed to play channel! BASS_ChannelPlay Error: %i\n", BASS_ErrorGetCode());
	}
	return r;
}


bool CE::Music::stopMusic() {
	return BASS_ChannelStop(this->stream);
}


bool CE::Music::pauseMusic() {
	return BASS_ChannelPause(this->stream);
}


HSTREAM CE::Music::getStream() {
	return this->stream;
}


CE::Animation::Animation(u16 speed, cu16 size, std::vector<u16> tilesArray) {
	this->size = size;
	this->speed = speed;
	this->counter = 0;
	this->curFrame = 0;
	this->tilesArray = tilesArray;
}


void CE::Animation::nextFrame(void) {
	this->counter++;
	if (this->counter > this->speed) {
		this->counter = 0;
		this->curFrame++;
		if (this->curFrame >= this->size)
			this->curFrame = 0;
	}
}


u16 CE::Animation::getTile(void) {
	u16 res = 0;
	try {
		res = this->tilesArray.at(this->curFrame);
	}
	catch (std::out_of_range& ex) {
		//std::cout << "OUT OF RANGE EXCEPTION !!!" << ex.what() << std::endl;
		debugInfoNl(DEBUG_WARNING, "Out of range exception !!!" + std::string(ex.what()));
	}
	return res;
}


u16 CE::Animation::getCurFrame(void) {
	return this->curFrame;
}


void CE::Animation::setCurrentFrame(u16 cf) {
	this->curFrame = cf;
}


CE::Animation::~Animation() {}


CE::GameObject::GameObject() {
	this->vector = new Vector2();
	this->width = 0.0f;
	this->height = 0.0f;
	this->name = "default_name";
}


CE::GameObject::GameObject(Vector2* _vector, float _width, float _height) {
	this->vector = _vector;
	this->width = _width;
	this->height = _height;
	this->name = "default_name";
}


CE::GameObject::GameObject(const char* _name, float _x, float _y, float _width, float _height) {
	this->vector = new Vector2(_x, _y);
	this->width = _width;
	this->height = _height;
	this->name = std::string(_name);
}


CE::GameObject::~GameObject() {
	delete this->vector;
	this->vector = nullptr;
	this->width = 0.0f;
	this->height = 0.0f;
	this->name = nullptr;
}


void CE::GameObject::update() {

}


void CE::GameObject::render() {

}


int CE::GameObject::getTileX(u16 tileWidth) {
	return (int)((this->vector->x + (this->width / 2)) / tileWidth);
}


int CE::GameObject::getTileY(u16 tileHeight) {
	return (int)((this->vector->y + (this->height / 2)) / tileHeight);
}
