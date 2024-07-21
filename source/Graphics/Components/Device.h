#ifndef DEVICE_H
#define DEVICE_H

#include <vulkan/vulkan_core.h>

#include <vector>

struct Device {
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	VkDevice logicalDevice = VK_NULL_HANDLE;

	VkQueue graphicsQueue = VK_NULL_HANDLE;
	VkQueue computeQueue = VK_NULL_HANDLE;
	VkQueue presentQueue = VK_NULL_HANDLE;

	static VkPhysicalDevice create_physical_device(const VkInstance &instance, const VkSurfaceKHR &surface, const std::vector<const char *> &deviceExtensions);

	VkDevice create_logical_device(const VkInstance &instance, const VkSurfaceKHR &surface, const std::vector<const char *> &deviceExtensions, const VkPhysicalDevice &physicalDevice);

	void init(const VkInstance &instance, const VkSurfaceKHR &surface, const std::vector<const char *> &deviceExtensions);

	Device() = default;
	Device(const VkInstance &instance, const VkSurfaceKHR &surface, const std::vector<const char *> &deviceExtensions);
	~Device();

	operator VkPhysicalDevice() const;
	operator VkDevice() const;
};

#endif //DEVICE_H
