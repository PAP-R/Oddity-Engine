#pragma once

#define GLM_FORCE_RADIANS

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <glm/ext.hpp>

#include <chrono>

#include "movement.h"
#include <iostream>

class Character {
private:
	Movement movement;

	std::chrono::steady_clock::time_point startTime = std::chrono::high_resolution_clock::now();

	std::chrono::steady_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

public:
	Movement getMovement() {
		return movement;
	}

	void setPos(float x, float y, float z) {
		movement.pos.x = x;
		movement.pos.y = y;
		movement.pos.z = z;
	}

	glm::vec3 getPos() {
		return movement.pos;
	}

	void setDir(float x, float y) {
		movement.dir.x = glm::cos(glm::radians(x));
		movement.dir.y = glm::sin(glm::radians(y));
		movement.dir.z = glm::sin(glm::radians(x));
	}

	void setDir(float x, float y, float z) {
		movement.dir.x = glm::cos(glm::radians(x));
		movement.dir.y = glm::sin(glm::radians(y));
		movement.dir.z = glm::sin(glm::radians(x));
	}

	glm::vec3 getDir() {
		return movement.dir;
	}

	void setVelocity(float x, float y, float z) {
		movement.velocity.x = x;
		movement.velocity.y = y;
		movement.velocity.z = z;

		std::cout << glm::to_string(movement.velocity) << std::endl;
	}

	void setAcceleration(float x, float y, float z) {
		movement.acceleration.x = x;
		movement.acceleration.y = y;
		movement.acceleration.z = z;
	}

	void update() {
		currentTime = std::chrono::high_resolution_clock::now();
		float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

		movement.velocity = movement.velocity + movement.acceleration * time;
		movement.pos = movement.pos + movement.velocity * movement.dir * time;

		//std::cout << glm::to_string(movement.velocity) << std::endl;

		startTime = std::chrono::high_resolution_clock::now();
	}
};