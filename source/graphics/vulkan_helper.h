#pragma once

#include <array>
#include <optional>
#include <vector>

#include "graphics.h"

const std::vector<const char*> deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

struct QueueFamilyIndices {
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;

	bool isComplete() {
		return graphicsFamily.has_value() && presentFamily.has_value();
	}
};

struct SwapChainSupportDetails {
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;

	bool isCapable() {
		return !formats.empty() && !presentModes.empty();
	}
};

struct Vertex {
	glm::vec3 pos;
	glm::vec3 color;

	static VkVertexInputBindingDescription getBindingDescription() {
		VkVertexInputBindingDescription bindingDescription{
			.binding = 0,
			.stride = sizeof(Vertex),
			.inputRate = VK_VERTEX_INPUT_RATE_VERTEX
		};

		return bindingDescription;
	}

	static std::array<VkVertexInputAttributeDescription, 2> getAttributeDestions() {
		std::array<VkVertexInputAttributeDescription, 2> attributeDescription{};

		attributeDescription[0] = {
			.location = 0,
			.binding = 0,
			.format = VK_FORMAT_R32G32B32_SFLOAT,
			.offset = offsetof(Vertex, pos)
		};

		attributeDescription[1] = {
			.location = 1,
			.binding = 0,
			.format = VK_FORMAT_R32G32B32_SFLOAT,
			.offset = offsetof(Vertex, color)
		};

		return attributeDescription;
	}
};



std::vector<const char*> getRequiredExtensions();

VkPhysicalDevice pickPhysicalDevice(VkInstance instance, VkSurfaceKHR surface);

int getDeviceScore(VkPhysicalDevice device, VkSurfaceKHR surface);

QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);

bool checkDeviceExtensionSupport(VkPhysicalDevice device);

SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);

VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);

VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);

VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow* window);

std::vector<char> readFile(const std::string& filename);

VkShaderModule createShaderModule(const std::vector<char>& code, VkDevice device);

void createBuffer(VkDevice device, VkPhysicalDevice physicalDevice, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);

void resizeBuffer(VkDevice device, VkPhysicalDevice physicalDevice, VkDeviceSize prevSize, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory, VkCommandPool commandPool, VkQueue graphicsQueue);

uint32_t findMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties);

void copyBuffer(VkDevice device, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size, VkCommandPool commandPool, VkQueue graphicsQueue, size_t srcOffset = 0, size_t dstOffset = 0);

void addToBuffer(VkDevice device, VkPhysicalDevice physicalDevice, VkBuffer dstBuffer, VkDeviceMemory dstMemory, void* srcData, VkDeviceSize size, VkCommandPool commandPool, VkQueue graphicsQueue, size_t srcOffset = 0, size_t dstOffset = 0);

float degreeToRadian(int rot);

size_t sizeToBlock(size_t size, size_t blockSize);

VkFormat findDepthFormat(VkPhysicalDevice physicalDevice);

void createImage(VkDevice device, VkPhysicalDevice physicalDevice, uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);

void transitionImageLayout(VkDevice device, VkCommandPool commandPool, VkQueue queue, VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);

void copyBufferToImage(VkDevice device, VkCommandPool commandPool, VkQueue queue, VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

VkCommandBuffer beginSingleTimeCommands(VkDevice device, VkCommandPool commandPool);

void endSingleTimeCommands(VkDevice device, VkQueue graphicsQueue, VkCommandPool commandPool, VkCommandBuffer commandBuffer);

VkImageView createImageView(VkDevice device, VkImage image, VkFormat format);

//Debug
void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);