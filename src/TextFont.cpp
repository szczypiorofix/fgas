/*
 * For Gold and Sweetrolls
 * Copyright (C) 2020 Piotr Wróblewski <szczypiorofix@o2.pl>
 */

#include "TextFont.h"
#include "Defines.h"
#include "XMLHelper.h"


TextFont::TextFont(std::string fn, Texture* tex) {
	this->charsCount = 0;
	this->fontWidth = 0;
	this->fontHeight = 0;
	this->fontSpace = 0;
	this->fontName = fn;
	this->fontImage = tex;
	this->fontItems = {};
	this->parseXML(fn);
}


TextFont::~TextFont() {
}


void TextFont::parseXML(std::string xmlFileName) {

	xmlDocPtr doc;
	xmlNodePtr cur;

	std::string xmlFile = DIR_RES_FONTS + xmlFileName + ".xml";
	doc = xmlParseFile(xmlFile.c_str());
	if (doc == NULL) {
		std::cout << "Document cannot be parsed successfully." << std::endl;
		exit(1);
	}

	cur = xmlDocGetRootElement(doc);
	if (cur == NULL) {
		std::cout << "Empty document." << std::endl;
		exit(1);
	}
#ifdef _DEBUG 
	std::cout << "Reading " << xmlFile << " file ..." << std::endl;
#endif
	

	if (xmlStrcmp(cur->name, (const xmlChar*)"font")) {
		std::cout << "Document of the wrong type. Root node must be 'font'." << std::endl;
		xmlFreeDoc(doc);
		exit(1);
	}

	this->fontWidth		= XMLHelper::readPropShort(cur, (const xmlChar*)"width");
	this->fontHeight	= XMLHelper::readPropShort(cur, (const xmlChar*)"height");
	this->fontSpace		= XMLHelper::readPropShort(cur, (const xmlChar*)"space");

	cur = cur->xmlChildrenNode;

	xmlNodePtr mainItems = cur;

	while (mainItems != NULL) {
		if (!xmlStrcmp(mainItems->name, (const xmlChar*)"item")) {
			this->charsCount++;
		}
		mainItems = mainItems->next;
	}

	this->fontItems = new FontItem * [this->charsCount];

	/*this->fontItems.clear();
	this->fontItems.reserve(this->charsCount);*/
	
	int i = 0;
	mainItems = cur;
	while (mainItems != NULL) {
		if (!xmlStrcmp(mainItems->name, (const xmlChar*)"item")) {
			FontItem* tempItem = new FontItem();
			tempItem->ascii		= XMLHelper::readPropShort(mainItems, (const xmlChar*)"ascii");
			tempItem->ucode		= XMLHelper::readPropShort(mainItems, (const xmlChar*)"ucode");
			tempItem->top		= XMLHelper::readPropShort(mainItems, (const xmlChar*)"top");
			tempItem->bottom		= XMLHelper::readPropShort(mainItems, (const xmlChar*)"bottom");
			tempItem->x			= XMLHelper::readPropShort(mainItems, (const xmlChar*)"x");
			tempItem->y			= XMLHelper::readPropShort(mainItems, (const xmlChar*)"y");
			tempItem->width		= XMLHelper::readPropShort(mainItems, (const xmlChar*)"width");
			tempItem->height		= XMLHelper::readPropShort(mainItems, (const xmlChar*)"height");
			tempItem->leading	= XMLHelper::readPropShort(mainItems, (const xmlChar*)"leading");
			tempItem->trailing	= XMLHelper::readPropShort(mainItems, (const xmlChar*)"trailing");
			
			this->fontItems[i] = tempItem;
			i++;
		}
		mainItems = mainItems->next;
	}

	xmlFreeDoc(doc);
	xmlCleanupMemory();
	xmlCleanupParser();

}


float TextFont::getStringFontWidth(const char* text) {
	float c = 0.0f;
	for (int i = 0; text[i] != 0; i++) {
		for (int j = 0; j < this->charsCount; j++) {
			if (text[i] == this->fontItems[j]->ascii) {
				c += this->fontItems[j]->width;
			}
		}
	}
	std::cout << c << std::endl;
	return c;
}


void TextFont::draw(const char* text, GLfloat x, GLfloat y, float size) {

	TextureRect src = {
		0, 0, 0, 0
	};

	TextureRect dest = {
		0, 0, 0, 0
	};

	int c = 0;

	while (text[c] != 0) {
		for (int i = 0; i < this->charsCount; i++) {
			if (text[c] == this->fontItems[i]->ascii) {

				dest.x = x + fontItems[i]->trailing + fontItems[i]->leading + (int)(c * fontWidth * size);
				dest.y = (GLfloat)y;// +fontItems[i]->top;
				dest.w = (GLfloat)(fontWidth * size);
				dest.h = (GLfloat)(fontHeight * size);

				src = {
					(GLfloat) fontItems[i]->x,
					(GLfloat) fontItems[i]->y,
					(GLfloat) fontItems[i]->width,
					(GLfloat) fontItems[i]->height
				};

				this->fontImage->draw(src, dest);

			}
		}
		c++;
	}
	
}
