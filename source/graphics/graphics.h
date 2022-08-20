#pragma once

#include <string>
#include <optional>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define VK_USEPLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#define GLFW_EXPOSED_NATIVE_WIN32

#define GLM_FORCE_RADIANS

#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#define GLM_ENABLE_EXPERIMENTAL

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <glm/ext.hpp>


#define MAX_FRAMES_IN_FLIGHT 2
#define VALIDATE true


namespace Graphics {
	struct UniformBufferObject {
		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 proj;
	};
}