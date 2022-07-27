#include <vector>


#include "graphics.h"
#include "checks.cpp"
#include <stdexcept>


const std::vector<const char*> validationLayers = {
	"VK_LAYER_KHRONOS_validation"
};


class Vulkan {
	friend class Graphics;
	void initVulkan() {
		createInstance();
	}

private:
	void createInstance() {
		if (VALIDATE && !validationLayerSupport()) {
			throw std::runtime_error("Validation Layers requested, but not supported");
		}

		VkApplicationInfo appInfo {
			.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
			.pApplicationName = "Triangle",
			.applicationVersion = VK_MAKE_VERSION(1, 0, 0),
			.pEngineName = "OddityEngine",
			.engineVersion = VK_MAKE_VERSION(1, 0, 0),
			.apiVersion = VK_API_VERSION_1_0
		};

		VkInstanceCreateInfo createInfo{
			.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
			.pApplicationInfo = &appInfo
		};

		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;

		auto extensions = getRequiredExtensions();

		createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
		createInfo.
	}

	bool validationLayerSupport() {
		uint32_t layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

		std::vector<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

		bool layerFound = false;
		for (const char* layerName : validationLayers) {

			for (const auto& layerProperties : availableLayers) {
				if (strcmp(layerName, layerProperties.layerName) == 0) {
					layerFound = true;
					break;
				}
			}
		}

		return layerFound;
	}

	std::vector<const char*> getRequiredExtensions() {
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;

		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

		if (VALIDATE) {
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}

		return extensions;
	}
};

