/*
 * For Gold and Sweetrolls
 * Copyright (C) 2020 Piotr Wróblewski <szczypiorofix@o2.pl>
 */

#include "TiledMap.h"
#include <iostream>


TiledMap::TiledMap(std::string fileName) {

	this->map = new Map();

	xmlDocPtr doc;
	xmlNodePtr cur;
	std::string fn = DIR_RES_IMAGES + fileName;

	debugInfoNl(DEBUG_INFO, "Parsing tmx file: " + fn + ".");

	doc = xmlParseFile(fn.c_str());
	if (doc == NULL) {
		std::cout << "Unable to parse the document: " << fn << std::endl;
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

	this->map->width = XMLHelper::readPropInt(cur, (const xmlChar*)"width");
	this->map->height = XMLHelper::readPropInt(cur, (const xmlChar*)"height");
	this->map->tileWidth = XMLHelper::readPropInt(cur, (const xmlChar*)"tilewidth");
	this->map->tileHeight = XMLHelper::readPropInt(cur, (const xmlChar*)"tileheight");

	cur = cur->xmlChildrenNode;
	xmlNodePtr firstForCounter = cur;

	u16 tileSetCounter = 0;
	u16 layerCounter = 0;
	u16 objectGroupCounter = 0;

	while (firstForCounter != NULL) {
		if ((!xmlStrcmp(firstForCounter->name, (const xmlChar*)"tileset")))		tileSetCounter++;
		if ((!xmlStrcmp(firstForCounter->name, (const xmlChar*)"layer")))		layerCounter++;
		if ((!xmlStrcmp(firstForCounter->name, (const xmlChar*)"objectgroup")))	objectGroupCounter++;
		firstForCounter = firstForCounter->next;
	}

	this->map->tileSets.clear();
	this->map->tileSets.reserve(tileSetCounter);

	this->map->layers.clear();
	this->map->layers.reserve(layerCounter);

	this->map->objectGroups.clear();
	this->map->objectGroups.reserve(objectGroupCounter);

	while (cur != NULL) {
		if ((!xmlStrcmp(cur->name, (const xmlChar*)"tileset"))) {
			this->map->tileSets.push_back(new TileSet());
			this->map->tileSets.back()->firstGid = XMLHelper::readPropInt(cur, (const xmlChar*)"firstgid");
			TileSetSource* tss = this->getTileSetSource((char*)xmlGetProp(cur, (const xmlChar*)"source"));
			this->map->tileSets.back()->source = tss;
		}
		if ((!xmlStrcmp(cur->name, (const xmlChar*)"layer"))) {
			this->map->layers.push_back(new Layer());
			this->map->layers.back()->id = XMLHelper::readPropInt(cur, (const xmlChar*)"id");
			this->map->layers.back()->name = (char*)xmlGetProp(cur, (const xmlChar*)"name");
			this->map->layers.back()->width = XMLHelper::readPropInt(cur, (const xmlChar*)"width");
			this->map->layers.back()->height = XMLHelper::readPropInt(cur, (const xmlChar*)"height");
			this->map->layers.back()->data = this->parseData(doc, cur);
		}
		if ((!xmlStrcmp(cur->name, (const xmlChar*)"objectgroup"))) {
			this->map->objectGroups.push_back(new ObjectGroup());
			this->map->objectGroups.back()->id = XMLHelper::readPropInt(cur, (const xmlChar*)"id");
			this->map->objectGroups.back()->name = (char*)xmlGetProp(cur, (const xmlChar*)"name");

			// Counting objects in a group
			xmlNodePtr objNode = cur->xmlChildrenNode;
			int objCounter = 0;
			while (objNode != NULL) {
				if ((!xmlStrcmp(objNode->name, (const xmlChar*)"object"))) {
					objCounter++;
				}
				objNode = objNode->next;
			}

			this->map->objectGroups.back()->objectsCount = objCounter;

			// Getting objects from object groups
			//this->map->objectGroups.back()->objects = this->getObjects(cur, this->map.objectGroups[ogc]->objectsCount);
		}
		cur = cur->next;
	}

	xmlFreeDoc(doc);
	xmlCleanupMemory();
	xmlCleanupParser();

}


std::vector<TiledObject*> TiledMap::getObjects(xmlNodePtr cur, int objectCount) {
	std::vector<TiledObject*> objects = {};

	cur = cur->xmlChildrenNode;
	while (cur != NULL) {
		if ((!xmlStrcmp(cur->name, (const xmlChar*)"object"))) {

			objects.push_back(new TiledObject());

			objects.back()->id = XMLHelper::readPropInt(cur, (const xmlChar*)"id");
			objects.back()->name = (char*)xmlGetProp(cur, (const xmlChar*)"name");
			objects.back()->type = (char*)xmlGetProp(cur, (const xmlChar*)"type");
			objects.back()->templateFile = (char*)xmlGetProp(cur, (const xmlChar*)"template");
			objects.back()->x = XMLHelper::readPropInt(cur, (const xmlChar*)"x");
			objects.back()->y = XMLHelper::readPropInt(cur, (const xmlChar*)"y");

			// Parse template file
			xmlDocPtr txDoc;
			xmlNodePtr txCurNode;

			std::string fn = DIR_RES_IMAGES + charArrayToString(objects.back()->templateFile);

			debugInfoNl(DEBUG_INFO, "Parsing tx file: " + fn + ".");

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
					objects.back()->source = (char*)xmlGetProp(txCurNode, (const xmlChar*)"source");
					objects.back()->firstGid = XMLHelper::readPropInt(txCurNode, (const xmlChar*)"firstgid");
				}
				if (!(xmlStrcmp(txCurNode->name, (const xmlChar*)"object"))) {
					objects.back()->gid = XMLHelper::readPropInt(txCurNode, (const xmlChar*)"gid");
					objects.back()->width = XMLHelper::readPropInt(txCurNode, (const xmlChar*)"width");
					objects.back()->height = XMLHelper::readPropInt(txCurNode, (const xmlChar*)"height");
				}
				txCurNode = txCurNode->next;
			}

			xmlFreeDoc(txDoc);
		}
		cur = cur->next;
	}

	return objects;
}


CE_ArrayInt TiledMap::parseData(xmlDocPtr doc, xmlNodePtr cur) {
	CE_ArrayInt arr;
	arr.size = this->map->width * this->map->height;

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

	debugInfoNl(DEBUG_INFO, "Parsing tsx file: " + fn + ".");

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

	tss->name = (char*)xmlGetProp(tsxCurNode, (const xmlChar*)"name");
	tss->tileWidth = XMLHelper::readPropInt(tsxCurNode, (const xmlChar*)"tilewidth");
	tss->tileHeight = XMLHelper::readPropInt(tsxCurNode, (const xmlChar*)"tileheight");
	tss->tileCount = XMLHelper::readPropInt(tsxCurNode, (const xmlChar*)"tilecount");
	tss->columns = XMLHelper::readPropInt(tsxCurNode, (const xmlChar*)"columns");

	tsxCurNode = tsxCurNode->xmlChildrenNode;
	while (tsxCurNode != NULL) {
		if ((!xmlStrcmp(tsxCurNode->name, (const xmlChar*)"image"))) {
			tss->imageSource = (char*)xmlGetProp(tsxCurNode, (const xmlChar*)"source");
			tss->width = XMLHelper::readPropInt(tsxCurNode, (const xmlChar*)"width");
			tss->height = XMLHelper::readPropInt(tsxCurNode, (const xmlChar*)"height");
		}
		tsxCurNode = tsxCurNode->next;
	}

	xmlFreeDoc(tsxDoc);
	return tss;
}


TiledMap::~TiledMap(void) {}
