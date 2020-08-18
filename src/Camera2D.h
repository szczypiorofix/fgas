/*
 * For Gold and Sweetrolls
 * Copyright (C) 2020 Piotr Wróblewski <szczypiorofix@o2.pl>
 */

#ifndef _CAMERA2D_H_
#define _CAMERA2D_H_
#pragma once

#include <glm/glm.hpp>


class Camera2D {
public:
	Camera2D();
	~Camera2D();

	void init(int screenWidth, int screenHeight);
	void update(void);

	void setPosition(const glm::vec2& newPosition) { this->position = newPosition; this->needsMatrixUpdate = true; }
	void setScale(float newScale) { this->scale = newScale; this->needsMatrixUpdate = true; }

	glm::vec2 getPosition() { return this->position; }
	float getPositionX();
	float getPositionY();
	float getScale(void) { return this->scale; }
	glm::mat4 getCameraMatrix() { return this->cameraMatrix; }

private:
	int screenWidth, screenHeight;
	bool needsMatrixUpdate;
	float scale;
	glm::vec2 position;
	glm::mat4 cameraMatrix;
	glm::mat4 orthoMatrix;
};

#endif
