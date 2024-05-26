#include "Window.h"

#define VK_USE_PLATFORM_WIN32_KHR
#include <SDL.h>
#include <SDL_vulkan.h>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include <vector>
#include <map>
#include <set>

#include <Util/Debug.h>

namespace OddityEngine::Graphics::Vulkan {
    VkResult create_debug_utils_messenger_EXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
        auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
        if (func != nullptr) {
            return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
        }
        else {
            return VK_ERROR_EXTENSION_NOT_PRESENT;
        }
    }

    void destroy_debug_utils_messenger_EXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
        auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
        if (func != nullptr) {
            func(instance, debugMessenger, pAllocator);
        }
    }

    void populate_debug_messenger_create_info(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
        createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        createInfo.pfnUserCallback = Window::debug_callback;
        createInfo.pUserData = nullptr;
    }

    QueueFamilyIndices Window::find_queue_families(VkPhysicalDevice device) {
        QueueFamilyIndices indices;

        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

        for (int i = 0; i < queueFamilyCount; i++) {
            if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                indices.graphicsFamily = i;
            }

            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);

            if (presentSupport) {
                indices.presentFamily = i;
            }

            if (indices.is_complete()) {
                break;
            }
        }

        return indices;
    }

    bool Window::check_device_extension_support(VkPhysicalDevice device) {
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

    bool is_device_suitable(VkPhysicalDevice device) {
        VkPhysicalDeviceProperties deviceProperties;
        vkGetPhysicalDeviceProperties(device, &deviceProperties);

        VkPhysicalDeviceFeatures deviceFeatures;
        vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

        return deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU && deviceFeatures.geometryShader;
    }

    size_t Window::rate_device_suitability(VkPhysicalDevice device) {
        VkPhysicalDeviceProperties deviceProperties;
        vkGetPhysicalDeviceProperties(device, &deviceProperties);

        VkPhysicalDeviceFeatures deviceFeatures;
        vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

        Debug::message("Device {} :", deviceProperties.deviceName);

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

        score += deviceProperties.limits.maxComputeSharedMemorySize;
        Debug::print("\tCompute shader memory: {:L}\n", deviceProperties.limits.maxComputeSharedMemorySize);

        score += (static_cast<size_t>(deviceProperties.limits.maxComputeWorkGroupCount[0]) + static_cast<size_t>(deviceProperties.limits.maxComputeWorkGroupCount[1]) + static_cast<size_t>(deviceProperties.limits.maxComputeWorkGroupCount[2])) / (1 << 12);
        Debug::print("\tCompute group count: [ {:L} | {:L} | {:L} ] / {}\n", deviceProperties.limits.maxComputeWorkGroupCount[0], deviceProperties.limits.maxComputeWorkGroupCount[1], deviceProperties.limits.maxComputeWorkGroupCount[2], (1 << 12));

        auto indices = find_queue_families(device);

        if (!(deviceFeatures.geometryShader && indices.is_complete() && check_device_extension_support(device))) {
            Debug::print("\tRequired features not available\n");
            score =  0;
        }

        Debug::print("\tfinal score: {:L}\n\n", score);

        return score;
    }

    bool Window::check_validation_layer_support() {
        uint32_t layerCount = 0;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        for (auto layerName : validationLayers) {
            bool layerFound = false;

            for (auto layerProperties : availableLayers) {
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

    SwapChainSupportDetails Window::query_swap_chain_support(VkPhysicalDevice device) {
        SwapChainSupportDetails details;

        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);



        return details;
    }

    std::vector<const char *> Window::get_required_extensions() {
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
        Debug::assert_error(SDL_Vulkan_GetInstanceExtensions(window, &extensionCount, nullptr) != SDL_TRUE, "Failed to get required extensions");

        std::vector<const char*> extensions(extensionCount);
        Debug::assert_error(SDL_Vulkan_GetInstanceExtensions(window, &extensionCount, extensions.data()) != SDL_TRUE, "Failed to get required extensions");

        if (enableValidataionLayers) {
            extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }

        extensionCount = extensions.size();
        Debug::message("Found {} required extensions:", extensionCount);
        for (size_t i = 0; i < extensionCount; i++) {
            Debug::print("\t{}\t{}\n", i, extensions[i]);
        }

        return extensions;
    }

    void Window::create_instance(const std::string& name) {
        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = name.c_str();
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

        Debug::assert_error(enableValidataionLayers && !check_validation_layer_support(), "Validation Layers not available");

        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
        if (enableValidataionLayers) {
            createInfo.enabledLayerCount = validationLayers.size();
            createInfo.ppEnabledLayerNames = validationLayers.data();

            populate_debug_messenger_create_info(debugCreateInfo);
            createInfo.pNext = &debugCreateInfo;
        }
        else {
            createInfo.enabledLayerCount = 0;
            createInfo.pNext = nullptr;
        }

        Debug::assert_error(vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS, "Failed to create vulkan Instance");
    }

    void Window::setup_debug_messenger() {
        if (!enableValidataionLayers) return;

        VkDebugUtilsMessengerCreateInfoEXT createInfo{};
        populate_debug_messenger_create_info(createInfo);

        Debug::assert_error(create_debug_utils_messenger_EXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS, "Failed to set up debug messenger");
    }

    void Window::create_surface() {
        Debug::assert_error(SDL_Vulkan_CreateSurface(window, instance, &surface) != SDL_TRUE, "Failed to create surface");
    }

    void Window::pick_physical_device() {
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

        Debug::assert_error(deviceCount == 0, "Failed to find supported GPUs");

        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

        std::multimap<size_t, VkPhysicalDevice> candidates;

        for (const auto& device : devices) {
            candidates.insert(std::make_pair(rate_device_suitability(device), device));
        }

        if (candidates.rbegin()->first > 0) {
            physicalDevice = candidates.rbegin()->second;
        }
        else {
            Debug::error("Failed to find a suitable GPU");
        }
    }

    void Window::create_logical_device() {
        auto indices = find_queue_families(physicalDevice);

        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        std::set<uint32_t> uniqueQueueFamilies = {indices.graphicsFamily.value(), indices.presentFamily.value()};

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

        if (enableValidataionLayers) {
            createInfo.enabledLayerCount = validationLayers.size();
            createInfo.ppEnabledLayerNames = validationLayers.data();
        }
        else {
            createInfo.enabledLayerCount = 0;
        }

        Debug::assert_error(vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS, "Failed to create logical device");

        vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);
        vkGetDeviceQueue(device, indices.presentFamily.value(), 0, &presentQueue);
    }




    Window::Window(const char* name, const int width, const int height, const unsigned int flags) : Graphics::Window(name, width, height, flags | SDL_WINDOW_VULKAN) {
        create_instance(name);
        setup_debug_messenger();
        create_surface();
        pick_physical_device();
        create_logical_device();
    }

    Window::~Window() {
        vkDestroyDevice(device, nullptr);

        if (enableValidataionLayers) {
            destroy_debug_utils_messenger_EXT(instance, debugMessenger, nullptr);
        }

        vkDestroySurfaceKHR(instance, surface, nullptr);
        vkDestroyInstance(instance, nullptr);

        Graphics::Window::~Window();
    }

    void Window::update() {

    }

    void Window::make_current() {

    }

    VkBool32 Window::debug_callback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserData) {
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