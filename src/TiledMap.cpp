/*
 * Dungeon Engine
 * Copyright (C) 2020 szczypiorofix <szczypiorofix@o2.pl>
 */

#include "TiledMap.h"
#include "Defines.h"
#include "XMLHelper.h"


TiledMap::TiledMap(std::string fileName) {
	
	xmlDocPtr doc;
	xmlNodePtr cur;
	std::string fn = DIR_RES_IMAGES + fileName;

#ifdef _DEBUG
	std::cout << "Parsing tmx file: " << fn << "... " << std::endl;
#endif

	doc = xmlParseFile(fn.c_str());
	if (doc == NULL) {
		std::cout << "Unable to parse document " << fn << std::endl;
		exit(1);
	}
	
	cur = xmlDocGetRootElement(doc);
	if (cur == NULL) {
		std::cout << "Empty document" << std::endl;
		exit(1);
	}

	if (xmlStrcmp(cur->name, (const xmlChar*)"map")) {
		std::cout << "Document of wrong type, root node should be 'map' !" << std::endl;
		xmlFreeDoc(doc);
		exit(1);
	}

	this->map = {0,0,0,0,0,0,NULL,0,NULL,0,NULL,0};

	this->map.width			= XMLHelper::readPropInt(cur, (const xmlChar*)"width");
	this->map.height		= XMLHelper::readPropInt(cur, (const xmlChar*)"height");
	this->map.tileWidth		= XMLHelper::readPropInt(cur, (const xmlChar*)"tilewidth");
	this->map.tileHeight	= XMLHelper::readPropInt(cur, (const xmlChar*)"tileheight");

	cur = cur->xmlChildrenNode;
	xmlNodePtr firstForCounter = cur;


	while (firstForCounter != NULL) {
		if ((!xmlStrcmp(firstForCounter->name, (const xmlChar*)"tileset")))		this->map.tileSetCounter++;
		if ((!xmlStrcmp(firstForCounter->name, (const xmlChar*)"layer")))		this->map.layerCounter++;
		if ((!xmlStrcmp(firstForCounter->name, (const xmlChar*)"objectgroup")))	this->map.objectGroupCounter++;
		firstForCounter = firstForCounter->next;
	}

	int tsc = 0;	// tilesets counter
	int lc = 0;		// layers counter
	int ogc = 0;	// object groups counter

	this->map.tileSets		= new TileSet * [this->map.tileSetCounter];
	this->map.layers		= new Layer * [this->map.layerCounter];
	this->map.objectGroups	= new ObjectGroup * [this->map.objectGroupCounter];

	while (cur != NULL) {
		if ((!xmlStrcmp(cur->name, (const xmlChar*)"tileset"))) {
			this->map.tileSets[tsc] = new TileSet();
			this->map.tileSets[tsc]->firstGid = XMLHelper::readPropInt(cur, (const xmlChar*)"firstgid");
			TileSetSource* tss = this->getTileSetSource((char*)xmlGetProp(cur, (const xmlChar*)"source"));
			this->map.tileSets[tsc]->source = tss;
			tsc++;
		}
		if ((!xmlStrcmp(cur->name, (const xmlChar*)"layer"))) {
			this->map.layers[lc] = new Layer();
			this->map.layers[lc]->id		= XMLHelper::readPropInt(cur, (const xmlChar*)"id");
			this->map.layers[lc]->name		= (char*)xmlGetProp(cur, (const xmlChar*)"name");
			this->map.layers[lc]->width		= XMLHelper::readPropInt(cur, (const xmlChar*)"width");
			this->map.layers[lc]->height	= XMLHelper::readPropInt(cur, (const xmlChar*)"height");
			this->map.layers[lc]->data		= this->parseData(doc, cur);
			lc++;
		}
		if ((!xmlStrcmp(cur->name, (const xmlChar*)"objectgroup"))) {
			this->map.objectGroups[ogc] = new ObjectGroup();
			this->map.objectGroups[ogc]->id		= XMLHelper::readPropInt(cur, (const xmlChar*)"id");
			this->map.objectGroups[ogc]->name	= (char*)xmlGetProp(cur, (const xmlChar*)"name");
	
			// Counting objects in a group
			xmlNodePtr objNode = cur->xmlChildrenNode;
			int objCounter = 0;
			while (objNode != NULL) {
				if ((!xmlStrcmp(objNode->name, (const xmlChar*)"object"))) {
					objCounter++;
				}
				objNode = objNode->next;
			}
			
			this->map.objectGroups[ogc]->objectsCount = objCounter;

			// Getting objects from object groups
			this->map.objectGroups[ogc]->objects = this->getObjects(cur, this->map.objectGroups[ogc]->objectsCount);
			ogc++;
		}
		cur = cur->next;
	}

	xmlFreeDoc(doc);
	xmlCleanupMemory();
	xmlCleanupParser();

}


TiledObject** TiledMap::getObjects(xmlNodePtr cur, int objectCount) {
	TiledObject** objects = new TiledObject * [objectCount];

	cur = cur->xmlChildrenNode;
	int c = 0;
	while (cur != NULL) {
		if ((!xmlStrcmp(cur->name, (const xmlChar*)"object"))) {
			
			objects[c] = new TiledObject();

			objects[c]->id = (int) XMLHelper::readPropInt(cur, (const xmlChar*)"id");
			objects[c]->name = (char*)xmlGetProp(cur, (const xmlChar*)"name");
			objects[c]->type = (char*)xmlGetProp(cur, (const xmlChar*)"type");
			objects[c]->templateFile = (char*)xmlGetProp(cur, (const xmlChar*)"template");
			objects[c]->x = XMLHelper::readPropInt(cur, (const xmlChar*)"x");
			objects[c]->y = XMLHelper::readPropInt(cur, (const xmlChar*)"y");

			// Parse template file
			xmlDocPtr txDoc;
			xmlNodePtr txCurNode;

			std::string fn = DIR_RES_IMAGES + charArrayToString(objects[c]->templateFile);
#ifdef _DEBUG
			std::cout << "Parsing tx file: " << fn << "... " << std::endl;
#endif

			txDoc = xmlParseFile(fn.c_str());
			if (txDoc == NULL) {
				std::cout << "Unable to parse document " << fn << std::endl;
				exit(1);
			}
			txCurNode = xmlDocGetRootElement(txDoc);
			if (txCurNode == NULL) {
				std::cout << "Empty document" << std::endl;
				xmlFreeDoc(txDoc);
				exit(0);
			}
			if (xmlStrcmp(txCurNode->name, (const xmlChar*)"template")) {
				std::cout << "Document of wrong type, root node should be 'template' !" << std::endl;
				xmlFreeDoc(txDoc);
				exit(1);
			}
			txCurNode = txCurNode->xmlChildrenNode;

			while (txCurNode != NULL) {
				if (!(xmlStrcmp(txCurNode->name, (const xmlChar*)"tileset"))) {
					objects[c]->source = (char*)xmlGetProp(txCurNode, (const xmlChar*)"source");
					objects[c]->firstGid = XMLHelper::readPropInt(txCurNode, (const xmlChar*)"firstgid");
				}
				if (!(xmlStrcmp(txCurNode->name, (const xmlChar*)"object"))) {
					objects[c]->gid		= XMLHelper::readPropInt(txCurNode, (const xmlChar*)"gid");
					objects[c]->width	= XMLHelper::readPropInt(txCurNode, (const xmlChar*)"width");
					objects[c]->height	= XMLHelper::readPropInt(txCurNode, (const xmlChar*)"height");
				}
				txCurNode = txCurNode->next;
			}

			xmlFreeDoc(txDoc);
			c++;
		}
		cur = cur->next;
	}

	return objects;
}


DG_ArrayInt TiledMap::parseData(xmlDocPtr doc, xmlNodePtr cur) {
	DG_ArrayInt arr;
	arr.size = this->map.width * this->map.height;

	xmlChar* key;
	cur = cur->xmlChildrenNode;
	while (cur != NULL) {
		if ((!xmlStrcmp(cur->name, (const xmlChar*)"data"))) {
			key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			std::string str = charArrayToString((char*)key);
			arr = *stringToArrayOfInt(str);

			//std::cout << "Array size: " << a->size << std::endl;
			//for (unsigned int i = 0; i < a->size; i++) {
			//	std::cout << a->arr[i] << ",";
			//}
			//std::cout << std::endl;
			
			xmlFree(key);
		}
		cur = cur->next;
	}

	return arr;
}


TileSetSource* TiledMap::getTileSetSource(std::string tsxFileName) {
	TileSetSource* tss = new TileSetSource();

	xmlDocPtr tsxDoc;
	xmlNodePtr tsxCurNode;

	std::string fn = DIR_RES_IMAGES + tsxFileName;

#ifdef _DEBUG
	std::cout << "Parsing tsx file: " << fn << "... " << std::endl;
#endif

	tsxDoc = xmlParseFile(fn.c_str());
	if (tsxDoc == NULL) {
		std::cout << "Unable to parse document " << fn << std::endl;
		exit(1);
	}

	tsxCurNode = xmlDocGetRootElement(tsxDoc);
	if (tsxCurNode == NULL) {
		std::cout << "Empty document: " << fn << std::endl;
		xmlFreeDoc(tsxDoc);
		exit(1);
	}

	if (xmlStrcmp(tsxCurNode->name, (const xmlChar*)"tileset")) {
		std::cout << "Document of the wrong type, root node != tileset !!! " << fn << std::endl;
		xmlFreeDoc(tsxDoc);
		exit(1);
	}

	tss->name			= (char*)xmlGetProp(tsxCurNode, (const xmlChar*)"name");
	tss->tileWidth		= XMLHelper::readPropInt(tsxCurNode, (const xmlChar*)"tilewidth");
	tss->tileHeight		= XMLHelper::readPropInt(tsxCurNode, (const xmlChar*)"tileheight");
	tss->tileCount		= XMLHelper::readPropInt(tsxCurNode, (const xmlChar*)"tilecount");
	tss->columns		= XMLHelper::readPropInt(tsxCurNode, (const xmlChar*)"columns");
	
	tsxCurNode = tsxCurNode->xmlChildrenNode;
	while (tsxCurNode != NULL) {
		if ((!xmlStrcmp(tsxCurNode->name, (const xmlChar*)"image"))) {
			tss->imageSource	= (char*)xmlGetProp(tsxCurNode, (const xmlChar*)"source");
			tss->width			= XMLHelper::readPropInt(tsxCurNode, (const xmlChar*)"width");
			tss->height			= XMLHelper::readPropInt(tsxCurNode, (const xmlChar*)"height");
		}
		tsxCurNode = tsxCurNode->next;
	}

	xmlFreeDoc(tsxDoc);
	return tss;
}

TiledMap::~TiledMap() {

}
