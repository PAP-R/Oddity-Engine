#ifndef QUEUEFAMILIES_H
#define QUEUEFAMILIES_H

#include <vulkan/vulkan.h>

#include <vector>
#include <optional>


struct QueueFamilyIndices {
	std::optional<uint32_t> graphicsAndComputeFamily;
	std::optional<uint32_t> presentFamily;

	bool is_complete() {
		return graphicsAndComputeFamily.has_value() && presentFamily.has_value();
	}
};

QueueFamilyIndices find_queue_families(VkPhysicalDevice device, VkSurfaceKHR surface);

#endif //QUEUEFAMILIES_H
