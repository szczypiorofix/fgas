/*
 * For Gold and Sweetrolls
 * Copyright (C) 2020 Piotr Wróblewski <szczypiorofix@o2.pl>
 */


#include <SDL2/SDL.h>
#include <GL/glew.h>

#include "Texture.h"
#include "Defines.h"


Texture::Texture(std::string fileName, GLfloat tileWidth, GLfloat tileHeight) {
	this->imageId = 0;
	this->data = NULL;
	this->bytesPerPixel = 0;
	this->bitsPerPixel = 0;
	this->depth = 0;
	this->format = 0;
	this->vao = 0;
	this->vbo = 0;
	this->ebo = 0;
	this->textureId = loadTexture(fileName);
	this->tileWidth = tileWidth;
	this->tileHeight = tileHeight;
	this->columns = (int)(this->width / this->tileWidth);

#ifdef _DEBUG 
	printf("Texture %s loaded. Columns %i.\n", fileName.c_str(), this->columns);
#endif
	
}


Texture::Texture(std::string fileName) {
	this->imageId = 0;
	this->data = NULL;
	this->bytesPerPixel = 0;
	this->bitsPerPixel = 0;
	this->depth = 0;
	this->format = 0;
	this->vao = 0;
	this->vbo = 0;
	this->ebo = 0;
	this->textureId = loadTexture(fileName);
	this->tileWidth = 0.0f;
	this->tileHeight = 0.0f;
	this->columns = 0;

#ifdef _DEBUG 
	printf("Texture %s loaded.\n", fileName.c_str());
#endif

}


GLuint Texture::loadTexture(std::string fileName) {

	ilGenImages(1, &this->imageId); /* Generation of one image name */
	ilBindImage(this->imageId); /* Binding of image name */

	ILboolean success = ilLoadImage((wchar_t*)fileName.c_str()); /* Loading of image "image.jpg" */
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

		printf("Loading texture %i to video memory.\n", this->textureId);

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
		printf("Unable to load image %s !!!\n", fileName.c_str());
		exit(1);
	}

	ilDeleteImages(1, &imageId); /* Because we have already copied image data into texture data we can release memory used by image. */

	return this->textureId;
}

GLfloat Texture::coordToFloatX(GLint x) {
	return ((x * 2.0) / SCREEN_WIDTH) - 1.0f;
}

GLfloat Texture::coordToFloatY(GLint y) {
	return ((y * 2.0) / SCREEN_HEIGHT) - 1.0f;
}


Texture::~Texture() {
	printf("Releasing texture from memory: %i\n", this->textureId);
	if (glIsTexture(this->textureId)) {
		glDeleteTextures(1, &this->textureId);
	}
	
	this->imageId = 0;
	this->data = NULL;
	this->bytesPerPixel = 0;
	this->bitsPerPixel = 0;
	this->depth = 0;
	this->format = 0;
	this->textureId = 0;
	this->tileWidth = 0.0f;
	this->tileHeight = 0.0f;
	this->columns = 0;
}


void Texture::drawTile(int _id, GLfloat dx, GLfloat dy) {

	if (this->tileWidth > 0 && this->tileHeight > 0) {
		glColor4ub(0xFF, 0xFF, 0xFF, 0xFF);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, this->textureId);

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
	
	if (this->textureId != 0) {
		//glColor4ub(0xFF, 0xFF, 0xFF, 0xFF);
		//glEnable(GL_TEXTURE_2D);
		//glBindTexture(GL_TEXTURE_2D, this->textureId);

		////GL_POINTS, GL_LINES, GL_LINE_STRIP, GL_LINE_LOOP, GL_QUADS, GL_TRIANGLES, GL_POLIGON
		//glBegin(GL_QUADS);
		//	glTexCoord2f( src.x / this->width, src.y / this->height );
		//	glVertex2f( dest.x, dest.y );
		//	glTexCoord2f( ( src.x + src.w) / this->width, src.y / this->height );
		//	glVertex2f( dest.x + dest.w, dest.y );
		//	glTexCoord2f( (src.x + src.w) / this->width, (src.y + src.h) / this->height );
		//	glVertex2f( dest.x + dest.w, dest.y + dest.h );
		//	glTexCoord2f( src.x / this->width, (src.y + src.h) / this->height );
		//	glVertex2f( dest.x, dest.y + dest.h );
		//glEnd();

		//glDisable(GL_TEXTURE_2D);



		//glEnable(GL_TEXTURE_2D);
		//float vertices[] = {
		//	// positions          // colors           // texture coords
		//	 0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f, // top right
		//	 0.5f, -0.5f, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f, // bottom right
		//	-0.5f, -0.5f, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f, // bottom left
		//	-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f  // top left 
		//};
		//unsigned int indices[] = {
		//	0, 1, 3, // first triangle
		//	1, 2, 3  // second triangle
		//};

		//glGenVertexArrays(1, &this->vao);
		//glGenBuffers(1, &this->vbo);
		//glGenBuffers(1, &this->ebo);

		//glBindVertexArray(this->vao);

		//glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
		//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
		//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		//// position attribute
		//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		//glEnableVertexAttribArray(0);
		//// color attribute
		//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		//glEnableVertexAttribArray(1);
		//// texture coord attribute
		//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		//glEnableVertexAttribArray(2);

		//glBindTexture(GL_TEXTURE_2D, textureId);
		//glBindVertexArray(this->vao);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//glDisable(GL_TEXTURE_2D);



		glEnable(GL_TEXTURE_2D);
		float vertices[] = {
			// positions          // colors           // texture coords
			 0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f, // top right
			 0.5f, -0.5f, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f, // bottom right
			-0.5f, -0.5f, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f, // bottom left
			-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f  // top left 
		};
		unsigned int indices[] = {
			0, 1, 3, // first triangle
			1, 2, 3  // second triangle
		};

		glGenVertexArrays(1, &this->vao);
		glGenBuffers(1, &this->vbo);
		glGenBuffers(1, &this->ebo);

		glBindVertexArray(this->vao);

		glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// color attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		// texture coord attribute
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);

		glBindTexture(GL_TEXTURE_2D, textureId);
		glBindVertexArray(this->vao);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glDisable(GL_TEXTURE_2D);


	}

}
