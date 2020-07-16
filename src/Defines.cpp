/*
 * For Gold and Sweetrolls
 * Copyright (C) 2020 Piotr Wróblewski <szczypiorofix@o2.pl>
 */

#include "Defines.h"



const u16 DEFAULT_SCREEN_WIDTH		    = 800;
const u16 DEFAULT_SCREEN_HEIGHT		    = 600;


const float MIN_SCALE           = 1.0f;
const float MAX_SCALE           = 5.0f;


const cu8 DIR_UP                = 0;
const cu8 DIR_RIGHT			    = 1;
const cu8 DIR_DOWN			    = 2;
const cu8 DIR_LEFT			    = 3;
const cu8 DIR_UP_RIGHT		    = 4;
const cu8 DIR_DOWN_RIGHT        = 5;
const cu8 DIR_DOWN_LEFT		    = 6;
const cu8 DIR_UP_LEFT		    = 7;


const char* DIR_RES_IMAGES		= "../res/images/";
const char* DIR_RES_MUSIC		= "../res/music/";
const char* DIR_RES				= "../res/";
const char* DIR_RES_FONTS		= "../res/fonts/";
const char* DIR_RES_SHADERS     = "../res/shaders/";
const char* DIR_RES_SCRIPTS		= "../res/scripts/";






cu16 TS_EMPTY = 0;
cu16 TS_FOUNTAIN_BLOOD1 = 1;
cu16 TS_FOUNTAIN_BLOOD2 = 2;
cu16 TS_FOUNTAIN_WATER1 = 3;
cu16 TS_FOUNTAIN_WATER2 = 4;
cu16 TS_COOKIE = 5;
cu16 TS_CHEST1 = 6;
cu16 TS_CHEST2 = 7;
cu16 TS_CHEST2_OPENED = 8;
cu16 TS_FOUNTAIN_EMPTY = 9;
cu16 TS_BOX_WOODEN = 10;
cu16 TS_DEBRIS_GREEN1 = 11;
cu16 TS_DEBRIS_GREEN2 = 12;
cu16 TS_DEBRIS_GREEN3 = 13;
cu16 TS_DEBRIS_GREEN4 = 14;
cu16 TS_COFFIN_MUMMY = 15;
cu16 TS_FOUNTAIN_MAGIC_WATER1 = 16;
cu16 TS_FOUNTAIN_MAGIC_WATER2 = 17;
cu16 TS_EMPTY_BLACK = 18;
cu16 TS_POLE_MAGIC_PINK = 19;
cu16 TS_ALTAR_DESTROYED = 20;
cu16 TS_STATUE_EMPTY = 21;
cu16 TS_STATUE_WARRIOR = 22;
cu16 TS_STATUE_TURTLE = 23;
cu16 TS_STATUE_TURTLE_CLOCK1 = 24;
cu16 TS_STATUE_TURTLE_CLOCK2 = 25;
cu16 TS_STATUE_TURTLE_CLOCK3 = 26;
cu16 TS_STATUE_TURTLE_CLOCK4 = 27;
cu16 TS_STATUE_TURTLE_CLOCK5 = 28;
cu16 TS_STATUE_TURTLE_CLOCK6 = 29;
cu16 TS_STATUE_TURTLE_CLOCK7 = 30;
cu16 TS_STATUE_TURTLE_CLOCK8 = 31;
cu16 TS_STATUE_POT = 32;
cu16 TS_STATUE_VERY_OLD = 33;
cu16 TS_STATUE_GREEN1 = 34;
cu16 TS_STATUE_GREEN2 = 35;
cu16 TS_STATUE_GREEN3 = 36;
cu16 TS_STATUE_GREEN4 = 37;
cu16 TS_STATUE_GREEN5 = 38;
cu16 TS_STATUE_GREEN6 = 39;
cu16 TS_STATUE_GREEN7 = 40;
cu16 TS_STATUE_GREEN8 = 41;
cu16 TS_STATUE_GREEN9 = 42;
cu16 TS_STATUE_GREEN10 = 43;
cu16 TS_STATUE_GREEN11 = 44;
cu16 TS_STATUE_SKULLS = 45;
cu16 TS_STATUE_VIOLET_HOLE = 46;
cu16 TS_STATUE_FLAME1 = 47;
cu16 TS_STATUE_FLAME2 = 48;
cu16 TS_STATUE_FLAME3 = 49;
cu16 TS_STATUE_FLAME4 = 50;
cu16 TS_STATUE_FLAME5 = 51;
cu16 TS_STATUE_FLAME6 = 52;
cu16 TS_STATUE_FLAME7 = 53;
cu16 TS_STATUE_FLAME8 = 54;


cu16 TS_SIGN_ON_WALL = 1357;


cu16 TS_TORCH_PUT_OUT = 1358;
cu16 TS_TORCH1 = 1359;
cu16 TS_TORCH2 = 1360;
cu16 TS_TORCH3 = 1361;
cu16 TS_TORCH4 = 1362;


cu16 TS_ARROW_N = 1485;
cu16 TS_ARROW_NE = 1486;
cu16 TS_ARROW_E = 1487;
cu16 TS_ARROW_SE = 1488;
cu16 TS_ARROW_S = 1489;
cu16 TS_ARROW_SW = 1490;
cu16 TS_ARROW_W = 1491;
cu16 TS_ARROW_NW = 1492;


cu16 TS_UI_CANCEL = 1734;
cu16 TS_UI_ACCEPT = 1735;







Vector2::Vector2() {
    this->x = 0.0f;
    this->y = 0.0f;
}


Vector2::Vector2(float x, float y) {
    this->x = x;
    this->y = y;
}


Vector2::~Vector2() {
    this->x = 0.0f;
    this->y = 0.0f;
}


Vector2 Vector2::operator+(Vector2 v) {
    return Vector2(this->x + v.x, this->y + v.y);
}


Vector2 Vector2::operator-(Vector2 v) {
    return Vector2(this->x - v.x, this->y - v.y);
}


Vector2& Vector2::operator+=(Vector2& v) {
    this->x += v.x;
    this->y += v.y;
    return *this;
}


Vector2& Vector2::operator-=(Vector2& v) {
    this->x -= v.x;
    this->y -= v.y;
    return *this;
}




int stringToInt(const char* a) {
    int c = 0, sign = 0, offset = 0, n = 0;
    if (a[0] == '-') {
        sign = -1;
    }
    if (sign == -1) {
        offset = 1;
    } else {
        offset = 0;
    }
    n = 0;
    for (c = offset; a[c] != '\0'; c++) {
        n = n * 10 + a[c] - '0';
    }
    if (sign == -1) {
        n = -n;
    }
    return n;
}


CE_ArrayInt* stringToArrayOfInt(std::string s) {
    int str_length = s.length();
    int* arr { new int[str_length + 1] { 0 } };
    int i = 0, j = 0;
    while (s[i] != '\0') {
        if ((s[i] != 13 && s[i] != 32 && s[i] != 10)) {
            if (s[i] == ',') {
                j++;
            }
            else {
                arr[j] = arr[j] * 10 + (s[i] - 48);
            }
        }
        i++;
    }
    j++;
    arr[j] = arr[j] * 10 + (s[i] - 48);
    CE_ArrayInt* array = new CE_ArrayInt();
    array->size = j;
    array->arr = arr;
    return array;
}


std::string charArrayToString(char* a) {
    std::string str = "";
    int c = 0;
    while (a[c] != '\0') {
        str = str + a[c];
        c++;
    }
    return str;
}

