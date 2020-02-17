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

	ILuint texid;
	GLuint image;

	GLfloat width;
	GLfloat height;
	GLfloat tileWidth;
	GLfloat tileHeight;
	int columns;

	void draw(TextureRect src, TextureRect dest);
	void drawTile(int _id, GLfloat dx, GLfloat dy);

private:
	GLuint loadTexture(std::string fileName);

};


#pragma once
#endif
