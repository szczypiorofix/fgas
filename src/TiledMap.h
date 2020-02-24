/*
 * For Gold and Sweetrolls
 * Copyright (C) 2020 Piotr Wróblewski <szczypiorofix@o2.pl>
 */

#ifndef _TILEDMAP_H_
#define _TILEDMAP_H_
#pragma once

#include <iostream>
#include <libxml/xmlIO.h>
#include <libxml/xinclude.h>
#include <libxml/tree.h>

#include "Defines.h"


typedef struct TileSetSource {
	char* name;
	int tileWidth;
	int tileHeight;
	int tileCount;
	int columns;
	int width;
	int height;
	char* imageSource;
} TileSetSource;

typedef struct TiledObject {
	int id;
	char* name;
	char* type;
	char* templateFile;
	int x;
	int y;
	int firstGid;
	char* source;
	int gid;
	int width;
	int height;
} TiledObject;

typedef struct ObjectGroup {
	int id;
	char* name;
	int objectsCount;
	TiledObject** objects;
} ObjectGroup;

typedef struct TiledTemplate {
	int tileSetFirstGid;
	std::string source;
	int objectGid;
	int width;
	int height;
} TiledTemplate;

typedef struct TileSet {
	int firstGid;
	TileSetSource* source;
} TileSet;

typedef struct Layer {
	int id;
	char* name;
	int width;
	int height;
	DG_ArrayInt data;
	int dataSize;
} Layer;

typedef struct Map {
	int width;
	int height;
	int tileWidth;
	int tileHeight;
	int nextLayerId;
	int nextObjectId;
	TileSet** tileSets;
	int tileSetCounter;
	Layer** layers;
	int layerCounter;
	ObjectGroup** objectGroups;
	int objectGroupCounter;
} Map;

class TiledMap {

public:
	TiledMap(std::string fileName);
	~TiledMap();

	Map map;

private:

	TileSetSource* getTileSetSource(std::string tsxFileName);
	DG_ArrayInt parseData(xmlDocPtr doc, xmlNodePtr cur);
	TiledObject** getObjects(xmlNodePtr cur, int objectCount);
};


#endif
