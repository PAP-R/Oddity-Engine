#pragma once

#include <vector>
#include <map>
#include <set>

#include <stdexcept>
#include <iostream>
#include <chrono>


#include "graphics.h"
#include "../checks/checks.h"

#include "vulkan_helper.h"


namespace Graphics {
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
		VkBuffer vertexBuffer;
		VkDeviceMemory vertexBufferMemory;
		VkBuffer indexBuffer;
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
		std::vector<Vertex> vertices = {
			{{0.0f, 0.0f, 0.5f}, {0.5f, 0.5f, 0.5f}},
			{{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}},
			{{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}},
			{{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}},
			{{-0.5f, 0.5f, 0.0f}, {0.5f, 0.5f, 0.5f}}
		};

		std::vector<uint32_t> indices = {
			0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 4, 1
		};


		size_t currentFrame = 0;


	public:
		void start(GLFWwindow* window) {
			this->window = window;

			createInstance();
			setupDebugMessenger();
			createSurface();
			physicalDevice = pickPhysicalDevice(instance, surface);
			createLogicalDevice(physicalDevice);
			createSwapChain();
			createImageViews();
			createRenderPass();
			createDescriptorSetLayout();
			createGraphicsPipeline();
			createFrameBuffer();
			createCommandPool();
			//createTextureImage();
			createVertexBuffer();
			createIndexBuffer();
			createUniformBuffers();
			createDescriptorPool();
			createDiscriptorSets();
			createCommandBuffers();
			createSyncObjects();
		}

		void update(bool *framebufferResized) {
			drawFrame(framebufferResized);
		}

		void stop() {
			cleanup();
		}

	private:
		//Creation
		void createInstance() {
			check(VALIDATE && !validationLayerSupport(), "Validation Layers requested, but not supported");

			VkApplicationInfo appInfo{
				.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
				.pApplicationName = "Triangle",
				.applicationVersion = VK_MAKE_VERSION(1, 0, 0),
				.pEngineName = "OddityEngine",
				.engineVersion = VK_MAKE_VERSION(1, 0, 0),
				.apiVersion = VK_API_VERSION_1_0
			};


			uint32_t glfwExtensionCount = 0;
			const char** glfwExtensions;

			auto extensions = getRequiredExtensions();

			VkInstanceCreateInfo createInfo{
				.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
				.pApplicationInfo = &appInfo,
				.enabledLayerCount = VALIDATE ? static_cast<uint32_t>(validationLayers.size()) : 0,
				.ppEnabledLayerNames = VALIDATE ? validationLayers.data() : nullptr,
				.enabledExtensionCount = static_cast<uint32_t>(extensions.size()),
				.ppEnabledExtensionNames = extensions.data()
			};

			if (VALIDATE) {
				VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
				populateDebugMessengerCreateInfo(debugCreateInfo);
				createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
			}

			check(vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS, "Failed to create Instance");
		}

		void createSurface() {
			check(glfwCreateWindowSurface(instance, window, nullptr, &surface), "Surface creation failed");
		}

		void createLogicalDevice(VkPhysicalDevice physicalDevice) {
			QueueFamilyIndices indices = findQueueFamilies(physicalDevice, surface);

			std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
			std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };

			float queuePriority = 1.0f;
			for (uint32_t queueFamily : uniqueQueueFamilies) {
				VkDeviceQueueCreateInfo queueCreateInfo{
					.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
					.queueFamilyIndex = queueFamily,
					.queueCount = 1,
					.pQueuePriorities = &queuePriority,
				};
				queueCreateInfos.push_back(queueCreateInfo);
			}

			VkPhysicalDeviceFeatures deviceFeatures{};

			VkDeviceCreateInfo createInfo{
				.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
				.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size()),
				.pQueueCreateInfos = queueCreateInfos.data(),

				.enabledLayerCount = VALIDATE ? static_cast<uint32_t>(validationLayers.size()) : 0,
				.ppEnabledLayerNames = VALIDATE ? validationLayers.data() : nullptr,

				.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size()),
				.ppEnabledExtensionNames = deviceExtensions.data(),

				.pEnabledFeatures = &deviceFeatures,
			};

			check(vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS, "Failed to create logical device");

			vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);
			vkGetDeviceQueue(device, indices.presentFamily.value(), 0, &presentQueue);
		}

		void createSwapChain() {
			SwapChainSupportDetails swapChainSupport = querySwapChainSupport(physicalDevice, surface);
			VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
			VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
			VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities, window);

			uint32_t imageCount = swapChainSupport.capabilities.maxImageCount > 0 && swapChainSupport.capabilities.minImageCount + 1 > swapChainSupport.capabilities.maxImageCount
				? swapChainSupport.capabilities.maxImageCount : swapChainSupport.capabilities.minImageCount + 1;

			QueueFamilyIndices indices = findQueueFamilies(physicalDevice, surface);
			bool sameFamily = indices.graphicsFamily != indices.presentFamily;
			uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

			VkSwapchainCreateInfoKHR createInfo{
				.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
				.surface = surface,

				.minImageCount = imageCount,
				.imageFormat = surfaceFormat.format,
				.imageColorSpace = surfaceFormat.colorSpace,
				.imageExtent = extent,
				.imageArrayLayers = 1,
				.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,

				.imageSharingMode = sameFamily ? VK_SHARING_MODE_CONCURRENT : VK_SHARING_MODE_EXCLUSIVE,
				.queueFamilyIndexCount = static_cast<uint32_t>(sameFamily ? 2 : 0),
				.pQueueFamilyIndices = sameFamily ? queueFamilyIndices : nullptr,

				.preTransform = swapChainSupport.capabilities.currentTransform,
				.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
				.presentMode = presentMode,
				.clipped = VK_TRUE,

				.oldSwapchain = VK_NULL_HANDLE
			};

			check(vkCreateSwapchainKHR(device, &createInfo, nullptr, &swapchain) != VK_SUCCESS, "Failed to create swapchain");

			swapchainImageFormat = surfaceFormat.format;
			swapchainExtent = extent;

			vkGetSwapchainImagesKHR(device, swapchain, &imageCount, nullptr);
			swapchainImages.resize(imageCount);
			vkGetSwapchainImagesKHR(device, swapchain, &imageCount, swapchainImages.data());
		}

		void createImageViews() {
			swapchainImageViews.resize(swapchainImages.size());
			for (int i = 0; i < swapchainImages.size(); i++) {
				VkImageViewCreateInfo createInfo{
					.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
					.image = swapchainImages[i],
					.viewType = VK_IMAGE_VIEW_TYPE_2D,
					.format = swapchainImageFormat,

					.components = {
						.r = VK_COMPONENT_SWIZZLE_IDENTITY,
						.g = VK_COMPONENT_SWIZZLE_IDENTITY,
						.b = VK_COMPONENT_SWIZZLE_IDENTITY,
						.a = VK_COMPONENT_SWIZZLE_IDENTITY
						},

					.subresourceRange = {
						.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
						.baseMipLevel = 0,
						.levelCount = 1,
						.baseArrayLayer = 0,
						.layerCount = 1
						}
				};

				check(vkCreateImageView(device, &createInfo, nullptr, &swapchainImageViews[i]) != VK_SUCCESS, "Failed to create Image View");
			}
		}

		void createRenderPass() {
			VkAttachmentDescription colorAttachment{
				.format = swapchainImageFormat,
				.samples = VK_SAMPLE_COUNT_1_BIT,
				.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
				.storeOp = VK_ATTACHMENT_STORE_OP_STORE,
				.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
				.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
				.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
				.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
			};

			VkAttachmentReference colorAttachmentRef{
				.attachment = 0,
				.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
			};

			VkSubpassDescription subpass{
				.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
				.colorAttachmentCount = 1,
				.pColorAttachments = &colorAttachmentRef
			};

			VkSubpassDependency dependency{
				.srcSubpass = VK_SUBPASS_EXTERNAL,
				.dstSubpass = 0,
				.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
				.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
				.srcAccessMask = 0,
				.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT
			};

			VkRenderPassCreateInfo renderPassInfo{
				.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
				.attachmentCount = 1,
				.pAttachments = &colorAttachment,
				.subpassCount = 1,
				.pSubpasses = &subpass,
				.dependencyCount = 1,
				.pDependencies = &dependency
			};

			check(vkCreateRenderPass(device, &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS, "Failed to create Render Pass");
		}

		void createDescriptorSetLayout() {
			VkDescriptorSetLayoutBinding uboLayoutBinding{
				.binding = 0,
				.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
				.descriptorCount = 1,
				.stageFlags = VK_SHADER_STAGE_VERTEX_BIT,
				.pImmutableSamplers = nullptr
			};

			VkDescriptorSetLayoutCreateInfo layoutInfo{
				.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
				.bindingCount = 1,
				.pBindings = &uboLayoutBinding
			};

			check(vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS, "Failed to create descriptor set layout");
		}

		void createGraphicsPipeline() {
			auto vertShaderCode = readFile("shaders/vert.spv");
			auto fragShaderCode = readFile("shaders/frag.spv");

			VkShaderModule vertShaderModule = createShaderModule(vertShaderCode, device);
			VkShaderModule fragShaderModule = createShaderModule(fragShaderCode, device);

			VkPipelineShaderStageCreateInfo vertShaderStageInfo{
				.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
				.stage = VK_SHADER_STAGE_VERTEX_BIT,
				.module = vertShaderModule,
				.pName = "main"
			};

			VkPipelineShaderStageCreateInfo fragShaderStageInfo{
				.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
				.stage = VK_SHADER_STAGE_FRAGMENT_BIT,
				.module = fragShaderModule,
				.pName = "main"
			};

			VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo,fragShaderStageInfo };

			auto bindingDescription = Vertex::getBindingDescription();
			auto attributeDescription = Vertex::getAttributeDestions();

			VkPipelineVertexInputStateCreateInfo vertexInputInfo{
				.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
				.vertexBindingDescriptionCount = 1,
				.pVertexBindingDescriptions = &bindingDescription,
				.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescription.size()),
				.pVertexAttributeDescriptions = attributeDescription.data()
			};

			VkPipelineInputAssemblyStateCreateInfo inputAssembly{
				.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
				.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
				.primitiveRestartEnable = VK_FALSE
			};

			VkViewport viewport{
				.x = 0.0f,
				.y = 0.0f,
				.width = (float)swapchainExtent.width,
				.height = (float)swapchainExtent.height,
				.minDepth = 0.0f,
				.maxDepth = 1.0f
			};

			VkRect2D scissor{
				.offset = { 0, 0 },
				.extent = swapchainExtent
			};

			VkPipelineViewportStateCreateInfo viewportState{
				.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
				.viewportCount = 1,
				.pViewports = &viewport,
				.scissorCount = 1,
				.pScissors = &scissor
			};


			VkPipelineRasterizationStateCreateInfo rasterizer{
				.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
				.depthClampEnable = VK_FALSE,
				.rasterizerDiscardEnable = VK_FALSE,
				.polygonMode = VK_POLYGON_MODE_FILL,
				.cullMode = VK_CULL_MODE_BACK_BIT,
				.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE,
				.depthBiasEnable = VK_FALSE,
				.depthBiasConstantFactor = 0.0f,
				.depthBiasClamp = 0.0f,
				.depthBiasSlopeFactor = 0.0f,
				.lineWidth = 1.0f
			};

			VkPipelineMultisampleStateCreateInfo multisampling{
				.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
				.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
				.sampleShadingEnable = VK_FALSE,
				.minSampleShading = 1.0f,
				.pSampleMask = nullptr,
				.alphaToCoverageEnable = VK_FALSE,
				.alphaToOneEnable = VK_FALSE
			};

			VkPipelineColorBlendAttachmentState colorBlendAttachment{
				.blendEnable = VK_FALSE,
				.srcColorBlendFactor = VK_BLEND_FACTOR_ONE,
				.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO,
				.colorBlendOp = VK_BLEND_OP_ADD,
				.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
				.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
				.alphaBlendOp = VK_BLEND_OP_ADD,
				.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT
			};

			VkPipelineColorBlendStateCreateInfo colorBlending{
				.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
				.logicOpEnable = VK_FALSE,
				.logicOp = VK_LOGIC_OP_COPY,
				.attachmentCount = 1,
				.pAttachments = &colorBlendAttachment,
				.blendConstants = { 0.0f, 0.0f, 0.0f, 0.0f }
			};

			VkPipelineLayoutCreateInfo pipelineLayoutInfo{
				.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
				.setLayoutCount = 1,
				.pSetLayouts = &descriptorSetLayout,
				.pushConstantRangeCount = 0,
				.pPushConstantRanges = nullptr
			};

			check(vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS, "Failed to create pipeline layout");

			VkGraphicsPipelineCreateInfo pipelineInfo{
				.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
				.stageCount = 2,
				.pStages = shaderStages,
				.pVertexInputState = &vertexInputInfo,
				.pInputAssemblyState = &inputAssembly,
				.pViewportState = &viewportState,
				.pRasterizationState = &rasterizer,
				.pMultisampleState = &multisampling,
				.pDepthStencilState = nullptr,
				.pColorBlendState = &colorBlending,
				.pDynamicState = nullptr,
				.layout = pipelineLayout,
				.renderPass = renderPass,
				.subpass = 0,
				.basePipelineHandle = VK_NULL_HANDLE,
				.basePipelineIndex = -1
			};

			check(vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline) != VK_SUCCESS, "Failed to create graphics pipeline");

			vkDestroyShaderModule(device, vertShaderModule, nullptr);
			vkDestroyShaderModule(device, fragShaderModule, nullptr);
		}

		void createFrameBuffer() {
			swapchainFramebuffers.resize(swapchainImageViews.size());

			for (size_t i = 0; i < swapchainImageViews.size(); i++) {
				VkImageView attachments[] = { swapchainImageViews[i] };

				VkFramebufferCreateInfo framebufferInfo{
					.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
					.renderPass = renderPass,
					.attachmentCount = 1,
					.pAttachments = attachments,
					.width = swapchainExtent.width,
					.height = swapchainExtent.height,
					.layers = 1
				};

				check(vkCreateFramebuffer(device, &framebufferInfo, nullptr, &swapchainFramebuffers[i]) != VK_SUCCESS, "Failed to create framebuffer");
			}
		}

		void createCommandPool() {
			QueueFamilyIndices queueFamilyIndices = findQueueFamilies(physicalDevice, surface);

			VkCommandPoolCreateInfo poolInfo{
				.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
				.flags = 0,
				.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value()
			};

			check(vkCreateCommandPool(device, &poolInfo, nullptr, &commandPool) != VK_SUCCESS, "Failed to create command pool");
		}

		void createVertexBuffer() {
			VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

			VkBuffer stagingBuffer;
			VkDeviceMemory stagingBufferMemory;
			createBuffer(device, physicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

			void* data;
			vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
			memcpy(data, vertices.data(), (size_t)bufferSize);
			vkUnmapMemory(device, stagingBufferMemory);

			createBuffer(device, physicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer, vertexBufferMemory);

			copyBuffer(device, stagingBuffer, vertexBuffer, bufferSize, commandPool, graphicsQueue);

			vkDestroyBuffer(device, stagingBuffer, nullptr);
			vkFreeMemory(device, stagingBufferMemory, nullptr);
		}

		void createIndexBuffer() {
			VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

			VkBuffer stagingBuffer;
			VkDeviceMemory stagingBufferMemory;
			createBuffer(device, physicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

			void* data;
			vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
			memcpy(data, indices.data(), (size_t)bufferSize);
			vkUnmapMemory(device, stagingBufferMemory);

			createBuffer(device, physicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer, indexBufferMemory);

			copyBuffer(device, stagingBuffer, indexBuffer, bufferSize, commandPool, graphicsQueue);

			vkDestroyBuffer(device, stagingBuffer, nullptr);
			vkFreeMemory(device, stagingBufferMemory, nullptr);
		}

		void createUniformBuffers() {
			VkDeviceSize bufferSize = sizeof(UniformBufferObject);

			uniformBuffers.resize(swapchainImages.size());
			uniformBuffersMemory.resize(swapchainImages.size());

			for (size_t i = 0; i < swapchainImages.size(); i++) {
				createBuffer(device, physicalDevice, bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffers[i], uniformBuffersMemory[i]);
			}
		}

		void createDescriptorPool() {
			VkDescriptorPoolSize poolSize{
				.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
				.descriptorCount = static_cast<uint32_t>(swapchainImages.size())
			};

			VkDescriptorPoolCreateInfo poolInfo{
				.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
				.maxSets = static_cast<uint32_t>(swapchainImages.size()),
				.poolSizeCount = 1,
				.pPoolSizes = &poolSize
			};

			check(vkCreateDescriptorPool(device, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS, "Failed to create descriptor pool");
		}

		void createDiscriptorSets() {
			std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, descriptorSetLayout);
			VkDescriptorSetAllocateInfo allocInfo{
				.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
				.descriptorPool = descriptorPool,
				.descriptorSetCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT),
				.pSetLayouts = layouts.data(),
			};

			descriptorSets.resize(MAX_FRAMES_IN_FLIGHT);
			if (vkAllocateDescriptorSets(device, &allocInfo, descriptorSets.data()) != VK_SUCCESS)
				throw std::runtime_error("Failed to allocate descriptor sets");

			for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
				VkDescriptorBufferInfo bufferInfo{
					.buffer = uniformBuffers[i],
					.offset = 0,
					.range = sizeof(UniformBufferObject),
				};

				VkWriteDescriptorSet descriptorWrite{
					.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
					.dstSet = descriptorSets[i],
					.dstBinding = 0,
					.dstArrayElement = 0,

					.descriptorCount = 1,
					.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,

					.pImageInfo = nullptr,
					.pBufferInfo = &bufferInfo,
					.pTexelBufferView = nullptr,
				};

				vkUpdateDescriptorSets(device, 1, &descriptorWrite, 0, nullptr);
			}
		}

		void createCommandBuffers() {
			commandBuffers.resize(swapchainFramebuffers.size());

			VkCommandBufferAllocateInfo allocInfo{
				.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
				.commandPool = commandPool,
				.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
				.commandBufferCount = (uint32_t)commandBuffers.size(),
			};

			check(vkAllocateCommandBuffers(device, &allocInfo, commandBuffers.data()) != VK_SUCCESS, "Failed to create command buffers");

			for (size_t i = 0; i < commandBuffers.size(); i++) {
				VkCommandBufferBeginInfo beginInfo{
					.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
					.flags = 0,
					.pInheritanceInfo = nullptr,
				};

				check(vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS, "Failed to begin command buffer recording");

				VkClearValue clearColor = { {{0.0f, 0.0f, 0.0f, 1.0f}} };

				VkRenderPassBeginInfo renderPassInfo{
					.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
					.renderPass = renderPass,
					.framebuffer = swapchainFramebuffers[i],
					.renderArea = {.offset = {0, 0}, .extent = swapchainExtent},
					.clearValueCount = 1,
					.pClearValues = &clearColor,
				};

				vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

				vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, this->graphicsPipeline);

				VkBuffer vertexBuffers[] = { vertexBuffer };
				VkDeviceSize offsets[] = { 0 };
				vkCmdBindVertexBuffers(commandBuffers[i], 0, 1, vertexBuffers, offsets);

				vkCmdBindIndexBuffer(commandBuffers[i], indexBuffer, 0, VK_INDEX_TYPE_UINT32);

				vkCmdBindDescriptorSets(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descriptorSets[currentFrame], 0, nullptr);

				vkCmdDrawIndexed(commandBuffers[i], static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);//Error

				vkCmdEndRenderPass(commandBuffers[i]);

				check(vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS, "Failed to record command buffer");
			}
		}

		void createSyncObjects() {
			imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
			renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
			inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
			imagesInFlight.resize(MAX_FRAMES_IN_FLIGHT);

			VkSemaphoreCreateInfo semaphoreInfo{
				.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO
			};

			VkFenceCreateInfo fenceInfo{
				.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
				.flags = VK_FENCE_CREATE_SIGNALED_BIT,
			};

			for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
				check(vkCreateSemaphore(device, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS ||
					vkCreateSemaphore(device, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS ||
					vkCreateFence(device, &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS, "Failed to create semaphores");
			}
		}

		//Debug
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

		void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
			createInfo = {
				.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
				.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
				.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
				.pfnUserCallback = debugCallback
			};
		}

		static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
			VkDebugUtilsMessageTypeFlagsEXT messageType,
			const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
			void* pUserData) {
			std::cerr << "Validation Layer: " << pCallbackData->pMessage << std::endl;

			if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {};

			return VK_FALSE;
		}

		VkResult vkCreateDebugUtilsMessengerEXT(VkInstance instance,
			const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
			const VkAllocationCallbacks* pAllocator,
			VkDebugUtilsMessengerEXT* pDebugMessenger) {
			auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
			if (func != nullptr) {
				return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
			}
			else {
				return VK_ERROR_EXTENSION_NOT_PRESENT;
			}
		}


		void setupDebugMessenger() {
			if (VALIDATE) return;

			VkDebugUtilsMessengerCreateInfoEXT createInfo{};
			populateDebugMessengerCreateInfo(createInfo);

			check(vkCreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS, "Failed to set up debug messenger!");
		}

		void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
			auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
			if (func != nullptr) {
				func(instance, debugMessenger, pAllocator);
			}
		}


		//MAIN
		void drawFrame(bool *framebufferResized) {
			vkWaitForFences(device, 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

			uint32_t imageIndex;
			VkResult result = vkAcquireNextImageKHR(device, swapchain, UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

			if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || *framebufferResized) {
				*framebufferResized = false;
				recreateSwapChain();
				return;
			}
			else if (result != VK_SUCCESS) {
				throw std::runtime_error("Failed to acquire swap chain image");
			}

			if (imagesInFlight[imageIndex] != VK_NULL_HANDLE) {
				vkWaitForFences(device, 1, &imagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
			}
			imagesInFlight[imageIndex] = inFlightFences[currentFrame];

			updateUniformBuffer(imageIndex);

			VkSemaphore waitSemaphores[] = { imageAvailableSemaphores[currentFrame] };
			VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

			VkSemaphore signalSemaphores[] = { renderFinishedSemaphores[currentFrame] };

			VkSubmitInfo submitInfo{
				.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,

				.waitSemaphoreCount = 1,
				.pWaitSemaphores = waitSemaphores,
				.pWaitDstStageMask = waitStages,
				.commandBufferCount = 1,
				.pCommandBuffers = &commandBuffers[imageIndex],

				.signalSemaphoreCount = 1,
				.pSignalSemaphores = signalSemaphores,
			};

			vkResetFences(device, 1, &inFlightFences[currentFrame]);

			if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS)
				throw std::runtime_error("Failed to submit draw command");

			VkSwapchainKHR swapChains[] = { swapchain };

			VkPresentInfoKHR presentInfo{
				.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
				.waitSemaphoreCount = 1,
				.pWaitSemaphores = signalSemaphores,
				.swapchainCount = 1,
				.pSwapchains = swapChains,
				.pImageIndices = &imageIndex,
				.pResults = nullptr,
			};

			vkQueuePresentKHR(presentQueue, &presentInfo);

			vkQueueWaitIdle(presentQueue);

			currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
		}

		void updateUniformBuffer(uint32_t currentImage) {
			static auto startTime = std::chrono::high_resolution_clock::now();

			auto currentTime = std::chrono::high_resolution_clock::now();
			float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

			UniformBufferObject ubo{
				.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(30.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
				.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
				.proj = glm::perspective(glm::radians(45.0f), swapchainExtent.width / (float)swapchainExtent.height, 0.1f, 10.0f),
			};
			ubo.proj[1][1] *= -1;

			void* data;
			vkMapMemory(device, uniformBuffersMemory[currentImage], 0, sizeof(ubo), 0, &data);
			memcpy(data, &ubo, sizeof(ubo));
			vkUnmapMemory(device, uniformBuffersMemory[currentImage]);
		}

		void cleanup() {
			cleanupSwapChain();

			vkDestroyBuffer(device, indexBuffer, nullptr);
			vkFreeMemory(device, indexBufferMemory, nullptr);

			vkDestroyBuffer(device, vertexBuffer, nullptr);
			vkFreeMemory(device, vertexBufferMemory, nullptr);

			for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
				vkDestroySemaphore(device, renderFinishedSemaphores[i], nullptr);
				vkDestroySemaphore(device, imageAvailableSemaphores[i], nullptr);
				vkDestroyFence(device, inFlightFences[i], nullptr);
			}

			vkDestroyCommandPool(device, commandPool, nullptr);

			vkDestroyDevice(device, nullptr);

			if (VALIDATE) {
				DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
			}

			vkDestroySurfaceKHR(instance, surface, nullptr);
			vkDestroyInstance(instance, nullptr);

			glfwDestroyWindow(window);

			glfwTerminate();
		}

		//Swapchain
		void cleanupSwapChain() {
			for (auto framebuffer : swapchainFramebuffers) {
				vkDestroyFramebuffer(device, framebuffer, nullptr);
			}

			vkDestroyPipeline(device, this->graphicsPipeline, nullptr);

			vkDestroyPipelineLayout(device, this->pipelineLayout, nullptr);

			vkDestroyRenderPass(device, renderPass, nullptr);

			for (auto imageView : swapchainImageViews) {
				vkDestroyImageView(device, imageView, nullptr);
			}

			vkDestroySwapchainKHR(device, swapchain, nullptr);

			for (size_t i = 0; i < swapchainImages.size(); i++) {
				vkDestroyBuffer(device, uniformBuffers[i], nullptr);
				vkFreeMemory(device, uniformBuffersMemory[i], nullptr);
			}

			vkDestroyDescriptorPool(device, descriptorPool, nullptr);

			vkDestroyDescriptorSetLayout(device, descriptorSetLayout, nullptr);
		}

		void recreateSwapChain() {
			int width = 0, height = 0;
			glfwGetFramebufferSize(window, &width, &height);
			while (width == 0 || height == 0) {
				glfwGetFramebufferSize(window, &width, &height);
				glfwWaitEvents();
			}

			vkDeviceWaitIdle(device);

			cleanupSwapChain();

			createSwapChain();
			createImageViews();
			createRenderPass();
			createDescriptorSetLayout();
			createGraphicsPipeline();
			createFrameBuffer();
			createUniformBuffers();
			createDescriptorPool();
			createDiscriptorSets();
			createCommandBuffers();
		}
	};
}