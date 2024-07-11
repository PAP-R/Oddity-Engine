#ifndef VULKAN_H
#define VULKAN_H

#include <Window/Window.h>
#include <vulkan/vulkan.h>

namespace OddityEngine {
	class Vulkan {
	protected:
        VkDebugUtilsMessengerEXT debugMessenger;

		Window* _window;

		VkInstance instance;




		std::vector<const char*> _validationLayers = {
			"VK_LAYER_KHRONOS_validation"
		};

#ifdef NDEBUG
		const bool enableValidataionLayers = false;
#else
		const bool enableValidataionLayers = true;
#endif

		/// Helpers
		std::vector<const char *> get_required_extensions();
		bool check_validation_layer_support() const;

		/// Setups
		void create_instance();
        void setup_debug_messenger();

	public:
		Vulkan(Window* window);

		static VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);
	};
}

#endif //VULKAN_H
