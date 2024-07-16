#include "Vulkan.h"

#include <vulkan/vulkan.h>
#include <SDL_vulkan.h>

#include <Util/Debug.h>

namespace OddityEngine {
	std::vector<const char *> Vulkan::get_required_extensions() {
		uint32_t availableExtensionCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &availableExtensionCount, nullptr);

		std::vector<VkExtensionProperties> availableExtensions(availableExtensionCount);
		vkEnumerateInstanceExtensionProperties(nullptr, &availableExtensionCount, availableExtensions.data());

		availableExtensionCount = availableExtensions.size();
		Debug::message("Found {} available Vulkan extensions:", availableExtensionCount);
		for (size_t i = 0; i < availableExtensionCount; i++) {
			Debug::print("\t{}\t{}\n", i, availableExtensions[i].extensionName);
		}

		unsigned int extensionCount = 0;
		Debug::assert_error(SDL_Vulkan_GetInstanceExtensions(*_window, &extensionCount, nullptr) != SDL_TRUE, "Failed to get required extensions");

		std::vector<const char *> extensions(extensionCount);
		Debug::assert_error(SDL_Vulkan_GetInstanceExtensions(*_window, &extensionCount, extensions.data()) != SDL_TRUE, "Failed to get required extensions");

		if (enableValidationLayers) {
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}

		extensionCount = extensions.size();
		Debug::message("Found {} required extensions:", extensionCount);
		for (size_t i = 0; i < extensionCount; i++) {
			Debug::print("\t{}\t{}\n", i, extensions[i]);
		}

		return extensions;
	}

	bool Vulkan::check_validation_layer_support() const {
		uint32_t layerCount = 0;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

		std::vector<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

		for (const auto layerName: validationLayers) {
			bool layerFound = false;

			for (const auto layerProperties: availableLayers) {
				if (strcmp(layerName, layerProperties.layerName) == 0) {
					layerFound = true;
					break;
				}
			}

			if (!layerFound) {
				return false;
			}
		}

		return true;
	}

	void populate_debug_messenger_create_info(VkDebugUtilsMessengerCreateInfoEXT &createInfo) {
		createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		createInfo.pfnUserCallback = Vulkan::debug_callback;
		createInfo.pUserData = nullptr;
	}

	VkResult create_debug_utils_messenger_EXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkDebugUtilsMessengerEXT *pDebugMessenger) {
		auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
		if (func != nullptr) {
			return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
		} else {
			return VK_ERROR_EXTENSION_NOT_PRESENT;
		}
	}

	void destroy_debug_utils_messenger_EXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks *pAllocator) {
		auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
		if (func != nullptr) {
			func(instance, debugMessenger, pAllocator);
		}
	}

	void Vulkan::create_instance() {
		auto title = SDL_GetWindowTitle(*_window);
		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = title;
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "OddityEngine";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_3;

		VkInstanceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;

		auto extensions = get_required_extensions();

		createInfo.enabledExtensionCount = extensions.size();
		createInfo.ppEnabledExtensionNames = extensions.data();

		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};

		if (enableValidationLayers) {
			Debug::assert_error(!check_validation_layer_support(), "Validation Layers not available");
			createInfo.enabledLayerCount = validationLayers.size();
			createInfo.ppEnabledLayerNames = validationLayers.data();

			populate_debug_messenger_create_info(debugCreateInfo);
			createInfo.pNext = &debugCreateInfo;
		} else {
			createInfo.enabledLayerCount = 0;
			createInfo.pNext = nullptr;
		}


		Debug::assert_error(vkCreateInstance(&createInfo, nullptr, &_instance) != VK_SUCCESS, "Failed to create vulkan Instance");
	}

	void Vulkan::setup_debug_messenger() {
		if (enableValidationLayers) {
			VkDebugUtilsMessengerCreateInfoEXT createInfo{};
			populate_debug_messenger_create_info(createInfo);


			Debug::assert_error(create_debug_utils_messenger_EXT(_instance, &createInfo, nullptr, &_debugMessenger) != VK_SUCCESS, "Failed to set up debug messenger");
		}
	}

	void Vulkan::create_surface() {
		Debug::assert_error(SDL_Vulkan_CreateSurface(*_window, _instance, &_surface) != SDL_TRUE, "Failed to create sdl vulkan surface");
	}

	Vulkan::Vulkan(Window *window) : _window(window) {
		_window->add_subdateable(this);

		set_active(false);
		create_instance();
		setup_debug_messenger();
		create_surface();

		_device = Device(_instance, _surface, _deviceExtensions);
		_swapChain = SwapChain(_device.physicalDevice, _device.device, _surface, _window->get_size());

		Debug::message("Vulkan initilized");
	}

	Vulkan::~Vulkan() {
		_swapChain = SwapChain();
		_device = Device();

		if (enableValidationLayers) {
			destroy_debug_utils_messenger_EXT(_instance, _debugMessenger, nullptr);
		}

		vkDestroySurfaceKHR(_instance, _surface, nullptr);
		vkDestroyInstance(_instance, nullptr);
	}

	void Vulkan::update() {
	}

	VkBool32 Vulkan::debug_callback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserData) {
		//        if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
		//            Debug::error("Vulkan error:\n{}", pCallbackData->pMessage);
		//        }
		//        else if (messageSeverity >= minSeverity) {
		//            Debug::message("Vulkan info:\n{}", pCallbackData->pMessage);
		//        }

		Debug::message("{}", pCallbackData->pMessage);

		return VK_FALSE;
	}
}
