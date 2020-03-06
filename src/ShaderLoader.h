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

	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;

private:

	GLuint programID;

	GLuint vertexShaderID;
	GLuint fragmentShaderID;

	GLuint vao;
	GLuint vbo;
	GLuint ebo;

	float c;

	GLint colAttrib;
	GLint posAttrib;

	void compileShaderFile(const std::string& filePath, GLuint& shaderId);

};


#endif
