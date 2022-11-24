#pragma once

#ifndef ODDITYENGINECMAKE_VULKAN_H
#define ODDITYENGINECMAKE_VULKAN_H

#include <vector>
#include <map>
#include <set>

#include <stdexcept>
#include <iostream>
#include <chrono>

#include "../checks/checks.h"
#include "../character/movement.h"
#include "graphic_libs.h"

#include "vulkan_helper.h"


#define MAX_FRAMES_IN_FLIGHT 2

namespace Graphics {
    struct UniformBufferObject {
        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 proj;
    };

    const std::vector<const char*> validationLayers = {
            "VK_LAYER_KHRONOS_validation"
    };


    class Vulkan {
    private:
        GLFWwindow* window;

        VkInstance instance;
        VkDebugUtilsMessengerEXT debugMessenger;
        VkSurfaceKHR surface;

        VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
        VkDevice device;

        VkQueue graphicsQueue;
        VkQueue presentQueue;

        //Swapchain
        VkSwapchainKHR swapchain;
        VkFormat swapchainImageFormat;
        VkExtent2D swapchainExtent;

        std::vector<VkImage> swapchainImages;

        std::vector<VkImageView> swapchainImageViews;


        //Render
        VkRenderPass renderPass;

        VkDescriptorSetLayout descriptorSetLayout;
        VkPipelineLayout pipelineLayout;

        VkPipeline graphicsPipeline;

        std::vector<VkFramebuffer> swapchainFramebuffers;


        //Pools
        VkCommandPool commandPool;
        VkDescriptorPool descriptorPool;

        std::vector<VkDescriptorSet> descriptorSets;


        //Buffer
        size_t vertexBlockSize = 1024;
        size_t vertexSize = vertexBlockSize;
        size_t vertexOffset = 0;
        VkBuffer vertexBuffer = VK_NULL_HANDLE;
        VkDeviceMemory vertexBufferMemory;
        size_t indexBlockSize = 1024;
        size_t indexSize = indexBlockSize;
        size_t indexCount = 0;
        size_t indexOffset = 0;
        VkBuffer indexBuffer = VK_NULL_HANDLE;
        VkDeviceMemory indexBufferMemory;

        std::vector<VkBuffer> uniformBuffers;
        std::vector<VkDeviceMemory> uniformBuffersMemory;
        std::vector<VkCommandBuffer> commandBuffers;


        //Semaphores
        std::vector<VkSemaphore> imageAvailableSemaphores;
        std::vector<VkSemaphore> renderFinishedSemaphores;
        std::vector<VkFence> inFlightFences;
        std::vector<VkFence> imagesInFlight;

        //Vertices
        std::vector<Vertex> baseVertices;

        std::vector<uint32_t> baseIndices;

        Movement movement;

        size_t currentFrame = 0;

        //VmaAllocator allocator;

        //Images
        VkImage textureImage;
        VkDeviceMemory textureImageMemory;
        VkImageView textureImageView;

        VkImage depthImage;
        VkDeviceMemory depthImageMemory;
        VkImageView depthImageView;
        VkSampler textureSampler;

        VkClearValue clearColor = { {{0.2f, 0.2f, 0.2f, 1.0f}} };

    public:
        Vulkan();

        void start(GLFWwindow* window);

        void update(bool *framebufferResized, Movement movement);

        void stop();

        void add(std::vector<Vertex> vertices, std::vector<uint32_t> indices);

        void addVertex(std::vector<Vertex> vertices);

        void readVertex();

        void addIndex(std::vector<uint32_t> indices);


    private:
        //Creation
        void createInstance();

        void createSurface();

        void createLogicalDevice(VkPhysicalDevice physicalDevice);
        /*
        void createVmaAllocator() {
            VmaVulkanFunctions vulkanFunctions = {
                .vkGetInstanceProcAddr = &vkGetInstanceProcAddr,
                .vkGetDeviceProcAddr = &vkGetDeviceProcAddr
            };

            VmaAllocatorCreateInfo createInfo = {
                .physicalDevice = physicalDevice,
                .device = device,
                .pVulkanFunctions = &vulkanFunctions,
                .instance = instance,
                .vulkanApiVersion = VK_API_VERSION_1_3,
            };

            vmaCreateAllocator(&createInfo, &allocator);
        }
        */
        void createSwapChain();

        void createImageViews();

        void createRenderPass();

        void createDescriptorSetLayout();

        void createGraphicsPipeline();

        void createFrameBuffer();

        void createCommandPool();

        void createDepthResources();

        void createTextureImage();

        void createTextureImageView();

        void createTextureSampler();

        void createVertexBuffer(VkDeviceSize size, VkDeviceSize prevSize = 0);

        void createIndexBuffer(VkDeviceSize size, VkDeviceSize prevSize = 0);

        void createUniformBuffers();

        void createDescriptorPool();

        void createDiscriptorSets();

        void createCommandBuffers();

        void updateCommandBuffer();

        void createSyncObjects();

        //Debug
        bool validationLayerSupport();

        void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

        static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                            VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                            const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                                            void* pUserData);

        VkResult vkCreateDebugUtilsMessengerEXT(VkInstance instance,
                                                const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
                                                const VkAllocationCallbacks* pAllocator,
                                                VkDebugUtilsMessengerEXT* pDebugMessenger);


        void setupDebugMessenger();

        //MAIN
        void drawFrame(bool *framebufferResized);

        void updateUniformBuffer(uint32_t currentImage);

        void cleanup();

        //Swapchain
        void cleanupSwapChain();

        void recreateSwapChain();
    };
}
#endif //ODDITYENGINECMAKE_VULKAN_H
