#pragma once

#define VK_USEPLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#define GLFW_EXPOSED_NATIVE_WIN32

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <stdio.h>

#include "../character/character.cpp"
#include "../character/movement.h"

namespace Input {

	void (*fullscreenFunc)();
	Character* _character;

	float speed = 0.1f;

	void setup(void (*fullscreen)(), Character* character) {
		fullscreenFunc = fullscreen;
		_character = character;
	}

	void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {

		if (action == GLFW_PRESS) {
			switch (key) {
			case 300:
				(*fullscreenFunc)();
				break;
			case 'W':
				_character->setVelocity(0.0f, speed, 0.0f);
				break;
			case 'A':
				_character->setVelocity(-speed, 0.0f, 0.0f);
				break;
			case 'S':
				_character->setVelocity(0.0f, -speed, 0.0f);
				break;
			case 'D':
				_character->setVelocity(speed, 0.0f, 0.0f);
				break;
			}
		}

		printf("%d | %d | %d | %d\n", key, scancode, action, mods);
	}

	int rotationX = 0, rotationY = 0;

	float sens = 10;

	int positve_modulo(int i, int n) {
		return (i % n + n) % n;
	}

	void cursorCallback(GLFWwindow* window, double xpos, double ypos) {
		float sensitivity = sens / 100;
		rotationX = positve_modulo(rotationX + static_cast<int>(xpos), 360 / sensitivity);
		rotationY = rotationY - static_cast<int>(ypos);
		rotationY = rotationY < (Y_MIN / sensitivity) ? (Y_MIN / sensitivity) : rotationY >(Y_MAX / sensitivity) ? (Y_MAX / sensitivity) : rotationY;
		_character->setDir(static_cast<double>(rotationX) * sensitivity, static_cast<double>(rotationY) * sensitivity);
		glfwSetCursorPos(window, 0, 0);
	}

	void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
		printf("%d | %d | %d\n", button, action, mods);
		if (action == GLFW_PRESS) {
			switch (button) {
			case 3:
				sens = sens > 1 ? sens - 1 : sens;
				break;
			case 4:
				sens = sens + 1;
				break;
			}
			printf("%f\n", sens);
		}
	}
}