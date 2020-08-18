/*
 * For Gold and Sweetrolls
 * Copyright (C) 2020 Piotr Wróblewski <szczypiorofix@o2.pl>
 */

#include "Camera2D.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


Camera2D::Camera2D() : position(0.0f, 0.0f),
	cameraMatrix(1.0f),
	orthoMatrix(1.0f),
	scale(100.0f),
	needsMatrixUpdate(true),
	screenWidth(0),
	screenHeight(0) {

}


Camera2D::~Camera2D() {}


float Camera2D::getPositionX() {
	return this->position.x;
}


float Camera2D::getPositionY() {
	return this->position.y;
}


void Camera2D::update(void) {

	if (this->needsMatrixUpdate) {
		glm::vec3 translate(-position.x, -position.y, 0.0f);
		this->cameraMatrix = glm::translate(this->orthoMatrix, translate);
		glm::vec3 s(this->scale, this->scale, 0.0f);
		this->cameraMatrix = glm::scale(this->cameraMatrix, s);
		
		this->needsMatrixUpdate = false;
		/*printf("Needs update\n");*/
		/*printf("Camera matrix: %.2f\n", this->cameraMatrix[0][0]);*/
		//printf("Camera matrix: %.2f:%.2f\n", this->cameraMatrix[0][0], this->cameraMatrix[1][1]);
	}

}


void Camera2D::init(int _screenWidth, int _screenHeight) {
	this->screenWidth = _screenWidth;
	this->screenHeight = _screenHeight;
	this->orthoMatrix = glm::ortho(0.0f, (float)_screenWidth, 0.0f, (float)_screenHeight);
}
