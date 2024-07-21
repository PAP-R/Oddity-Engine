#include "Device.h"

#include <vector>
#include <map>
#include <set>
#include <optional>

#include <Util/Debug.h>

#include "ValidationLayers.h"
#include "QueueFamilies.h"
#include "SwapChain.h"

bool check_device_extension_support(const VkPhysicalDevice& device, const std::vector<const char*>& deviceExtensions) {
	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

	std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

	for (const auto& extension : availableExtensions) {
		requiredExtensions.erase(extension.extensionName);
	}

	return requiredExtensions.empty();
}

size_t rate_device_suitability(const VkPhysicalDevice& device, const VkSurfaceKHR& surface, const std::vector<const char*> &deviceExtensions) {
        VkPhysicalDeviceProperties deviceProperties;
        vkGetPhysicalDeviceProperties(device, &deviceProperties);

        VkPhysicalDeviceFeatures deviceFeatures;
        vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

        Debug::message("Device {} :", deviceProperties.deviceName);

        if (auto indices = find_queue_families(device, surface); !(deviceFeatures.geometryShader && indices.is_complete() && check_device_extension_support(device, deviceExtensions))) {
            Debug::print("\tRequired features not available\n");
            return 0;
        }

        auto swapChainSupport = query_swap_chain_support(device, surface);

        if (swapChainSupport.formats.empty() || swapChainSupport.presentModes.empty()) {
            Debug::print("\tRequired swapchain not available\n");
            return 0;
        }

        size_t score = 0;

        if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
            Debug::print("\tDiscrete\n");
            score += 1000;
        }

        if (deviceFeatures.multiViewport) {
            Debug::print("\tMultiViewport\n");
            score += 100;
        }

        score += deviceProperties.limits.maxImageDimension1D;
        Debug::print("\t1D dimensions: {:L}\n", deviceProperties.limits.maxImageDimension1D);

        score += deviceProperties.limits.maxImageDimension2D;
        Debug::print("\t2D dimensions: {:L}\n", deviceProperties.limits.maxImageDimension2D);

        score += deviceProperties.limits.maxImageDimension3D;
        Debug::print("\t3D dimensions: {:L}\n", deviceProperties.limits.maxImageDimension3D);

        score += (static_cast<size_t>(deviceProperties.limits.maxComputeWorkGroupCount[0]) + static_cast<size_t>(deviceProperties.limits.maxComputeWorkGroupCount[1]) + static_cast<size_t>(deviceProperties.limits.maxComputeWorkGroupCount[2])) / (1 << 12);
        Debug::print("\tCompute group count: [ {:L} | {:L} | {:L} ] / {}\n", deviceProperties.limits.maxComputeWorkGroupCount[0], deviceProperties.limits.maxComputeWorkGroupCount[1], deviceProperties.limits.maxComputeWorkGroupCount[2], (1 << 12));




        Debug::print("\tfinal score: {:L}\n\n", score);

        return score;
    }

VkPhysicalDevice Device::create_physical_device(const VkInstance &instance, const VkSurfaceKHR &surface, const std::vector<const char *> &deviceExtensions) {
	VkPhysicalDevice physicalDevice;

	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

	Debug::assert_error(deviceCount == 0, "Failed to find supported GPUs");

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

	std::multimap<size_t, VkPhysicalDevice> candidates;

	for (const auto& device : devices) {
		candidates.insert(std::make_pair(rate_device_suitability(device, surface, deviceExtensions), device));
	}

	for (auto & candidate : candidates) {
		Debug::print("\t{:L}\t", candidate.first);
	}

	if (candidates.rbegin()->first > 0) {
		physicalDevice = candidates.rbegin()->second;
		Debug::message("Used gpu with score {:L}", candidates.rbegin()->first);
	}
	else {
		Debug::error("Failed to find a suitable GPU");
	}

	return physicalDevice;
}

VkDevice Device::create_logical_device(const VkInstance &instance, const VkSurfaceKHR &surface, const std::vector<const char *> &deviceExtensions, const VkPhysicalDevice& physicalDevice) {
	VkDevice logicalDevice;

	auto indices = find_queue_families(physicalDevice, surface);

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set<uint32_t> uniqueQueueFamilies = {indices.graphicsAndComputeFamily.value(), indices.presentFamily.value()};

	float queuePriority = 1.0f;
	for (auto queueFamily : uniqueQueueFamilies) {
		VkDeviceQueueCreateInfo queueCreateInfo{};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;
		queueCreateInfos.push_back(queueCreateInfo);
	}

	VkPhysicalDeviceFeatures deviceFeatures{};

	VkDeviceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.queueCreateInfoCount = queueCreateInfos.size();
	createInfo.pQueueCreateInfos = queueCreateInfos.data();

	createInfo.pEnabledFeatures = &deviceFeatures;

	createInfo.enabledExtensionCount = deviceExtensions.size();
	createInfo.ppEnabledExtensionNames = deviceExtensions.data();

	if (enableValidationLayers) {
		createInfo.enabledLayerCount = validationLayers.size();
		createInfo.ppEnabledLayerNames = validationLayers.data();
	}
	else {
		createInfo.enabledLayerCount = 0;
	}

	Debug::assert_error(vkCreateDevice(physicalDevice, &createInfo, nullptr, &logicalDevice) != VK_SUCCESS, "Failed to create logical device");

	VkPhysicalDeviceProperties properties;
	vkGetPhysicalDeviceProperties(physicalDevice, &properties);

	Debug::message("Created device {} on {}", fmt::ptr(logicalDevice), properties.deviceName);

	return logicalDevice;
}

void Device::init(const VkInstance &instance, const VkSurfaceKHR &surface, const std::vector<const char *> &deviceExtensions) {
	physicalDevice = create_physical_device(instance, surface, deviceExtensions);
	logicalDevice = create_logical_device(instance, surface, deviceExtensions, physicalDevice);

	auto indices = find_queue_families(physicalDevice, surface);

	vkGetDeviceQueue(logicalDevice, indices.graphicsAndComputeFamily.value(), 0, &graphicsQueue);
	vkGetDeviceQueue(logicalDevice, indices.graphicsAndComputeFamily.value(), 0, &computeQueue);
	vkGetDeviceQueue(logicalDevice, indices.presentFamily.value(), 0, &presentQueue);
}

Device::Device(const VkInstance &instance, const VkSurfaceKHR &surface, const std::vector<const char *> &deviceExtensions)
: physicalDevice(create_physical_device(instance, surface, deviceExtensions)), logicalDevice(create_logical_device(instance, surface, deviceExtensions, physicalDevice)) {
	// physicalDevice = create_physical_device(instance, surface, deviceExtensions);
	// logicalDevice = create_logical_device(instance, surface, deviceExtensions, physicalDevice);

	auto indices = find_queue_families(physicalDevice, surface);

	vkGetDeviceQueue(logicalDevice, indices.graphicsAndComputeFamily.value(), 0, &graphicsQueue);
	vkGetDeviceQueue(logicalDevice, indices.graphicsAndComputeFamily.value(), 0, &computeQueue);
	vkGetDeviceQueue(logicalDevice, indices.presentFamily.value(), 0, &presentQueue);
}

Device::~Device() {
	if (logicalDevice != VK_NULL_HANDLE) vkDestroyDevice(logicalDevice, nullptr);
}

Device::operator VkPhysicalDevice() const {
	return physicalDevice;
}

Device::operator VkDevice() const {
	return logicalDevice;
}

