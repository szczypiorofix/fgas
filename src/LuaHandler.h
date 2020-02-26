/*
 * For Gold and Sweetrolls
 * Copyright (C) 2020 Piotr Wróblewski <szczypiorofix@o2.pl>
 */

#ifndef _LUAHANDLER_H_
#define _LUAHANDLER_H_
#pragma once

#include <iostream>
#include <lua535/lua.hpp>
#include "Player.h"

class LuaHandler {

public:

	LuaHandler(const std::string& fileName);
	~LuaHandler();

	void close();
	
	bool loadFile(const std::string fileName );

	bool getInt(const char* name, int& value);
	bool getString(const char* name, std::string& value);
	
	Player* getPlayer();

	bool getGlobal(const char* name);


private:
	lua_State* luaState;
	
	lua_Number getFieldInt(const char* objectName, const char* propertyName);

};

#endif
