/*
 * For Gold and Sweetrolls
 * Copyright (C) 2020 Piotr Wróblewski <szczypiorofix@o2.pl>
 */

#ifndef _DEFINES_H_
#define _DEFINES_H_
#pragma once

#include <string>



extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;


extern const float MIN_SCALE;
extern const float MAX_SCALE;


extern const int DIR_UP;
extern const int DIR_RIGHT;
extern const int DIR_DOWN;
extern const int DIR_LEFT;
extern const int DIR_UP_RIGHT;
extern const int DIR_DOWN_RIGHT;
extern const int DIR_DOWN_LEFT;
extern const int DIR_UP_LEFT;


extern const char* DIR_RES_IMAGES;
extern const char* DIR_RES_MUSIC;
extern const char* DIR_RES;
extern const char* DIR_FONTS;
extern const char* DIR_SHADERS;
extern const char* DIR_SCRIPTS;


typedef struct DG_ArrayInt {
    unsigned int size;
    int* arr;
} DG_ArrayInt;



enum class State {
    SPLASH_SCREEN,
    MAIN_MENU,
    GAME,
    QUIT
};



int stringToInt(const char* a);

std::string charArrayToString(char* a);

DG_ArrayInt* stringToArrayOfInt(std::string s);


#endif
