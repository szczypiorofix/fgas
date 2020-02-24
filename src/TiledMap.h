/*
 * For Gold and Sweetrolls
 * Copyright (C) 2020 Piotr Wróblewski <szczypiorofix@o2.pl>
 */

#ifndef _TILEDMAP_H_
#define _TILEDMAP_H_
#pragma once

#include <iostream>
#include <vector>
#include <libxml/xmlIO.h>
#include <libxml/xinclude.h>
#include <libxml/tree.h>

#include "Defines.h"


typedef struct TileSetSource {
	char* name;
	u16 tileWidth;
	u16 tileHeight;
	u16 tileCount;
	u16 columns;
	u16 width;
	u16 height;
	char* imageSource;
} TileSetSource;

typedef struct TiledObject {
	u16 id;
	char* name;
	char* type;
	char* templateFile;
	s16 x;
	s16 y;
	u16 firstGid;
	char* source;
	u16 gid;
	u16 width;
	u16 height;
} TiledObject;

typedef struct ObjectGroup {
	u16 id;
	char* name;
	u16 objectsCount;
	std::vector<TiledObject*> objects;
} ObjectGroup;

typedef struct TiledTemplate {
	u16 tileSetFirstGid;
	std::string source;
	u16 objectGid;
	u16 width;
	u16 height;
} TiledTemplate;

typedef struct TileSet {
	u16 firstGid;
	TileSetSource* source;
} TileSet;

typedef struct Layer {
	u16 id;
	char* name;
	u16 width;
	u16 height;
	DG_ArrayInt data;
	u16 dataSize;
} Layer;

typedef struct Map {
	u16 width;
	u16 height;
	u16 tileWidth;
	u16 tileHeight;
	u16 nextLayerId;
	u16 nextObjectId;
	std::vector<TileSet*> tileSets;
	std::vector<Layer*> layers;
	std::vector<ObjectGroup*> objectGroups;
	
	Map() : width(0), height(0), tileWidth(0), tileHeight(0), nextLayerId(0), nextObjectId(0), tileSets(), layers(), objectGroups() {}

} Map;

class TiledMap {

public:
	TiledMap(std::string fileName);
	~TiledMap();

	Map* map;

private:

	TileSetSource* getTileSetSource(std::string tsxFileName);
	DG_ArrayInt parseData(xmlDocPtr doc, xmlNodePtr cur);
	std::vector<TiledObject*> getObjects(xmlNodePtr cur, int objectCount);
};


#endif
