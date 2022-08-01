#pragma once

#define VK_USEPLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#define GLFW_EXPOSED_NATIVE_WIN32

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <stdio.h>

#include "../character/character.cpp"

namespace Input {
	typedef struct {
		void (*activate)();
		void (*deactivate)();
	} KeyFunctions;

	void setup(void (*fullscreen)(), Character* character);

	void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

	void cursorCallback(GLFWwindow* window, double xpos, double ypos);

	void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

	void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
}
