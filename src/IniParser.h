#ifndef _INIPARSER_H_
#define _INIPARSER_H_
#pragma once


#include <string>
#include <vector>


enum class ValueType{
	INTEGER,
	DECIMAL,
	STRING
};


typedef union IniType {
	int integer;
	float decimal;
	std::string* string;
} IniType;


struct KeyValue {
	std::string name;
	ValueType unionType;
	IniType value;
	
	// constructor
	KeyValue(std::string _n, IniType _v, ValueType _t) : name(_n), value(_v), unionType(_t) {}
};


class IniParser {

public:
	IniParser();
	~IniParser();

	bool readFile(const char* _fileName);
	bool writeFile(const char* _fileName, std::vector<KeyValue*> _items);

	std::vector<KeyValue*> getItems();

	int getInteger(const char* _name);
	std::string getString(const char* _name);
	float getDecimal(const char* _name);

private:
	std::vector<KeyValue*> items;
};

#endif
