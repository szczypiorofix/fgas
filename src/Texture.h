/*
 * For Gold and Sweetrolls
 * Copyright (C) 2020 Piotr Wróblewski <szczypiorofix@o2.pl>
 */

#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <iostream>
#include <SDL2/SDL_opengl.h>
#include <IL/il.h>



typedef struct TextureRect {
	GLfloat x, y;
	GLfloat w, h;
} TextureRect;



class Texture {

public:
	Texture(std::string _fileName);
	Texture(std::string _fileName, GLfloat _tileWidth, GLfloat _tileHeight);
	~Texture();

	ILuint imageId;
	GLuint textureId;

	GLfloat width;
	GLfloat height;
	GLfloat tileWidth;
	GLfloat tileHeight;

	ILubyte* data;
	ILuint format;
	ILuint bytesPerPixel;
	ILuint bitsPerPixel;
	ILuint depth;

	int columns;

	void draw(TextureRect _src, TextureRect _dest);
	void drawTile(int _id, GLfloat _dx, GLfloat _dy);
	void drawTile(int _id, GLfloat _dx, GLfloat _dy, GLfloat _scale);

private:
	GLuint loadTexture(std::string _fileName);
	
	GLfloat coordToFloatX(GLfloat _x);
	GLfloat coordToFloatY(GLfloat _y);

	GLuint vao;
	GLuint vbo;
	GLuint ebo;
};


#pragma once
#endif
