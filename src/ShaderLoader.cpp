/*
 * For Gold and Sweetrolls
 * Copyright (C) 2020 Piotr Wróblewski <szczypiorofix@o2.pl>
 */


#include "ShaderLoader.h"
#include <sstream>
#include <vector>
#include <fstream>
#include "Defines.h"


ShaderLoader::ShaderLoader() : programID(0), vertexShaderID(0), fragmentShaderID(0), colAttrib(0), posAttrib(0) {
}


ShaderLoader::~ShaderLoader() {
	debugInfo("Delete shaders.");
	glDeleteProgram(this->programID);
}



void ShaderLoader::compileShaders(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath) {
	
	// Create and compile the vertex shader
	this->vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	this->compileShaderFile(vertexShaderFilePath, this->vertexShaderID);
	
	// Create and compile the fragment shader
	this->fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	this->compileShaderFile(fragmentShaderFilePath, this->fragmentShaderID);

	// Link the vertex and fragment shader into a shader program
	this->programID = glCreateProgram();
	glAttachShader(this->programID, this->vertexShaderID);
	glAttachShader(this->programID, this->fragmentShaderID);
	//glBindFragDataLocation(this->programID, 0, "outColor");
	glLinkProgram(this->programID);
	
	GLint isLinked = 0;
	glGetProgramiv(this->programID, GL_LINK_STATUS, (int*)&isLinked);
	if (isLinked == GL_FALSE) {
		GLint maxLength = 0;
		glGetProgramiv(this->programID, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(this->programID, maxLength, &maxLength, &infoLog[0]);

		glDeleteProgram(this->programID);
		glDeleteShader(this->vertexShaderID);
		glDeleteShader(this->fragmentShaderID);

		printf("%s\n", &(infoLog[0]));
		printf("Shaders failed to link.\n");
	}

	glDetachShader(this->programID, this->vertexShaderID);
	glDetachShader(this->programID, this->fragmentShaderID);
	glDeleteShader(this->vertexShaderID);
	glDeleteShader(this->fragmentShaderID);

}


void ShaderLoader::compileShaderFile(const std::string& filePath, GLuint& shaderId) {

	std::string shaderCode;
	std::ifstream shaderFile;
	// ensure ifstream objects can throw exceptions:
	shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		shaderFile.open(DIR_RES_SHADERS + filePath);
		std::stringstream shaderStream;
		shaderStream << shaderFile.rdbuf();
		shaderFile.close();
		shaderCode = shaderStream.str();
	} catch (std::ifstream::failure e) {
		printf("ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ\n");
	}

	const char* contentsPtr = shaderCode.c_str();
	glShaderSource(shaderId, 1, &contentsPtr, NULL);
	glCompileShader(shaderId);

	GLint success = 0;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE) {
		GLint maxLength = 0;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<char> errorLog(maxLength);
		glGetShaderInfoLog(shaderId, maxLength, &maxLength, &errorLog[0]);
		glDeleteShader(shaderId);
		printf("%s\n", &(errorLog[0]));
		printf("Shader %s failed to compile.\n", filePath.c_str());
		return;
	}
	debugInfo("Shader " + filePath + "compiled successfuly.");
}


void ShaderLoader::setBool(const std::string& name, bool value) const {
	glUniform1i(glGetUniformLocation(this->programID, name.c_str()), (int)value);
}


void ShaderLoader::setInt(const std::string& name, int value) const {
	glUniform1i(glGetUniformLocation(this->programID, name.c_str()), value);
}


void ShaderLoader::setFloat(const std::string& name, float value) const {
	glUniform1f(glGetUniformLocation(this->programID, name.c_str()), value);
}


void ShaderLoader::use(GLuint textureId) {

	glUseProgram(this->programID);
	
}


void ShaderLoader::unuse() {
	glUseProgram(0);
}
