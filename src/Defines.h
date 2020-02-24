/*
 * For Gold and Sweetrolls
 * Copyright (C) 2020 Piotr Wróblewski <szczypiorofix@o2.pl>
 */

#ifndef _DEFINES_H_
#define _DEFINES_H_
#pragma once

#include <string>

// 8bit
typedef unsigned char                   u8;  //  0 - 255
typedef const unsigned char             cu8;
typedef signed char                     s8;  // -127 - 127
typedef const signed char               cs8;


// 16bit
typedef unsigned short                  u16; // 0 - 65535
typedef const unsigned short            cu16;
typedef signed short                    s16; // -32767 - 32767
typedef const signed short              cs16;


// 32bit
typedef unsigned long                   u32; // 0 - 4,294,967,295
typedef const unsigned long             cu32;
typedef signed long                     s32; // -2,147,483,647 - 2,147,483,647
typedef const signed long               cs32;


// 64bit
typedef unsigned long long              u64; // 0 - 18,446,744,073,709,551,615
typedef const unsigned long long        cu64;
typedef signed long long                s64; // -9,223,372,036,854,775,807 - 9,223,372,036,854,775,807
typedef const signed long long          cs64;






extern const u16 SCREEN_WIDTH;
extern const u16 SCREEN_HEIGHT;


extern const float MIN_SCALE;
extern const float MAX_SCALE;


extern const cu8 DIR_UP;
extern const cu8 DIR_RIGHT;
extern const cu8 DIR_DOWN;
extern const cu8 DIR_LEFT;
extern const cu8 DIR_UP_RIGHT;
extern const cu8 DIR_DOWN_RIGHT;
extern const cu8 DIR_DOWN_LEFT;
extern const cu8 DIR_UP_LEFT;


extern const char* DIR_RES_IMAGES;
extern const char* DIR_RES_MUSIC;
extern const char* DIR_RES;
extern const char* DIR_FONTS;
extern const char* DIR_SHADERS;
extern const char* DIR_SCRIPTS;


typedef struct DG_ArrayInt {
    u16 size;
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
