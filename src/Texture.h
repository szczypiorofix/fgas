/*
 * For Gold and Sweetrolls
 * Copyright (C) 2020 Piotr Wróblewski <szczypiorofix@o2.pl>
 */

#ifndef _TEXTURE_H_
#define _TEXTURE_H_
#pragma once


#include "Defines.h"
#include <GL/glew.h>
#include <IL/ilu.h>
#include <glm/glm.hpp>

class Texture {
public:
	Texture(std::string _fileName, GLfloat _x, GLfloat _y, u16 _screenWidth, u16 _screenHeight);
	~Texture(void);
	ILuint imageId;
	GLuint textureId;
	GLfloat x, y;
	GLfloat width, height;
	GLfloat tileWidth, tileHeight;
	u16 screenWidth, screenHeight;
	ILubyte* data;
	ILuint format;
	ILuint bytesPerPixel;
	ILuint bitsPerPixel;
	ILuint depth;
	int columns;
	void draw(GLfloat _x, GLfloat _y);
	void draw(GLfloat _x, GLfloat _y, GLfloat _width, GLfloat _height);
	void draw(glm::mat4 positionMatrix);
	void draw(glm::mat4 positionMatrix, glm::mat2 textureMatrix);
	/*void draw(TextureRect _src, TextureRect _dest);
	void drawTile(int _id, GLfloat _dx, GLfloat _dy);
	void drawTile(int _id, GLfloat _dx, GLfloat _dy, GLfloat _scale);*/
private:
	GLuint loadTexture(std::string _fileName);
	GLfloat coordToFloatX(GLfloat _x);
	GLfloat coordToFloatY(GLfloat _y);
	GLfloat coordToFloat(GLfloat _i, u16 length);
	GLuint vao;
	GLuint vbo;
	GLuint ebo;
};

#endif
