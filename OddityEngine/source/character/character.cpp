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

	std::chrono::steady_clock::time_point startTime = std::chrono::high_resolution_clock::now();

	std::chrono::steady_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
	double time;

public:
	Movement movement;

	Movement getMovement() {
		return movement;
	}

	void setPos(double x, double y, double z) {
		movement.pos.x = x;
		movement.pos.y = y;
		movement.pos.z = z;
	}

	glm::vec3 getPos() {
		return movement.pos;
	}

	void setDir(double x, double y) {
		movement.dir.x = glm::cos(glm::radians(x));
		movement.dir.y = glm::sin(glm::radians(y));
		movement.dir.z = glm::sin(glm::radians(x));
		movement.dirN.x = -glm::sin(glm::radians(x));
		movement.dirN.y = glm::sin(glm::radians(y));
		movement.dirN.z = glm::cos(glm::radians(x));
	}

	void setDir(double x, double y, double z) {
		movement.dir.x = glm::cos(glm::radians(x));
		movement.dir.y = glm::sin(glm::radians(y));
		movement.dir.z = glm::sin(glm::radians(x));
		movement.dirN.x = -glm::sin(glm::radians(x));
		movement.dirN.y = glm::sin(glm::radians(y));
		movement.dirN.z = glm::cos(glm::radians(x));
	}

	glm::vec3 getDir() {
		return movement.dir;
	}

	void setVelocity(double x, double y, double z) {
		movement.velocity.x = x;
		movement.velocity.y = y;
		movement.velocity.z = z;
	}

	void addVelocity(double x, double y, double z) {
		movement.velocity.x += x;
		movement.velocity.y += y;
		movement.velocity.z += z;
	}

	void setVelocityX(double x) {
		movement.velocity.x = x;
	}

	void setVelocityY(double y) {
		movement.velocity.y = y;
	}

	void setVelocityZ(double z) {
		movement.velocity.z = z;
	}

	glm::vec3 getVelocity() {
		return movement.velocity;
	}

	void setAcceleration(double x, double y, double z) {
		movement.acceleration.x = x;
		movement.acceleration.y = y;
		movement.acceleration.z = z;
	}

	void addAcceleration(double x, double y, double z) {
		movement.acceleration.x += x;
		movement.acceleration.y += y;
		movement.acceleration.z += z;
	}

	void setAccelerationX(double x) {
		movement.acceleration.x = x;
	}

	void setAccelerationY(double y) {
		movement.acceleration.y = y;
	}

	void setAccelerationZ(double z) {
		movement.acceleration.z = z;
	}

	glm::vec3 getAcceleration() {
		return movement.acceleration;
	}

	void pushX(double x) {
		movement.push.x = x;
	}

	void pushY(double y) {
		movement.push.y = y;
	}

	void pushZ(double z) {
		movement.push.z = z;
	}

	void update() {
		currentTime = std::chrono::high_resolution_clock::now();
		time = std::chrono::duration<double, std::chrono::seconds::period>(currentTime - startTime).count();

		movement.velocity = movement.velocity + (movement.acceleration + movement.push) * time;
		glm::vec3 moveDir = glm::vec3((movement.velocity.x * movement.dir.x + movement.velocity.z * movement.dirN.x), movement.velocity.y, (movement.velocity.x * movement.dir.z + movement.velocity.z * movement.dirN.z));

		movement.pos = movement.pos + moveDir * time;

		movement.acceleration = lint(movement.acceleration, glm::vec3(0.0, -9.81, 0.0), 10.0, time);
		movement.velocity = lint(movement.velocity, glm::vec3(0.0, movement.velocity.y, 0.0), 10.0, time);

		if (movement.pos.y < 0) {
			movement.pos.y = 0;
			movement.velocity.y = 0;
		}

		//printf("V: %f | A: %f\n", movement.velocity.y, movement.acceleration.y);

		//std::cout << glm::to_string(movement.velocity) << std::endl;

		startTime = std::chrono::high_resolution_clock::now();
	}

private:
	glm::vec3 lint(glm::vec3 num, glm::vec3 goal, double stepsize, double time) {
		glm::vec3 result;
		for (size_t i = 0; i < 3; i++) {
			if (num[i] == goal[i]) {
				result[i] = num[i];
			}
			else if (num[i] > goal[i]) {
				double resultSingle = num[i] - stepsize * time;
				result[i] = resultSingle > goal[i] ? resultSingle : goal[i];
			}
			else {
				double resultSingle = num[i] + stepsize * time;
				result[i] = resultSingle < goal[i] ? resultSingle : goal[i];
			}
		}
		return result;
	}
};