#include "SpriteSheet.h"



SpriteSheet::SpriteSheet(Texture* _texture, GLuint _tilesOnWidth, GLuint _tilesOnHeight) :
	texture(_texture),
	textureId(_texture->textureId),
	tileWidth(0),
	tileHeight(0),
	tilesOnWidth(_tilesOnWidth),
	tilesOnHeight(_tilesOnHeight) {
	

	this->tileWidth = (int)_texture->width / this->tilesOnWidth;
	this->tileHeight = (int)_texture->height / this->tilesOnHeight;
	printf("Creating sprite sheet from texture id %i. Tiles on width %i, tiles on height %i\n", _texture->textureId, this->tilesOnWidth, this->tilesOnHeight);


	printf("Tile width %i, tile height %i\n", this->tileWidth, this->tileHeight);



}


SpriteSheet::~SpriteSheet() {  }


void SpriteSheet::draw(GLuint _index, GLfloat _x, GLfloat _y) {
	
}


void SpriteSheet::draw(Tile& _sprite) {

}
