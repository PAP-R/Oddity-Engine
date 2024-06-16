#ifndef ODDITYENGINE_WINDOW_VULKAN_H
#define ODDITYENGINE_WINDOW_VULKAN_H

#include <SDL.h>
#include <SDL_vulkan.h>
#include <vulkan/vulkan.h>

#include <string>
#include <vector>
#include <array>
#include <optional>

#include <Graphics/Window.h>
#include <Graphics/Camera.h>

#include "Buffer.h"

#define minSeverity VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT

const int MAX_FRAMES_IN_FLIGHT = 2;

namespace OddityEngine::Graphics::Vulkan {
    struct Vertex {
        glm::vec2 pos;
        glm::vec3 dir;

        static VkVertexInputBindingDescription get_binding_description() {
            VkVertexInputBindingDescription bindingDescription{};
            bindingDescription.binding = 0;
            bindingDescription.stride = sizeof(Vertex);
            bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

            return bindingDescription;
        }

        static std::array<VkVertexInputAttributeDescription, 2> get_attribute_descriptions() {
            std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};
            attributeDescriptions[0].binding = 0;
            attributeDescriptions[0].location = 0;
            attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
            attributeDescriptions[0].offset = offsetof(Vertex, pos);
            attributeDescriptions[1].binding = 0;
            attributeDescriptions[1].location = 1;
            attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
            attributeDescriptions[1].offset = offsetof(Vertex, dir);

            return attributeDescriptions;
        }
    };

    struct UniformBufferObject {
        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 proj;
        glm::vec2 screenSize;
        glm::vec2 mousePos;
        glm::vec4 cameraPos;
        alignas(16) float time;
    };

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
        std::vector<const char*> validationLayers = {
            "VK_LAYER_KHRONOS_validation"
        };

        std::vector<const char*> deviceExtensions = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };

        uint32_t currentFrame = 0;

        VkInstance instance;
        VkDebugUtilsMessengerEXT debugMessenger;
        VkSurfaceKHR surface;
        VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
        VkDevice device;
        VkQueue graphicsQueue;
        VkQueue presentQueue;
        VkSwapchainKHR swapChain;

        std::vector<VkImage> swapChainImages;
        std::vector<VkImageView> swapChainImageViews;
        VkFormat swapChainFormat;
        VkExtent2D swapChainExtent;

        std::vector<VkFramebuffer> swapChainFramebuffers;

        VkRenderPass renderPass;

        VkDescriptorSetLayout descriptorSetLayout;
        VkPipelineLayout pipelineLayout;

        VkPipeline graphicsPipeline;

        VkCommandPool commandPool;
        std::vector<VkCommandBuffer> commandBuffers;

        std::vector<VkSemaphore> imageAvailableSemaphores;
        std::vector<VkSemaphore> renderFinishedSemaphores;
        std::vector<VkFence> inFlightFences;

        Buffer<Vertex> vertexBuffer;
        Buffer<uint32_t> indexBuffer;

        std::vector<Buffer<UniformBufferObject>> uniformBuffers;

        VkDescriptorPool descriptorPool;
        std::vector<VkDescriptorSet> descriptorSets;

        bool framebufferResized = false;
        bool minimized = false;

        const std::vector<Vertex> vertices = {
            {{-1, -1}, {-1, 1, 1}},
            {{1, -1}, {1, 1, 1}},
            {{1, 1}, {1, -1, 1}},
            {{-1, 1}, {-1, -1, 1}}
        };

        const std::vector<uint32_t> indices = {
            0, 1, 2,
            2, 3, 0
        };

        Camera camera = Camera();

#ifdef NDEBUG
        const bool enableValidataionLayers = false;
#else
        const bool enableValidataionLayers = true;
#endif

        std::vector<const char*> get_required_extensions();

        QueueFamilyIndices find_queue_families(VkPhysicalDevice device);
        bool check_device_extension_support(VkPhysicalDevice device);
        size_t rate_device_suitability(VkPhysicalDevice device);

        bool check_validation_layer_support();
        SwapChainSupportDetails query_swap_chain_support(VkPhysicalDevice device);
        VkSurfaceFormatKHR choose_swap_surface_format(const std::vector<VkSurfaceFormatKHR>& availableFomrats);
        VkPresentModeKHR choose_swap_present_mode(const std::vector<VkPresentModeKHR>& availablePresentModes);
        VkExtent2D choose_swap_extent(const VkSurfaceCapabilitiesKHR& capabilities);

        VkShaderModule create_shader_module(const std::string& path);

        uint32_t find_memory_type(uint32_t typeFilter, VkMemoryPropertyFlags properties);

        void create_instance(const std::string& name);
        void setup_debug_messenger();
        void create_surface();

        void pick_physical_device();
        void create_logical_device();

        void create_swap_chain();

        void create_image_views();
        void create_render_pass();
        void create_descriptor_set_layout();
        void create_graphics_pipeline();
        void create_framebuffers();

        void create_command_pool();

        void create_vertex_buffer();
        void create_index_buffer();
        void create_uniform_buffers();

        void create_descriptor_pool();
        void create_descriptor_sets();

        void create_command_buffer();
        void create_sync_objects();

        void update_uniform_buffer(uint32_t currentImage);
        void record_command_buffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
        void draw_frame();

        void cleanup_swap_chain();
        void recreate_swap_chain();

    public:
        Window(const char* name, int width, int height, unsigned int flags);
        ~Window();

        void update() override;
        void make_current() override;

        void set_size(glm::ivec2 size) override;

        static VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);
    };
}

#endif //ODDITYENGINE_WINDOW_VULKAN_H
