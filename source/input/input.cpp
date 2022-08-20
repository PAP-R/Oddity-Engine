#pragma once
#include "input.h"

#define VK_USEPLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#define GLFW_EXPOSED_NATIVE_WIN32

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <stdio.h>
#include <vector>

#include "../character/character.cpp"
#include "../character/movement.h"

#define MOUSE_OFFSET GLFW_KEY_LAST + 1

namespace Input {

	std::vector<KeyFunctions> keymap;

	void (*fullscreenFunc)();
	Character* _character;

	double speed = 1.0;
	double jump = 1.0;
	float sens = 10;

	void setup(void (*fullscreen)(), Character* character, void (*addVertex)()) {
		fullscreenFunc = fullscreen;
		_character = character;


		keymap.resize(GLFW_KEY_LAST + GLFW_MOUSE_BUTTON_LAST + 2);

		keymap[GLFW_KEY_F11] = (KeyFunctions{fullscreen, nullptr});

		keymap[GLFW_KEY_W] = (KeyFunctions{ []() { _character->moveX(speed); }, []() { _character->moveX(0.0); } });
		keymap[GLFW_KEY_S] = (KeyFunctions{ []() { _character->moveX(-speed); }, []() { _character->moveX(0.0); } });
		keymap[GLFW_KEY_A] = (KeyFunctions{ []() { _character->moveZ(-speed); }, []() { _character->moveZ(0.0); } });
		keymap[GLFW_KEY_D] = (KeyFunctions{ []() { _character->moveZ(speed); }, []() { _character->moveZ(0.0); } });
		keymap[GLFW_KEY_SPACE] = (KeyFunctions{ []() { _character->pushY(jump); }, []() { _character->pushY(0.0); } });
		keymap[MOUSE_OFFSET + GLFW_MOUSE_BUTTON_LEFT] = (KeyFunctions{ addVertex, []() {printf("Mouse button released\n"); }});
		keymap[MOUSE_OFFSET + GLFW_MOUSE_BUTTON_3] = (KeyFunctions{ []() { sens = sens > 1 ? sens - 1 : sens; }, []() { ; } });
		keymap[MOUSE_OFFSET + GLFW_MOUSE_BUTTON_4] = (KeyFunctions{ []() { sens = sens + 1; }, []() { ; } });
	}

	void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
		switch (action) {
		case GLFW_PRESS:
			if (keymap[key].activate) (*keymap[key].activate)();
			break;
		case GLFW_RELEASE:
			if (keymap[key].deactivate) (*keymap[key].deactivate)();
			break;
		}
	}

	int rotationX = 0, rotationY = 0;


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
		switch (action) {
		case GLFW_PRESS:
			if (keymap[button + MOUSE_OFFSET].activate) (*keymap[button + MOUSE_OFFSET].activate)();
			break;
		case GLFW_RELEASE:
			if (keymap[button + MOUSE_OFFSET].deactivate) (*keymap[button + MOUSE_OFFSET].deactivate)();
			break;
		}
	}

	void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
		speed = speed + yoffset;
		jump = jump + xoffset;

		printf("Jump : %f | Speed : %f\n", jump, speed);
	}
}