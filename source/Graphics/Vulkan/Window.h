#ifndef ODDITYENGINE_WINDOW_VULKAN_H
#define ODDITYENGINE_WINDOW_VULKAN_H

#include <SDL.h>
#include <SDL_vulkan.h>
#include <vulkan/vulkan.h>

#include <string>
#include <vector>
#include <optional>

#include <Graphics/Window.h>

#define minSeverity VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT

namespace OddityEngine::Graphics::Vulkan {
    struct QueueFamilyIndices {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;

        bool is_complete() {
            return graphicsFamily.has_value() && presentFamily.has_value();
        }
    };

    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    class Window : public Graphics::Window {
        VkInstance instance;
        VkDebugUtilsMessengerEXT debugMessenger;
        VkSurfaceKHR surface;
        VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
        VkDevice device;
        VkQueue graphicsQueue;
        VkQueue presentQueue;

        std::vector<const char*> validationLayers = {
            "VK_LAYER_KHRONOS_validation"
        };

        std::vector<const char*> deviceExtensions = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };



#ifdef NDEBUG
        const bool enableValidataionLayers = false;
#else
        const bool enableValidataionLayers = true;
#endif

        QueueFamilyIndices find_queue_families(VkPhysicalDevice device);
        bool check_device_extension_support(VkPhysicalDevice device);
        size_t rate_device_suitability(VkPhysicalDevice device);


        bool check_validation_layer_support();
        SwapChainSupportDetails query_swap_chain_support(VkPhysicalDevice device);

        std::vector<const char*> get_required_extensions();
        void create_instance(const std::string& name);
        void setup_debug_messenger();
        void create_surface();
        void pick_physical_device();
        void create_logical_device();

    public:
        Window(const char* name, int width, int height, unsigned int flags);
        ~Window();

        void update() override;
        void make_current() override;

        static VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);
    };
}

#endif //ODDITYENGINE_WINDOW_VULKAN_H
