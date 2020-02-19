/*
 * For Gold and Sweetrolls
 * Copyright (C) 2020 Piotr Wróblewski <szczypiorofix@o2.pl>
 */


#include "ShaderLoader.h"

#include <vector>
#include <fstream>
#include "Defines.h"


ShaderLoader::ShaderLoader() : _programID(0), _vertexShaderID(0), _fragmentShaderID(0), _numAttributes(0) {
}


ShaderLoader::~ShaderLoader() {}



void ShaderLoader::compileShaders(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath) {

	_programID = glCreateProgram();

	_vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	if (_vertexShaderID == 0) {
		printf("Vertex Shader failed to create.");
	}

	_fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	if (_fragmentShaderID == 0) {
		printf("Fragment Shader failed to create.");
	}

	compileShader( DIR_SHADERS + vertexShaderFilePath, _vertexShaderID);
	compileShader( DIR_SHADERS + fragmentShaderFilePath, _fragmentShaderID);

}

void ShaderLoader::linkShaders() {
	
	glAttachShader(_programID, _vertexShaderID);
	glAttachShader(_programID, _fragmentShaderID);

	glLinkProgram(_programID);

	int isLinked = 0;
	glGetProgramiv(_programID, GL_LINK_STATUS, &isLinked);
	if (isLinked == GL_FALSE) {
		GLint maxLength = 0;
		glGetProgramiv(_programID, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(_programID, maxLength, &maxLength, &infoLog[0]);

		glDeleteProgram(_programID);
		glDeleteShader(_vertexShaderID);
		glDeleteShader(_fragmentShaderID);

		printf("%s\n", &infoLog[0]);
		printf("ERROR !!! Shaders failed to link !\n");
	}

	glDetachShader(_programID, _vertexShaderID);
	glDetachShader(_programID, _fragmentShaderID);
	glDeleteShader(_vertexShaderID);
	glDeleteShader(_fragmentShaderID);
}

void ShaderLoader::addAttribute(const std::string& attributeName) {
	glBindAttribLocation(
		_programID,
		_numAttributes,  // index position in GLSL list of variables, if there's 2 linked variables the first is 0, the second is 1
		attributeName.c_str()
	);
}

GLint ShaderLoader::getUniformLocation(const std::string& uniformName) {

	GLint location = glGetUniformLocation(_programID, uniformName.c_str());
	if (location == GL_INVALID_INDEX) {
		printf("Uniform %s not found in shader!\n", uniformName.c_str());
	}
	return location;
	
}

void ShaderLoader::use() {
	glUseProgram(_programID); 
	for (int i = 0; i < _numAttributes; i++) {
		glEnableVertexAttribArray(i);
	}
}

void ShaderLoader::unuse() {
	glUseProgram(_programID);
	for (int i = 0; i < _numAttributes; i++) {
		glDisableVertexAttribArray(i);
	}
}



void ShaderLoader::compileShader(const std::string& filePath, GLuint& id) {
	std::ifstream shaderFile(filePath);
	if (shaderFile.fail()) {
		perror(filePath.c_str());
		printf("Failed to open file %s\n", filePath.c_str());
	}

	std::string fileContent = "";
	std::string line;
	while (std::getline(shaderFile, line)) {
		fileContent += line + '\n';
	}
	shaderFile.close();

	const char* contentsPtr = fileContent.c_str();

	glShaderSource( id, 1, &contentsPtr, nullptr );  // 1 string becouse all is in one string

	glCompileShader(id);
	GLint success = 0;
	glGetShaderiv(id, GL_COMPILE_STATUS, &success);

	if (success == GL_FALSE) {
		GLint maxLength = 0;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<char> errorLog(maxLength);
		glGetShaderInfoLog(id, maxLength, &maxLength, &errorLog[0]);
		glDeleteShader(id);
		printf("%s\n", &(errorLog[0]));
		printf(("Shader %s failed to compile.\n", filePath.c_str()));
		return;
	}
	printf("Shader %s compiled successfully.\n", filePath.c_str());
}




