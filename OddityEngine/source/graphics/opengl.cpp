#include "graphics.h"
#include "graphics.cpp"

#include <string>

class OpenGL {
	GLFWwindow* window;

	void initWindow(int width, int height, std::string title) {
		glfwInit();

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

		window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
		glfwSetWindowUserPointer(window, this);
		glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
	}

	static void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
		auto app = reinterpret_cast<Graphics*>(glfwGetWindowUserPointer(window));
		app->framebufferResized = true;
	}
};
