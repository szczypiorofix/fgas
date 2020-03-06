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
	Texture(std::string fileName);
	Texture(std::string fileName, GLfloat tileWidth, GLfloat tileHeight);
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

	void draw(TextureRect src, TextureRect dest);
	void drawTile(int _id, GLfloat dx, GLfloat dy);

private:
	GLuint loadTexture(std::string fileName);
	
	GLfloat coordToFloatX(GLint x);
	GLfloat coordToFloatY(GLint y);

	GLuint vao;
	GLuint vbo;
	GLuint ebo;
};


#pragma once
#endif
