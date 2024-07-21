#ifndef VULKAN_H
#define VULKAN_H

#include <Window/Window.h>
#include <vulkan/vulkan.h>

#include "Components/ValidationLayers.h"
#include "Components/Device.h"
#include "Components/SwapChain.h"

namespace OddityEngine {
	class Vulkan : public Updateable {
	protected:
		VkDebugUtilsMessengerEXT _debugMessenger;

		Window *_window;

		VkInstance _instance;

		VkSurfaceKHR _surface;

		Device _device;
		VkPhysicalDevice _physicalDevice;
		VkDevice _logicalDevice;

		SwapChain _swapChain;


		std::vector<const char *> _deviceExtensions = {
			VK_KHR_SWAPCHAIN_EXTENSION_NAME
		};

		/// Helpers
		std::vector<const char *> get_required_extensions();

		bool check_validation_layer_support() const;

		/// Setups
		void create_instance();

		void setup_debug_messenger();

		void create_surface();

	public:
		Vulkan(Window *window);

		~Vulkan();

		void update() override;

		static VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserData);
	};
}

#endif //VULKAN_H
