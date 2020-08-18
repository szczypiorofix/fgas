#ifndef _SPRITESHEET_H_
#define _SPRITESHEET_H_
#pragma once

#include "Texture.h"
#include <GL/glew.h>
#include <vector>



typedef struct Tile {
	GLfloat x, y;
	GLfloat width, height;
	GLuint index;
	GLuint ix, iy;
} Tile;



class SpriteSheet {
public:
	SpriteSheet(Texture* _texture, GLuint _tilesOnWidth, GLuint _tilesOnHeight);
	~SpriteSheet();

	int tileWidth, tileHeight;
	int tilesOnWidth, tilesOnHeight;

	std::vector<Tile*> tiles;

	void draw(Tile& _sprite);
	void draw(GLuint _index, GLfloat _x, GLfloat _y);

private:
	GLuint textureId;
	Texture* texture;
};

#endif
