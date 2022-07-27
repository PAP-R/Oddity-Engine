#include "graphics.h"


#include <string>


#include "vulkan.cpp"

class Graphics {
private:
	Vulkan vulkan;

public:
	bool framebufferResized = false;


	void start(int width, int height, std::string title) {
		initWindow(width, height, title);
		vulkan.initVulkan();
	}

	void update() {

	}

	void add() {

	}

	void remove() {

	}

private:
	GLFWwindow* window;
	bool framebufferResized = false;

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
