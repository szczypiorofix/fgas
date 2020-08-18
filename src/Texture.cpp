/*
 * For Gold and Sweetrolls
 * Copyright (C) 2020 Piotr Wróblewski <szczypiorofix@o2.pl>
 */

#include "Texture.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


Texture::Texture(std::string _fileName, GLfloat _x, GLfloat _y, u16 _screenWidth, u16 _screenHeight) {
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
			this->coordToFloatX(this->x), this->coordToFloatY(-this->y),									1.0f, 1.0f, 1.0f,		0, 1,  // 0,1

			// upper right
			this->coordToFloatX(this->x + this->width), this->coordToFloatY(-this->y),						1.0f, 1.0f, 1.0f,		1, 1,  // 1,1

			// lower right
			this->coordToFloatX(this->x + this->width), this->coordToFloatY(-this->y - this->height),		1.0f, 1.0f, 1.0f,		1, 0,  // 1,0

			// lower left
			this->coordToFloatX(this->x), this->coordToFloatY(-this->y - this->height),						1.0f, 1.0f, 1.0f,		0, 0   // 0,0

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



GLuint Texture::loadTexture(std::string _fileName) {

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


Texture::~Texture(void) {

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


GLfloat Texture::coordToFloatX(GLfloat _x) {
	return (GLfloat)(_x * 2.0 / this->screenWidth - 0.5f);
}


GLfloat Texture::coordToFloatY(GLfloat _y) {
	return (GLfloat)(_y * 2.0 / this->screenHeight + 0.5f);
}


GLfloat Texture::coordToFloat(GLfloat _i, u16 length) {
	return (GLfloat)(_i * 2.0 / length + 1.0f);
}


//void Texture::drawTile(int _id, GLfloat _dx, GLfloat _dy) {
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


//void Texture::drawTile(int _id, GLfloat _dx, GLfloat _dy, GLfloat _scale) {
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


void Texture::draw(GLfloat _x, GLfloat _y) {
	if (this->textureId != 0 && this->vao != 0) {
		GLint id = 0;
		glGetIntegerv(GL_CURRENT_PROGRAM, &id);

		int vertexColorLocation = glGetUniformLocation(id, "newPos");
		//printf("newColor location: %i\n", vertexColorLocation);
		//glUniform2f(vertexColorLocation, this->coordToFloatX(x + (this->screenWidth / 2)), this->coordToFloatY(-y - (this->screenHeight / 2)));
		glUniform2f(vertexColorLocation, this->coordToFloatX(_x), this->coordToFloatY(-_y));

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


void Texture::draw(glm::mat4 positionMatrix, glm::mat2 textureMatrix) {
	GLint id = 0;
	glGetIntegerv(GL_CURRENT_PROGRAM, &id);
	unsigned int vertexPositionLocation = glGetUniformLocation(id, "transform");
	glUniformMatrix4fv(vertexPositionLocation, 1, GL_FALSE, &(positionMatrix[0][0]));

	unsigned int vertexTextureLocation = glGetUniformLocation(id, "texXY");
	//glm::mat2 m1 = glm::mat2(1.0f);
	glUniformMatrix2fv(vertexTextureLocation, 1, GL_FALSE, &(textureMatrix[0][0]));


	//unsigned int vertexTextureTilePosition = glGetUniformLocation(id, "tilePosition");
	//glm::mat2 tm = glm::mat2(1.0f);
	//tm[0][0] = 1.0f;
	//tm[1][1] = 1.0f;
	//tm[0][1] = 0.0f;
	//tm[1][0] = 0.0f;
	//glUniformMatrix2fv(vertexTextureTilePosition, 1, GL_FALSE, &(tm[0][0]));


	//unsigned int vertexTextureWidthHeight = glGetUniformLocation(id, "texWH");
	//glm::mat2 twh = glm::mat2(1.0f);
	//tm[0][0] = 1.5f;
	//tm[1][1] = 1.5f;
	//tm[0][1] = 0.0f;
	//tm[1][0] = 0.0f;
	//glUniformMatrix2fv(vertexTextureWidthHeight, 1, GL_FALSE, &(twh[0][0]));


	glBindTexture(GL_TEXTURE_2D, this->textureId);
	glBindVertexArray(this->vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}


void Texture::draw(glm::mat4 positionMatrix) {
	GLint id = 0;
	glGetIntegerv(GL_CURRENT_PROGRAM, &id);
	unsigned int vertexPositionLocation = glGetUniformLocation(id, "pos");
	glUniformMatrix4fv(vertexPositionLocation, 1, GL_FALSE, &(positionMatrix[0][0]));

	unsigned int vertexTextureLocation = glGetUniformLocation(id, "texXY");
	glUniform2f(vertexTextureLocation, 0.0f, 0.0f);
	//glm::mat2 m1 = glm::mat2(1.0f);
	//m1[0][0] = 1.0f;
	//m1[1][1] = 1.0f;
	//m1[0][1] = 0.0f;
	//m1[1][0] = 0.0f;
	//glUniformMatrix2fv(vertexTextureLocation, 1, GL_FALSE, &(m1[0][0]));

	glBindTexture(GL_TEXTURE_2D, this->textureId);
	glBindVertexArray(this->vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}


void Texture::draw(GLfloat _x, GLfloat _y, GLfloat _width, GLfloat _height) {
	GLint id = 0;
	glGetIntegerv(GL_CURRENT_PROGRAM, &id);
	/*unsigned int vertexPositionLocation = glGetUniformLocation(id, "position");
	glUniform2f(vertexPositionLocation, this->coordToFloatX( _x ), this->coordToFloatY( -_y ));*/

	/*trans = glm::scale(trans, glm::vec3( _width / this->width , _height / this->height, 1.0));*/

	/*trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));
	trans = glm::scale(trans, glm::vec3(1.0f, 1.0f, 0.0f));
	unsigned int vertexTransformLocation = glGetUniformLocation(id, "transform");
	glUniformMatrix4fv(vertexTransformLocation, 1, GL_FALSE, glm::value_ptr(trans));*/

	//glm::mat4 orthoMatrix = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f);
	
	glm::mat4 trans = glm::mat4(1.0f);

	//glm::vec3 translate(this->coordToFloatX(_x), this->coordToFloatY(-_y), 0.0f);


	trans = glm::translate(trans, glm::vec3(this->coordToFloatX(_x), this->coordToFloatY(-_y), 0.0f));
	//trans = glm::translate(orthoMatrix, translate);
	unsigned int vertexTransformLocation = glGetUniformLocation(id, "transform");
	glUniformMatrix4fv(vertexTransformLocation, 1, GL_FALSE, glm::value_ptr(trans));

	glBindTexture(GL_TEXTURE_2D, this->textureId);
	glBindVertexArray(this->vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}


//void Texture::draw(TextureRect _src, TextureRect _dest) {
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
