/*
 * For Gold and Sweetrolls
 * Copyright (C) 2020 Piotr Wr�blewski <szczypiorofix@o2.pl>
 */

#include "LuaHandler.h"
#include "Defines.h"
//#include <cassert>


LuaHandler::LuaHandler(const std::string& fileName) {
    std::string fn = DIR_RES_SCRIPTS + fileName;
    this->luaState = luaL_newstate();
    luaL_openlibs(this->luaState);
    if (!loadFile(fn)) {
        std::cout << "Cannot read file: " << fn.c_str() << std::endl;
    }
}


LuaHandler::~LuaHandler() {
    close();
}


void LuaHandler::close() {
    std::cout << "Shutting down lua state." << std::endl;
    if (!this->luaState)
        return;
    lua_close(this->luaState);
    this->luaState = nullptr;
}


bool LuaHandler::loadFile(const std::string fileName) {
    if (this->luaState && (fileName.length() > 0)) {
        luaL_loadfile(this->luaState, fileName.c_str());
        if (lua_pcall(this->luaState, 0, 0, 0) == LUA_OK)
            return true;
    }
    std::cout << lua_tostring(this->luaState, -1) << std::endl;
    return false;
}


bool LuaHandler::getInt(const char* name, int& value) {
    bool res = false;
    if (!this->getGlobal(name))
        return res;
    if (lua_isnumber(this->luaState, -1) == false) {
        lua_pop(this->luaState, 1);
        return res;
    }
    value = (int)(lua_tointeger(this->luaState, -1));
    lua_pop(this->luaState, 1);
    res = true;
    return res;
}


bool LuaHandler::getString(const char* name, std::string& value) {
    if (!this->getGlobal(name))
        return false;
    if (lua_type(this->luaState, -1) != LUA_TSTRING) {
        lua_pop(this->luaState, 1);
        return false;
    }
    value = lua_tostring(this->luaState, -1);
    lua_pop(this->luaState, 1);
    return true;
}


Player* LuaHandler::getPlayer() {
    if (!this->getGlobal("Player"))
        return nullptr;
   
    lua_getglobal(this->luaState, "Player");
    lua_pushstring(this->luaState, "name");
    lua_gettable(this->luaState, -2);
    const char* playerName = lua_tostring(this->luaState, -1);
    lua_Number playerX = getFieldInt("Player", "x");
    lua_Number playerY = getFieldInt("Player", "y");
    lua_Number playerWidth = getFieldInt("Player", "width");
    lua_Number playerHeight = getFieldInt("Player", "height");
    return new Player(playerName, (float)playerX, (float)playerY, (float)playerWidth, (float)playerHeight);
}


lua_Number LuaHandler::getFieldInt(const char* objectName, const char* propertyName) {
    lua_getglobal(this->luaState, objectName);
    lua_getfield(this->luaState, -1, propertyName);
    lua_Number r = lua_tonumber(this->luaState, -1);
    lua_pop(this->luaState, -1);
    return r;
}


bool LuaHandler::getGlobal(const char* name) {
    if ((!this->luaState) || (!name))
        return false;
    lua_getglobal(this->luaState, name);
    return true;
}
