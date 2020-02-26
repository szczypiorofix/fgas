/*
 * For Gold and Sweetrolls
 * Copyright (C) 2020 Piotr Wróblewski <szczypiorofix@o2.pl>
 */

#ifndef _TEXTFONT_H_
#define _TEXTFONT_H_
#pragma once

#include <iostream>
#include <vector>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include "Texture.h"


typedef struct FontItem {
	short ascii;
	short ucode;
	short top;
	short bottom;
	short x;
	short y;
	short width;
	short height;
	short leading;
	short trailing;

} FontItem;


class TextFont {

public:
	TextFont(std::string fn, Texture* tex);
	~TextFont();

	void draw(const char* text, GLfloat x, GLfloat y, float size);

private:
	Texture* fontImage;
	int imageWidth;
	int imageHeight;
	FontItem** fontItems;
	int charsCount;
	
	std::string fontName;

	int fontWidth;
	int fontHeight;
	int fontSpace;

	void parseXML(std::string xmlFileName);
	float getStringFontWidth(const char* text);

};

#endif
