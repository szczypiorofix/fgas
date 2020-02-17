/*
 * Dungeon Engine
 * Copyright (C) 2020 szczypiorofix <szczypiorofix@o2.pl>
 */

#include <SDL2/SDL.h>
#include <GL/glew.h>

#include "Texture.h"
#include "Defines.h"


Texture::Texture(std::string fileName, GLfloat tileWidth, GLfloat tileHeight) {
	this->image = loadTexture(fileName);
	this->tileWidth = tileWidth;
	this->tileHeight = tileHeight;
	this->columns = (int)(this->width / this->tileWidth);

#ifdef _DEBUG 
	printf("Texture %s, columns %i.\n", fileName.c_str(), this->columns);
#endif
	
}


Texture::Texture(std::string fileName) {
	this->image = loadTexture(fileName);
	this->tileWidth = 0.0f;
	this->tileHeight = 0.0f;
	this->columns = 0;

#ifdef _DEBUG 
	printf("Texture %s.\n", fileName.c_str());
#endif

}


GLuint Texture::loadTexture(std::string fileName) {

	ilGenImages(1, &texid); /* Generation of one image name */
	ilBindImage(texid); /* Binding of image name */

	ILboolean success = ilLoadImage((wchar_t*)fileName.c_str()); /* Loading of image "image.jpg" */
	if (success) {
		success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE); /* Convert every colour component into
		  unsigned byte. If your image contains alpha channel you can replace IL_RGB with IL_RGBA */
		if (!success) {
			printf("Unable to convert image into bytes.\n");
			exit(1);
		}
		glGenTextures(1, &image); /* Texture name generation */
		glBindTexture(GL_TEXTURE_2D, image); /* Binding of texture name */
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexEnvf(GL_TEXTURE_2D, GL_TEXTURE_ENV_MODE, GL_DECAL);

		this->width = (GLfloat)ilGetInteger(IL_IMAGE_WIDTH);
		this->height = (GLfloat)ilGetInteger(IL_IMAGE_HEIGHT);

		glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP), ilGetInteger(IL_IMAGE_WIDTH),
			ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE,
			ilGetData()); /* Texture specification */
	}
	else {
		printf("Unable to load image %s !!!\n", fileName.c_str());
		exit(1);
	}

	ilDeleteImages(1, &texid); /* Because we have already copied image data into texture data we can release memory used by image. */

	return this->image;
}


Texture::~Texture() {}


void Texture::drawTile(int _id, GLfloat dx, GLfloat dy) {

	if (this->tileWidth > 0 && this->tileHeight > 0) {
		glColor4ub(0xFF, 0xFF, 0xFF, 0xFF);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, this->image);

		int sx = (int)( (_id % this->columns) * this->tileWidth );
		int sy = (int)( (_id / this->columns) * this->tileHeight );

		//GL_POINTS, GL_LINES, GL_LINE_STRIP, GL_LINE_LOOP, GL_QUADS, GL_TRIANGLES, GL_POLIGON
		glBegin(GL_QUADS);

			glTexCoord2f( sx / this->width, sy / this->height );
			glVertex2f( dx, dy );

			glTexCoord2f( (sx + this->tileWidth) / this->width, sy / this->height );
			glVertex2f( dx + this->tileWidth, dy );

			glTexCoord2f( (sx + this->tileWidth) / this->width, (sy + this->tileHeight) / this->height );
			glVertex2f( dx + this->tileWidth, dy + this->tileHeight );

			glTexCoord2f( sx / this->width, (sy + this->tileHeight) / this->height );
			glVertex2f( dx, dy + this->tileHeight );

		glEnd();
		glDisable(GL_TEXTURE_2D);
	}

}


void Texture::draw(TextureRect src, TextureRect dest) {
	glColor4ub(0xFF, 0xFF, 0xFF, 0xFF);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, this->image);

	//GL_POINTS, GL_LINES, GL_LINE_STRIP, GL_LINE_LOOP, GL_QUADS, GL_TRIANGLES, GL_POLIGON
	glBegin(GL_QUADS);

		glTexCoord2f( src.x / this->width, src.y / this->height );
		glVertex2f( dest.x, dest.y );

		glTexCoord2f( (src.x + src.w) / this->width , src.y / this->height );
		glVertex2f( dest.x + dest.w, dest.y );

		glTexCoord2f( (src.x + src.w) / this->width, (src.y + src.h) / this->height );
		glVertex2f( dest.x + dest.w, dest.y + dest.h );

		glTexCoord2f( src.x / this->width, (src.y + src.h) / this->height );
		glVertex2f( dest.x, dest.y + dest.h );

	glEnd();
	glDisable(GL_TEXTURE_2D);
}
