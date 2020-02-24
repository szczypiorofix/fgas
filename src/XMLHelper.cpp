/*
 * For Gold and Sweetrolls
 * Copyright (C) 2020 Piotr Wróblewski <szczypiorofix@o2.pl>
 */


#include "XMLHelper.h"

int XMLHelper::xmlCharToInt(const xmlChar* a) {
	int c = 0;
	int sign = 0;
	int offset = 0;
	int n = 0;
	if (a[0] == '-') {
		sign = -1;
	}
	if (sign == -1) {
		offset = 1;
	} else {
		offset = 0;
	}
	for (c = offset; a[c] != '\0'; c++) {
		n = n * 10 + a[c] - '0';
	}
	if (sign == -1) {
		n = -n;
	}
	return n;
}


short XMLHelper::xmlCharToShort(const xmlChar* a) {
	short c = 0;
	short sign = 0;
	short offset = 0;
	short n = 0;
	if (a[0] == '-') {
		sign = -1;
	}
	if (sign == -1) {
		offset = 1;
	} else {
		offset = 0;
	}
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
	int i = 0;
	if (c != NULL) {
		i = xmlCharToInt(c);
		xmlFree(c);
	}
	return i;
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
