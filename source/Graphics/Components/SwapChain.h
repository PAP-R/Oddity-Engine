#ifndef SWAPCHAIN_H
#define SWAPCHAIN_H

#include <vulkan/vulkan_core.h>

#include <vector>

#include <glm/glm.hpp>

struct SwapChainSupportDetails {
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

SwapChainSupportDetails query_swap_chain_support(const VkPhysicalDevice& device, const VkSurfaceKHR& surface);

class SwapChain {
protected:
	VkSwapchainKHR _swapChain = VK_NULL_HANDLE;
	VkDevice _device = VK_NULL_HANDLE;

public:
	std::vector<VkImage> swapChainImages;
	std::vector<VkImageView> swapChainImageViews;
	VkFormat swapChainFormat;
	VkExtent2D swapChainExtent{};

    std::vector<VkFramebuffer> swapChainFramebuffers;

	SwapChain() = default;
	SwapChain(VkPhysicalDevice physicalDevice, VkDevice device, const VkSurfaceKHR& surface, const glm::ivec2& size);
	~SwapChain();

	operator VkSwapchainKHR () const;
};



#endif //SWAPCHAIN_H
