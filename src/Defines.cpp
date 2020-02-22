/*
 * For Gold and Sweetrolls
 * Copyright (C) 2020 Piotr Wróblewski <szczypiorofix@o2.pl>
 */

#include "Defines.h"



const int SCREEN_WIDTH		    = 800;
const int SCREEN_HEIGHT		    = 600;


const float MIN_SCALE             = 1.0f;
const float MAX_SCALE             = 5.0f;


const int DIR_UP			    = 0;
const int DIR_RIGHT			    = 1;
const int DIR_DOWN			    = 2;
const int DIR_LEFT			    = 3;
const int DIR_UP_RIGHT		    = 4;
const int DIR_DOWN_RIGHT	    = 5;
const int DIR_DOWN_LEFT		    = 6;
const int DIR_UP_LEFT		    = 7;


const char* DIR_RES_IMAGES		= "../res/images/";
const char* DIR_RES_MUSIC		= "../res/music/";
const char* DIR_RES				= "../res/";
const char* DIR_RES_FONTS		= "../res/fonts/";
const char* DIR_SHADERS         = "../shaders/";
const char* DIR_RES_SCRIPTS		= "../res/scripts/";



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

DG_ArrayInt* stringToArrayOfInt(std::string s) {
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
    DG_ArrayInt* array = new DG_ArrayInt();
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
