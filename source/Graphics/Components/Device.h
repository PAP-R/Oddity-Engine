#ifndef DEVICE_H
#define DEVICE_H

#include <vulkan/vulkan_core.h>

#include <vector>

struct Device {
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	VkDevice device = VK_NULL_HANDLE;

	VkQueue graphicsQueue = VK_NULL_HANDLE;
	VkQueue computeQueue = VK_NULL_HANDLE;
	VkQueue presentQueue = VK_NULL_HANDLE;

	void create_physical_device(const VkInstance &instance, const VkSurfaceKHR &surface, const std::vector<const char *> &deviceExtensions);

	void create_logical_device(const VkInstance &instance, const VkSurfaceKHR &surface, const std::vector<const char *> &deviceExtensions);

	Device() = default;
	Device(const VkInstance &instance, const VkSurfaceKHR &surface, const std::vector<const char *> &deviceExtensions);
	~Device();

	operator VkPhysicalDevice() const;
	operator VkDevice() const;
};

#endif //DEVICE_H
