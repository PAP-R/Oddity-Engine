#pragma once

#ifndef ODDITYENGINECMAKE_GRAPHICLIBS_H
#define ODDITYENGINECMAKE_GRAPHICLIBS_H

#define VALIDATE true

#define VMA_IMPLEMENTATION
#define VMA_STATIC_VULKAN_FUNCTIONS 0
#define VMA_DYNAMIC_VULKAN_FUNCTIONS 1
//#include <vk_mem_alloc.h>

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

#include <vulkan/vulkan_core.h>

#endif //ODDITYENGINECMAKE_GRAPHICLIBS_H
