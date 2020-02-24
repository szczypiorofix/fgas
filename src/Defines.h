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




// Tileset list

extern cu16 TS_EMPTY;
extern cu16 TS_FOUNTAIN_BLOOD1;
extern cu16 TS_FOUNTAIN_BLOOD2;
extern cu16 TS_FOUNTAIN_WATER1;
extern cu16 TS_FOUNTAIN_WATER2;
extern cu16 TS_COOKIE;
extern cu16 TS_CHEST1;
extern cu16 TS_CHEST2;
extern cu16 TS_CHEST2_OPENED;
extern cu16 TS_FOUNTAIN_EMPTY;
extern cu16 TS_BOX_WOODEN;
extern cu16 TS_DEBRIS_GREEN1;
extern cu16 TS_DEBRIS_GREEN2;
extern cu16 TS_DEBRIS_GREEN3;
extern cu16 TS_DEBRIS_GREEN4;
extern cu16 TS_COFFIN_MUMMY;
extern cu16 TS_FOUNTAIN_MAGIC_WATER1;
extern cu16 TS_FOUNTAIN_MAGIC_WATER2;
extern cu16 TS_EMPTY_BLACK;
extern cu16 TS_POLE_MAGIC_PINK;
extern cu16 TS_ALTAR_DESTROYED;
extern cu16 TS_STATUE_EMPTY;
extern cu16 TS_STATUE_WARRIOR;
extern cu16 TS_STATUE_TURTLE;
extern cu16 TS_STATUE_TURTLE_CLOCK1;
extern cu16 TS_STATUE_TURTLE_CLOCK2;
extern cu16 TS_STATUE_TURTLE_CLOCK3;
extern cu16 TS_STATUE_TURTLE_CLOCK4;
extern cu16 TS_STATUE_TURTLE_CLOCK5;
extern cu16 TS_STATUE_TURTLE_CLOCK6;
extern cu16 TS_STATUE_TURTLE_CLOCK7;
extern cu16 TS_STATUE_TURTLE_CLOCK8;
extern cu16 TS_STATUE_POT;
extern cu16 TS_STATUE_VERY_OLD;
extern cu16 TS_STATUE_GREEN1;
extern cu16 TS_STATUE_GREEN2;
extern cu16 TS_STATUE_GREEN3;
extern cu16 TS_STATUE_GREEN4;
extern cu16 TS_STATUE_GREEN5;
extern cu16 TS_STATUE_GREEN6;
extern cu16 TS_STATUE_GREEN7;
extern cu16 TS_STATUE_GREEN8;
extern cu16 TS_STATUE_GREEN9;
extern cu16 TS_STATUE_GREEN10;
extern cu16 TS_STATUE_GREEN11;
extern cu16 TS_STATUE_SKULLS;
extern cu16 TS_STATUE_VIOLET_HOLE;
extern cu16 TS_STATUE_FLAME1;
extern cu16 TS_STATUE_FLAME2;
extern cu16 TS_STATUE_FLAME3;
extern cu16 TS_STATUE_FLAME4;
extern cu16 TS_STATUE_FLAME5;
extern cu16 TS_STATUE_FLAME6;
extern cu16 TS_STATUE_FLAME7;
extern cu16 TS_STATUE_FLAME8;


extern cu16 TS_SIGN_ON_WALL;


// Torch

extern cu16 TS_TORCH_PUT_OUT;
extern cu16 TS_TORCH1;
extern cu16 TS_TORCH2;
extern cu16 TS_TORCH3;
extern cu16 TS_TORCH4;


// Arrow

extern cu16 TS_ARROW_N;
extern cu16 TS_ARROW_NE;
extern cu16 TS_ARROW_E;
extern cu16 TS_ARROW_SE;
extern cu16 TS_ARROW_S;
extern cu16 TS_ARROW_SW;
extern cu16 TS_ARROW_W;
extern cu16 TS_ARROW_NW;


// UI Elements

extern cu16 TS_UI_CANCEL;
extern cu16 TS_UI_ACCEPT;







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
