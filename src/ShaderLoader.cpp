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
	printf("Delete shaders ...\n");
	glDeleteProgram(this->programID);
}



void ShaderLoader::compileShaders(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath) {
	// Create Vertex Array Object
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Create a Vertex Buffer Object and copy the vertex data to it
	GLuint vbo;
	glGenBuffers(1, &vbo);

	GLfloat vertices[] = {
		-0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // Top-left
		 0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // Top-right
		 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // Bottom-right
		-0.5f, -0.5f, 1.0f, 1.0f, 1.0f  // Bottom-left
	};

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Create an element array
	GLuint ebo;
	glGenBuffers(1, &ebo);

	GLuint elements[] = {
		0, 1, 2,
		2, 3, 0
	};

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

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
	glBindFragDataLocation(this->programID, 0, "outColor");
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
	std::ifstream f(DIR_RES_SHADERS + filePath);
	std::string str = "";
	if (f) {
		std::ostringstream ss;
		ss << f.rdbuf();
		str = ss.str();
	}
	f.close();

	const char* contentsPtr = str.c_str();
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
		std::printf("%s\n", &(errorLog[0]));
		printf("Shader %s failed to compile.\n", filePath.c_str());
		return;
	}
}


void ShaderLoader::use() {
	glUseProgram(this->programID);

	this->posAttrib = glGetAttribLocation(this->programID, "position");
	glEnableVertexAttribArray(this->posAttrib);
	glVertexAttribPointer(this->posAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);

	this->colAttrib = glGetAttribLocation(this->programID, "color");
	glEnableVertexAttribArray(this->colAttrib);
	glVertexAttribPointer(this->colAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
}


void ShaderLoader::unuse() {
	glUseProgram(0);

	this->posAttrib = glGetAttribLocation(this->programID, "position");
	glDisableVertexAttribArray(this->posAttrib);

	this->colAttrib = glGetAttribLocation(this->programID, "color");
	glDisableVertexAttribArray(this->colAttrib);
}
