#ifndef DEVICE_H
#define DEVICE_H
#include <vulkan/vulkan_core.h>

VkPhysicalDevice create_physical_device(VkInstance instance);
VkDevice create_device(VkInstance instance);

#endif //DEVICE_H
