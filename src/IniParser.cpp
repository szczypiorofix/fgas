#include "IniParser.h"


#include <iostream>
#include <fstream>


IniParser::IniParser() {}


IniParser::~IniParser() {
    for (unsigned int i = 0; i < this->items.size(); i++) {
        delete this->items.at(i);
        this->items.at(i) = nullptr;
    }
}


bool IniParser::readFile(const char* _fileName) {
    std::ifstream fp(_fileName); //, std::ifstream::ate | std::ifstream::binary);
    if (fp.is_open()) {
        printf("INIPARSER: Reading %s file ...", _fileName);
        //std::cout << "File size: " << fp.tellg() << std::endl;
        std::string line;
        std::vector<std::string> lines;

        while (std::getline(fp, line)) {
            //std::cout << line << std::endl;
            if (line.size() < 3) {
                printf("File size is too small for ini file.\n");
                return false;
            }
            if (isdigit(line[0]) || (line.find('=') == std::string::npos)) {
                printf("File is not in a proper .ini format.\n");
                return false;
            }
            lines.push_back(line);
        }
        fp.close();

        std::string tempName = "";
        std::string tempValue = "";
        bool isValue = false;
        bool hasFloatingPoint = false;
        bool hasEqualChar = false;

        for (unsigned int i = 0; i < lines.size(); i++) {
            isValue = false;
            hasFloatingPoint = false;

            for (unsigned int j = 0; j < lines.at(i).size(); j++) {
                if (lines.at(i)[j] == '.') {
                    hasFloatingPoint = true;
                }
                if (lines.at(i)[j] != '=') {
                    if (!isValue) {
                        tempName += lines.at(i)[j];
                    }
                    else {
                        tempValue += lines.at(i)[j];
                    }
                }
                else {
                    hasEqualChar = true;
                    isValue = true;
                }
            }
            ValueType unionType = ValueType::INTEGER;
            IniType t;
            if (!isdigit( tempValue.c_str()[0] )) {
                t.string = new std::string(tempValue);
                unionType = ValueType::STRING;
            }
            else {
                if (hasFloatingPoint) {
                    t.decimal = std::stof(tempValue.c_str());
                    unionType = ValueType::DECIMAL;
                }
                else {
                    t.integer = atoi(tempValue.c_str());
                }
            }
            this->items.push_back(new KeyValue(tempName, t, unionType));
            tempName = "";
            tempValue = "";
            if (!hasEqualChar) {
                printf("INIPARSER: Unable to parse file. There's no '=' char.\n");
                return false;
            }
        }
        //printf("All stuff:\n");
        //for (unsigned int i = 0; i < this->items.size(); i++) {
        //    if (this->items.at(i)->unionType == ValueType::INTEGER)
        //        printf("Key: %s - value integer: %i\n", this->items.at(i)->name.c_str(), this->items.at(i)->value.integer);
        //    else if (this->items.at(i)->unionType == ValueType::STRING)
        //        printf("Key: %s - value string: %s\n", this->items.at(i)->name.c_str(), this->items.at(i)->value.string->c_str());
        //    else
        //        printf("Key: %s - value float: %f\n", this->items.at(i)->name.c_str(), this->items.at(i)->value.decimal);
        //}
    }
    else {
        printf("INIPARSER: Unable to open file %s.\n", _fileName);
        return false;
    }
    printf(" done.\n");
	return true;
}


bool IniParser::writeFile(const char* _fileName, std::vector<KeyValue*> _items) {
    if (_items.size() > 0) {
        std::ofstream myfile(_fileName);
        if (myfile.is_open()) {
            printf("INIPARSER: Writing settings to file %s ...", _fileName);
            for (unsigned int i = 0; i < _items.size(); i++) {
                switch (_items.at(i)->unionType) {
                case ValueType::INTEGER:
                    myfile << _items.at(i)->name.c_str() << "=" << _items.at(i)->value.integer << "\n";
                    break;
                case ValueType::STRING:
                    myfile << _items.at(i)->name.c_str() << "=" << _items.at(i)->value.string->c_str() << "\n";
                    break;
                case ValueType::DECIMAL:
                    myfile << _items.at(i)->name.c_str() << "=" << _items.at(i)->value.decimal << "\n";
                    break;
                default:
                    printf("Ooops! Something went wrong, mister.\n");
                    exit(0);
                    break;
                }
            }
            myfile.close();
            printf(" done.\n");
        }
        else {
            printf("Unable to open file!\n");
            return false;
        }
    }
    else {
        printf("Attribues are empty!\n");
        return false;
    }
    return true;
}


std::vector<KeyValue*> IniParser::getItems() {
    return this->items;
}


int IniParser::getInteger(const char* _name) {
    int r = 0;
    bool itemFound = false;
    for (unsigned int i = 0; i < this->items.size(); i++) {
        if (this->items.at(i)->unionType == ValueType::INTEGER) {
            if (this->items.at(i)->name.compare(_name) == 0) {
                r = this->items.at(i)->value.integer;
                itemFound = true;
            }
        }
    }
    if (!itemFound) {
        printf("Unable to find an item %s.\n", _name);
    }
    return r;
}


std::string IniParser::getString(const char* _name) {
    std::string s = "";
    bool itemFound = false;
    for (unsigned int i = 0; i < this->items.size(); i++) {
        if (this->items.at(i)->unionType == ValueType::STRING) {
            if (this->items.at(i)->name.compare(_name) == 0) {
                s = *this->items.at(i)->value.string;
                itemFound = true;
            }
        }
    }
    if (!itemFound) {
        printf("Unable to find an item %s.\n", _name);
    }
    return s;
}


float IniParser::getDecimal(const char* _name) {
    float r = 0.0f;
    bool itemFound = false;
    for (unsigned int i = 0; i < this->items.size(); i++) {
        if (this->items.at(i)->unionType == ValueType::DECIMAL) {
            if (this->items.at(i)->name.compare(_name) == 0) {
                r = this->items.at(i)->value.decimal;
                itemFound = true;
            }
        }
    }
    if (!itemFound) {
        printf("Unable to find an item %s.\n", _name);
    }
    return r;
}
