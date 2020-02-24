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
	void linkShaders();
	void addAttribute(const std::string& attributeName);

	GLint getUniformLocation(const std::string& uniformName);

	void use();
	void unuse();

private:
	
	GLuint _programID;

	GLuint _vertexShaderID;
	GLuint _fragmentShaderID;

	int _numAttributes;

	void compileShader(const std::string& filePath, GLuint& id);

};


#endif
