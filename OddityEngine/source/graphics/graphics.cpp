#pragma once

#include "graphics.h"


#include <string>


#include "vulkan.cpp"


namespace Graphics {
	class Engine {
	private:
		GLFWwindow* window;
		Vulkan vulkan;

		bool framebufferResized = false;

	public:
		void start(int width, int height, std::string title) {
			Engine::framebufferResized = false;
			initWindow(width, height, title);
			vulkan.start(window);
		}

		void stop() {
			vulkan.stop();
		}

		bool update() {
			glfwPollEvents();
			vulkan.update(&framebufferResized);
			return !glfwWindowShouldClose(window);
		}

		void add() {

		}

		void remove() {

		}

	private:
		void initWindow(int width, int height, std::string title) {
			glfwInit();

			glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
			glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);


			GLFWmonitor* monitor = glfwGetPrimaryMonitor();
			const GLFWvidmode* mode = glfwGetVideoMode(monitor);

			glfwWindowHint(GLFW_RED_BITS, mode->redBits);
			glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
			glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
			glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

			window = glfwCreateWindow(mode->width, mode->height, title.c_str(), monitor, NULL);
			glfwSetWindowUserPointer(window, this);
			glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
		}

		static void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
			auto app = reinterpret_cast<Engine*>(glfwGetWindowUserPointer(window));
			app->framebufferResized = true;
		}
	};
}