/*
 * Dungeon Engine
 * Copyright (C) 2020 szczypiorofix <szczypiorofix@o2.pl>
 */


#include "XMLHelper.h"

int XMLHelper::xmlCharToInt(const xmlChar* a) {
	int c = 0, sign = 0, offset = 0, n = 0;
	if (a[0] == '-') {
		sign = -1;
	}
	if (sign == -1) {
		offset = 1;
	}
	else {
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


short XMLHelper::xmlCharToShort(const xmlChar* a) {
	short c = 0, sign = 0, offset = 0, n = 0;
	if (a[0] == '-') {
		sign = -1;
	}
	if (sign == -1) {
		offset = 1;
	}
	else {
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


int XMLHelper::readPropInt(xmlNodePtr node, const xmlChar* prop) {
	xmlChar* c = xmlGetProp(node, prop);
	int s = 0;
	if (c != NULL) {
		s = xmlCharToInt(c);
		xmlFree(c);
	}
	return s;
}


short XMLHelper::readPropShort(xmlNodePtr node, const xmlChar* prop) {
	xmlChar* c = xmlGetProp(node, prop);
	short s = 0;
	if (c != NULL) {
		s = xmlCharToShort(c);
		xmlFree(c);
	}
	return s;
}
