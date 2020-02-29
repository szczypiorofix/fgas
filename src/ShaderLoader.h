/*
 * For Gold and Sweetrolls
 * Copyright (C) 2020 Piotr Wróblewski <szczypiorofix@o2.pl>
 */

#ifndef _SHADERLOADER_H_
#define _SHADERLOADER_H_
#pragma once


#include <string>
#include <GL/glew.h>

class ShaderLoader {

public:
	ShaderLoader();
	~ShaderLoader();

	void compileShaders(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath);

	void use();
	void unuse();


private:
	
	GLuint programID;

	GLuint vertexShaderID;
	GLuint fragmentShaderID;

	GLint colAttrib;
	GLint posAttrib;

	void compileShaderFile(const std::string& filePath, GLuint& shaderId);

};


#endif
