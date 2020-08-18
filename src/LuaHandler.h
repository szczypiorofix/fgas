/*
 * For Gold and Sweetrolls
 * Copyright (C) 2020 Piotr Wróblewski <szczypiorofix@o2.pl>
 */

#ifndef _LUAHANDLER_H_
#define _LUAHANDLER_H_
#pragma once

#include <lua535/lua.hpp>
#include "GameObject.h"


class LuaHandler {
public:
	LuaHandler(const std::string& fileName);
	~LuaHandler(void);
	void close(void);
	bool loadFile(const std::string fileName);
	bool getInt(const char* name, int& value);
	bool getString(const char* name, std::string& value);

	GameObject* getObject(const char* _objectName);

	bool getGlobal(const char* name);
private:
	lua_State* luaState;
	lua_Number getFieldInt(const char* objectName, const char* propertyName);
};

#endif
