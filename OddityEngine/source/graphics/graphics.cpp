#pragma once

#include "graphics.h"


#include <string>
#include <chrono>


#include "vulkan.cpp"
#include "../character/movement.h"


namespace Graphics {
	class Engine {
	private:
		GLFWwindow* window;
		GLFWmonitor* monitor;
		Vulkan vulkan;

		const GLFWvidmode* standardmode;


		int posx = 0;
		int posy = 0;
		int width = 400;
		int height = 400;

		bool framebufferResized = false;

		bool fullscreen = true;


	public:
		void start(int width, int height, std::string title) {
			Engine::framebufferResized = false;
			initWindow(width, height, title);
			vulkan.start(window);
		}

		void stop() {
			vulkan.stop();
		}

		bool update(Movement movement) {
			glfwPollEvents();
			vulkan.update(&framebufferResized, movement);
			return !glfwWindowShouldClose(window);
		}

		void fullScreen() {
			if (fullscreen) {
				glfwGetWindowPos(window, &posx, &posy);
				glfwGetWindowSize(window, &width, &height);

				glfwSetWindowMonitor(window, monitor, 0, 0, standardmode->width, standardmode->height, standardmode->refreshRate);
				fullscreen = false;
			}
			else {
				glfwSetWindowMonitor(window, NULL, posx, posy, width, height, standardmode->refreshRate);
				fullscreen = true;
			}

			printf("Fullscreen Toggle\n");
		}

		void setKeyCallback(GLFWkeyfun callback) {
			glfwSetKeyCallback(window, callback);
		}

		void setCursorPositionCallback(GLFWcursorposfun callback) {
			glfwSetCursorPosCallback(window, callback);
		}

		void setMouseButtonCallback(GLFWmousebuttonfun callback) {
			glfwSetMouseButtonCallback(window, callback);
		}

		void setScrollCallback(GLFWscrollfun callback) {
			glfwSetScrollCallback(window, callback);
		}

		void add() {

		}

		void remove() {

		}

	private:
		void initWindow(int width, int height, std::string title) {
			this->width = width;
			this->height = height;

			glfwInit();

			monitor = glfwGetPrimaryMonitor();
			standardmode = glfwGetVideoMode(monitor);

			glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
			glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

			const GLFWvidmode* mode = glfwGetVideoMode(monitor);

			glfwWindowHint(GLFW_RED_BITS, mode->redBits);
			glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
			glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
			glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

			window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
			glfwSetWindowUserPointer(window, this);
			glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);

			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}

		static void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
			auto app = reinterpret_cast<Engine*>(glfwGetWindowUserPointer(window));
			app->framebufferResized = true;
		}
	};
}